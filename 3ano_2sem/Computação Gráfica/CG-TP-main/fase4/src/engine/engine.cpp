#include <stdio.h>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <functional>

#include "parser.hpp"
#include "list.hpp"


using namespace std;

int frameCount = 0;
float fps = 0.0f;
int previousTime = 0;

int startX, startY, tracking = 0;

int renderMode = GL_LINE;
bool showCatmullRomCurves = true;

float alfa_, radius, beta_;

vector<List*> listas;

vector<vector<Transform>> transformations;

vector<int*> n_vertices;

GLuint vertexCount;

vector<GLuint> buffers, normais, textures;

map<string, GLuint> textureIDs;
vector<string> modelTextureFiles;
vector<Color> modelMaterials;


float camX, camY, camZ;

World world; 

void initializeCamCoordinates() {
	float x = world.camera.position.x;
	float y = world.camera.position.y;
	float z = world.camera.position.z;

	float t = sqrt(z*z + x*x);

	radius = sqrt(x*x + y*y + z*z);
	beta_ = asin(y / radius); // função inversa do sin para ir buscar o angulo
	alfa_ = atan(x / z); 
}

void spherical2Cartesian() {

	world.camera.position.x = radius * cos(beta_) * sin(alfa_);
	world.camera.position.y = radius * sin(beta_);
	world.camera.position.z = radius * cos(beta_) * cos(alfa_);
}

// catmull operations


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};
			
	float tt[4] = {t*t*t, t*t, t, 1};
	float td[4] = {3*t*t, 2*t, 1, 0};
	// Compute A = M * P
	float aux[4];
	for (int i = 0; i < 3; i++) { // x,y,z
		float pp[4] = {p0[i], p1[i], p2[i], p3[i]};
		multMatrixVector((float*)m, pp, aux);
		pos[i] = tt[0] * aux[0] +
				 tt[1] * aux[1] +
				 tt[2] * aux[2] +
				 tt[3] * aux[3] ;

		deriv[i] = td[0] * aux[0] +
				   td[1] * aux[1] +
				   td[2] * aux[2] +
				   td[3] * aux[3] ;
	}
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, vector<Vertex>& points) {

	int n_points = points.size();

	float t = gt * n_points; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + n_points-1)%n_points;	
	indices[1] = (indices[0]+1)%n_points;
	indices[2] = (indices[1]+1)%n_points; 
	indices[3] = (indices[2]+1)%n_points;

	float p0[3] = { points[indices[0]].getX(), points[indices[0]].getY(), points[indices[0]].getZ() };
    float p1[3] = { points[indices[1]].getX(), points[indices[1]].getY(), points[indices[1]].getZ() };
    float p2[3] = { points[indices[2]].getX(), points[indices[2]].getY(), points[indices[2]].getZ() };
    float p3[3] = { points[indices[3]].getX(), points[indices[3]].getY(), points[indices[3]].getZ() };

	getCatmullRomPoint(t, p0, p1, p2, p3, pos, deriv);
}

void renderCatmullRomCurve(vector<Vertex>& points) {
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	// draw curve using line segments with GL_LINE_LOOP
	float pos[3], deriv[3];
	glBegin(GL_LINE_LOOP);
	float npoints = 100;
	for (int i = 0; i < npoints; i++) {
		getGlobalCatmullRomPoint(i/npoints, pos, deriv, points);
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
	if (world.lights.lights.size() > 0) {
		glEnable(GL_LIGHTING);
	}
	if (modelTextureFiles.size() > 0) {
		glEnable(GL_TEXTURE_2D);
	}
}	

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	if (world.camera.projection.far && world.camera.projection.fov && world.camera.projection.near) {
		gluPerspective(world.camera.projection.fov ,ratio, world.camera.projection.near ,world.camera.projection.far);
	} else {
		gluPerspective(60.0f ,ratio, 1.0f ,1000.0f);
	}

	// return to the model vie%w matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void loadTexturesRecursive(const Group& group) {
    for (const Model& model : group.models) {
        if (!model.texture_file.empty()) {
            string texPath = "../textures/" + model.texture_file;
            FILE* file = fopen(texPath.c_str(), "r");
            if (!file) {
                cerr << "Erro ao abrir textura: " << texPath << endl;
                continue;
            }
            fclose(file);

            if (textureIDs.find(model.texture_file) == textureIDs.end()) {
                unsigned int t, tw, th;
                unsigned char *texData;

                ilGenImages(1, &t);
                ilBindImage(t);
                ilLoadImage((ILstring)texPath.c_str());
                tw = ilGetInteger(IL_IMAGE_WIDTH);
                th = ilGetInteger(IL_IMAGE_HEIGHT);
                ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
                texData = ilGetData();

                GLuint tex;
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D, tex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
                glGenerateMipmap(GL_TEXTURE_2D);

                textureIDs[model.texture_file] = tex;
            }
        }
    }
    for (const Group& subgroup : group.subgroups) {
        loadTexturesRecursive(subgroup);
    }
}

