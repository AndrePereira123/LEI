#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "parser.hpp"
#include "list.hpp"

using namespace std;


float alfa_, radius, beta_;

List* lista = nullptr;

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

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void draw(List* lista) {
	for (int i = 0; i < lista->size(); i++) {
		Vertex vertice = lista->get(i);
		glVertex3f(vertice.getX(), vertice.getY(), vertice.getZ());
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





    // put the geometric transformations here

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // drawing
    
	glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
		draw(lista);
    glEnd();
   

	// End of frame
	glutSwapBuffers();
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


	lista = (parseShapes(world.group.models));

	if (lista != NULL) {
		// init GLUT and the window
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
			glutInitWindowPosition(100,100);
			glutInitWindowSize(800,800);
			glutCreateWindow("CG@DI-UM");
	
			glutReshapeWindow(world.window.width, world.window.height);
				
		// Required callback registry 
			glutDisplayFunc(renderScene);
			glutReshapeFunc(changeSize);
		
			
		// put here the registration of the keyboard callbacks
			glutSpecialFunc(processSpecialKeys);
		
		
		//  OpenGL settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			
		// enter GLUT's main cycle
			glutMainLoop();
			
	
		delete(lista);
		return 0;
	} else {
		cerr << "The XML file was not correctly parsed." << endl;
		return 1;
	}
	
}