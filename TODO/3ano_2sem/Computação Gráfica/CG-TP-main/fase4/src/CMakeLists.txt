cmake_minimum_required(VERSION 3.5)

# Project Name
project(CG-TP)

make_directory(objects)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1) # cuidado com isto
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(../tinyxml)
add_library(tinyxml2 ../tinyxml/tinyxml2.cpp)  # Criar biblioteca estática

include_directories(../include)

# Adicionar o executável generator
add_executable(generator
    generator/generator.cpp
    generator/plane.cpp
    generator/vertex.cpp
    generator/box.cpp
    generator/cone.cpp
    generator/sphere.cpp
    generator/ring.cpp
    generator/patch.cpp
)

add_executable(engine
    engine/engine.cpp
    engine/parser.cpp
    generator/vertex.cpp
)



find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif()

if(WIN32)
    message(STATUS "Toolkits_DIR set to: ${TOOLKITS_FOLDER}")
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")

    if(NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(FATAL_ERROR "GLUT not found")
    endif()

    if(NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
        message(FATAL_ERROR "GLEW not found")
    endif()

    if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")
		message(ERROR ": DevIL not found")
	endif ()	
	

    include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil)
    target_link_libraries(generator ${OPENGL_LIBRARIES}
                          ${TOOLKITS_FOLDER}/glut/glut32.lib
                          ${TOOLKITS_FOLDER}/devil/devIL.lib
                          ${TOOLKITS_FOLDER}/glew/glew32.lib)

    if(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
                                                   AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
        file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
        file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})

    find_package(DevIL REQUIRED)
	link_libraries(${IL_LIBRARIES} )
	include_directories(${IL_INCLUDE_DIR})

    if(NOT APPLE)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
        target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} tinyxml2)
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${IL_LIBRARIES} tinyxml2)
    else()
        target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} tinyxml2)
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES} tinyxml2)
    endif()

    if(NOT GLUT_FOUND)
        message(FATAL_ERROR "GLUT not found!")
    endif()

    target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} tinyxml2 -lglut)
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES} tinyxml2 -lglut)
endif()