void loadTexture() {
    loadTexturesRecursive(world.group);
}

void prepareFigures() {

	int indice_list = 0;

	for (List* lista: listas) {	

		float *v, *n, *t;

		int size = n_vertices.size();

		v = (float *)malloc(sizeof(float) * (*n_vertices[indice_list]) * 3);
		
		n = (float *)malloc(sizeof(float) * (*n_vertices[indice_list]) * 3);

		t = (float *)malloc(sizeof(float) * (*n_vertices[indice_list]) * 2);


		int vertex_indice = 0;

		for (int i = 0; i < lista->size(); i++) {
			Vertex vertice = lista->get(i);
			v[vertex_indice*3 + 0] = vertice.getX();
			v[vertex_indice*3 + 1] = vertice.getY();
			v[vertex_indice*3 + 2] = vertice.getZ();

			n[vertex_indice*3 + 0] = vertice.getnormalX();
            n[vertex_indice*3 + 1] = vertice.getnormalY();
            n[vertex_indice*3 + 2] = vertice.getnormalZ();

			t[vertex_indice*2 + 0] = vertice.gettextS();
			t[vertex_indice*2 + 1] = vertice.gettextT();

			vertex_indice++;
		}
		

		vertexCount = *n_vertices[indice_list];

		glGenBuffers(1, &buffers[indice_list]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[indice_list]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v,     GL_STATIC_DRAW);

		glGenBuffers(1, &normais[indice_list]);
		glBindBuffer(GL_ARRAY_BUFFER, normais[indice_list]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, n,     GL_STATIC_DRAW);

		glGenBuffers(1, &textures[indice_list]);
		glBindBuffer(GL_ARRAY_BUFFER, textures[indice_list]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, t,     GL_STATIC_DRAW);

		free(v);
		free(n);
		free(t);

		indice_list++;

	}
}

void draw() {
    for (int i = 0; i < n_vertices.size(); i++) {
        glPushMatrix();

        string textureFile = "";
        if (i < modelTextureFiles.size()) {
            textureFile = modelTextureFiles[i];
        }

		//aplicar material/cor ao objeto a ser desenhado
		if (i < modelMaterials.size()) {
			Color c = modelMaterials[i];
			GLfloat ambient[4]  = {stof(c.ambient.R)/255.0f,  stof(c.ambient.G)/255.0f,  stof(c.ambient.B)/255.0f,  1.0f};
			GLfloat diffuse[4]  = {stof(c.diffuse.R)/255.0f,  stof(c.diffuse.G)/255.0f,  stof(c.diffuse.B)/255.0f,  1.0f};
			GLfloat specular[4] = {stof(c.specular.R)/255.0f, stof(c.specular.G)/255.0f, stof(c.specular.B)/255.0f, 1.0f};
			GLfloat emissive[4] = {stof(c.emissive.R)/255.0f, stof(c.emissive.G)/255.0f, stof(c.emissive.B)/255.0f, 1.0f};

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, stof(c.shininess.value));
		}


        // Verificar se a textura existe no mapa
        bool hasTexture = !textureFile.empty() && textureIDs.find(textureFile) != textureIDs.end();
        
		// texturas
        if (hasTexture) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureIDs[textureFile]);
        } else {
            glDisable(GL_TEXTURE_2D);
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
        glVertexPointer(3,GL_FLOAT,0,0);

        glBindBuffer(GL_ARRAY_BUFFER, normais[i]);
        glNormalPointer(GL_FLOAT, 0, 0);

        if (hasTexture) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, textures[i]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
        } else {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        vector<Transform> transformation_list;

        transformation_list = transformations[i];

        for(Transform transformation: transformation_list) {

            vector<pair<int, function<void()>>> operations;

            if (transformation.rotate.order != -1) {
                operations.push_back({transformation.rotate.order, [&]() {
                    float angle = transformation.rotate.angle;
                    if (transformation.rotate.time > 0) {
                        angle = ((glutGet(GLUT_ELAPSED_TIME) / 1000.0f) * 360.0f) / transformation.rotate.time;
                        // n dar overflow ao angulo
                        if (angle >= 360.0f) {
                            angle -= 360.0f;
                        } else if (angle < 0.0f) {
                            angle += 360.0f;
                        }
                    }
                    glRotatef(angle, transformation.rotate.x, transformation.rotate.y, transformation.rotate.z);
                }});
            } 

            if (transformation.translate.order != -1) {
                operations.push_back({transformation.translate.order, [&]() {
                    // se for dinamico
                    if (transformation.translate.time != 0) {
                        float elapsed = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 
                        float t = elapsed / transformation.translate.time;
                        //overflow
                        if (t >= 1.0f) {
                            t -= (int)t;
                        }

                        float pos[3];
                        static float Yi[3] = {0, 1, 0};
                        float Xi[3], Zi[3];
                        float m[16];

                        //geração das linhas
                        if (showCatmullRomCurves) {
                            renderCatmullRomCurve(transformation.translate.points);
                        }

                        getGlobalCatmullRomPoint(t, pos, Xi, transformation.translate.points);

                        glTranslatef(pos[0], pos[1], pos[2]);

                        if (transformation.translate.align) {
                            //Zi
                            cross(Xi, Yi, Zi);
    
                            //Yi
                            cross(Zi, Xi, Yi);
    
                            normalize(Xi);
                            normalize(Yi);
                            normalize(Zi);
                            //construir a matriz de rotação
                            buildRotMatrix(Xi, Yi, Zi, m);
    
                            glMultMatrixf(m);
                        }


                    } else {
                        glTranslatef(transformation.translate.x, transformation.translate.y, transformation.translate.z);
                    }
                }});
            } 

            if (transformation.scale.order != -1) {
                operations.push_back({transformation.scale.order, [&]() {
                    glScalef(transformation.scale.x, transformation.scale.y, transformation.scale.z);
                }});
            } 
    
            sort(operations.begin(), operations.end(), [](const pair<int, function<void()>>& a, const pair<int, function<void()>>& b) {
                return a.first < b.first;
            });

            for (auto& op : operations) {
                op.second();
            }
        }

        glDrawArrays(GL_TRIANGLES, 0, *n_vertices[i] * 3);

        if (hasTexture) {
            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        glPopMatrix();

        // desenho das normais

		/* glDisable(GL_LIGHTING);

        glColor3f(1.0f, 0.0f, 0.0f); // vermelho

        glBegin(GL_LINES);

        for (int j = 0; j < *n_vertices[i]; ++j) {

            Vertex v = listas[i]->get(j);

            float x = v.getX();
            float y = v.getY();
            float z = v.getZ();

            float nx = v.getnormalX();
            float ny = v.getnormalY();
            float nz = v.getnormalZ();

            glVertex3f(x, y, z);

            float scale = 0.2f; 
            glVertex3f(x + nx * scale, y + ny * scale, z + nz * scale);
        }

		glEnd();

		glEnable(GL_LIGHTING); */

    }
}

void setupLights() {
	for (int i = 0; i < world.lights.lights.size(); i++){
		Light light = world.lights.lights[i];
		GLenum light_id = GL_LIGHT0 + i;

		if (light.type == "point") {
			GLfloat pos[4] = {light.posX, light.posY, light.posZ, 1.0f};
			glLightfv(light_id, GL_POSITION, pos);
		} else if (light.type == "directional") {
			GLfloat dir[4] = {light.dirX, light.dirY, light.dirZ, 0.0f};
			glLightfv(light_id, GL_POSITION, dir);
		} else if (light.type == "spotlight" || light.type == "spot") {
			GLfloat pos[4] = {light.posX, light.posY, light.posZ, 1.0f};
			GLfloat dir[3] = {light.dirX, light.dirY, light.dirZ};
			glLightfv(light_id, GL_POSITION, pos);	
			glLightfv(light_id, GL_SPOT_DIRECTION, dir);
			glLightf(light_id, GL_SPOT_CUTOFF, light.cutoff);
		}
	}
}

void renderScene() {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(world.camera.position.x,world.camera.position.y,world.camera.position.z, 
			  world.camera.lookAt.x,world.camera.lookAt.y,world.camera.lookAt.z,
			  world.camera.up.x,world.camera.up.y,world.camera.up.z);

	glDisable(GL_LIGHTING);

// put axis drawing in here
	glBegin(GL_LINES);
		// X
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);

		// Y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);

		//Z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	if (world.lights.lights.size() > 0) {
		glEnable(GL_LIGHTING);
	}

    // put the geometric transformations here

	setupLights();

    glPolygonMode(GL_FRONT_AND_BACK, renderMode);
    
    // drawing
    
	glColor3f(1,1,1);
	
	draw();
    
	frameCount++;
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000) {
		fps = frameCount * 1000.0f / timeInterval;
		frameCount = 0;
		previousTime = currentTime;

		char title[64];
		sprintf(title, "CG@DI-UM - FPS: %.2f", fps);
		glutSetWindowTitle(title);
	}

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'f':
        case 'F':
            renderMode = GL_FILL;
            glutPostRedisplay();
            break;
        case 'w': 
        case 'W':
            renderMode = GL_LINE;
            glutPostRedisplay();
            break;
        case 'p': 
        case 'P':
            renderMode = GL_POINT;
            glutPostRedisplay();
            break;
        case 'c': // Toggle Catmull-Rom curves
        case 'C':
            showCatmullRomCurves = !showCatmullRomCurves;
            glutPostRedisplay();
            break;
    }
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa_ -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa_ += 0.1; break;

	case GLUT_KEY_UP:
		beta_ += 0.1f;
		if (beta_ > 1.5f)
			beta_ = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta_ -= 0.1f;
		if (beta_ < -1.5f)
			beta_ = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) 
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alfa_ += (xx - startX);
			beta_ += (yy - startY);
		}
		else if (tracking == 2) {
			
			radius -= yy - startY;
			if (radius < 3)
				radius = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
    if (!tracking)
        return;

    int deltaX = xx - startX;
    int deltaY = yy - startY;

    if (tracking == 1) {
        alfa_ += deltaX * 0.01f;
        beta_ += deltaY * 0.01f;
        
        if (beta_ > 1.5f)
            beta_ = 1.5f;
        else if (beta_ < -1.5f)
            beta_ = -1.5f;
    }
    else if (tracking == 2) {
        radius -= deltaY * 0.1f;
        if (radius < 3.0f)
            radius = 3.0f;
    }

    // Update camera position
    spherical2Cartesian();
    
    startX = xx;
    startY = yy;
    
    glutPostRedisplay();
}


void initGL() {

	// OpenGL settings 
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	
	// init
		if (world.lights.lights.size() > 0) {
			glEnable(GL_RESCALE_NORMAL);
			
			float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
			
			glEnable(GL_LIGHTING);
			for(int i = 0; i < world.lights.lights.size(); i++) {
				GLenum light_id = GL_LIGHT0 + i;
				glEnable(light_id);
				GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
				glLightfv(light_id, GL_DIFFUSE, white);
            	glLightfv(light_id, GL_SPECULAR, white);
			}
		}
		
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		prepareFigures();
		
}

int main(int argc, char **argv) {
	// EXEMPLO
	
	// USAGE ./engine test_1_1.xml


	if (argc < 2 || argc > 2) {
		cerr << "Invalid input.\nUsage: ./engine <file>\nFile needs to be xml and needs to be in tests directory." << endl;
		return 1;
	}

	string ficheiro = argv[1];
	//verificação se o ficheiro existe na pasta

	string path = "../tests/" + ficheiro;

    if(!parseXML(path.c_str(), world)) {
        return 1;
    }


	initializeCamCoordinates();

	int total_vertices = 0;

	vector<Transform> inicial;

	parseShapes(world.group, listas, transformations, &total_vertices, inicial, n_vertices, modelTextureFiles, modelMaterials);


	if (listas.size() > 0) {
		// cada model tem o seu respectivo buffer
			buffers.resize(listas.size());
			normais.resize(listas.size());
			textures.resize(listas.size());

		// init GLUT and the window
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
			glutInitWindowPosition(100,100);
			glutInitWindowSize(800,800);
			glutCreateWindow("CG@DI-UM");
	
			glutReshapeWindow(world.window.width, world.window.height);
				
		// Required callback registry 
			glutDisplayFunc(renderScene);
			glutIdleFunc(renderScene);
			glutReshapeFunc(changeSize);
		
			
		// put here the registration of the keyboard callbacks
			glutSpecialFunc(processSpecialKeys);
			glutMouseFunc(processMouseButtons);
			glutMotionFunc(processMouseMotion);
			glutKeyboardFunc(processKeys);
		
		#ifndef __APPLE__	
		// init GLEW
			glewInit();
		#endif	
			ilInit();
			loadTexture();


		// start OpenGL
			initGL();

		// enter GLUT's main cycle
			glutMainLoop();
			
	
		//delete(lista);
		return 0;
	} else {
		cerr << "The XML file was not correctly parsed." << endl;
		return 1;
	}
	
}