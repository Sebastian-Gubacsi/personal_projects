/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added lighting, shading, culling, and animation
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Menu.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Light.hpp"

GLint winWidth = 800, winHeight = 800;
GLint isInMove = 0,
	  xbegin = 0,
	  csType = 1, /* coordinate system type: 1 for MCS, 2 for WCS, 3 for VCS, 4 for Light */
	  transType = 4;

int cullingMode = 0;  // 0=no culling, 1=OpenGL, 2=my backface
int shadingMode = 0;  // 0=wireframe, 1=my constant, 2=OpenGL flat, 3=OpenGL smooth
int animationMode = 0; // 0=off, 1=single, 2=solar system
bool lightOn = false;

Shape *selectObj = NULL;  // pointer to select object
Light *myLight = NULL;

World myWorld;
Camera myCamera;

// Animation variables
GLfloat singleObjectAngle = 0.0;
GLfloat sunAngle = 0.0;
GLfloat earthOrbitAngle = 0.0;
GLfloat earthRotationAngle = 0.0;
GLfloat moonOrbitAngle = 0.0;
GLfloat moonRotationAngle = 0.0;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myCamera.setProjectionMatrix();

	// Setup lighting based on mode
	if (shadingMode >= 2 && lightOn) {
		// OpenGL lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		myLight->applyOpenGLLight(GL_LIGHT0);
		
		if (shadingMode == 2) {
			glShadeModel(GL_FLAT);
		} else {
			glShadeModel(GL_SMOOTH);
		}
	} else {
		glDisable(GL_LIGHTING);
	}
	
	// Setup culling
	if (cullingMode == 1) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_CULL_FACE);
		if (shadingMode > 0) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	myWorld.draw();
	
	// Draw light if on
	if (lightOn && myLight != NULL) {
		glDisable(GL_LIGHTING);
		myLight->draw();
	}
	
	glFlush();
	glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = newWidth;
	winHeight = newHeight;
}

void mouseActionFcn(int button, int state, int xMouse, int yMouse) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		isInMove = 1;
		xbegin = xMouse;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isInMove = 0;
	}
}

void mouseMotionFcn(GLint xMouse, GLint yMouse) {
	if (isInMove) {
		if (csType == 1){     //  Model transformations
			MCSTransform(xMouse);
		}
		else if (csType == 2){ // WCS transformations
			WCSTransform(xMouse);
		}
		else if(csType == 3){ // VCS transformations
			VCSTransform(xMouse);
		}
		else if(csType == 4){ // Light transformations
			LightTransform(xMouse);
		}
		glutPostRedisplay();
	}
}

void animationUpdate(int value) {
	if (animationMode == 1) {
		// Single object animation
		if (selectObj != NULL) {
			// Reset and rotate around own z-axis
			GLfloat x0 = selectObj->getMC().mat[0][3];
			GLfloat y0 = selectObj->getMC().mat[1][3];
			GLfloat z0 = selectObj->getMC().mat[2][3];
			GLfloat rx = selectObj->getMC().mat[0][2];
			GLfloat ry = selectObj->getMC().mat[1][2];
			GLfloat rz = selectObj->getMC().mat[2][2];
			
			selectObj->rotate(x0, y0, z0, rx, ry, rz, 2.0);
			singleObjectAngle += 2.0;
		}
	} else if (animationMode == 2) {
		// Solar system animation
		Shape* house = myWorld.searchById(3);  // Sun
		Shape* cube = myWorld.searchById(1);   // Earth
		Shape* pyramid = myWorld.searchById(2); // Moon
		
		if (house != NULL && cube != NULL && pyramid != NULL) {
			// Reset positions to start
			static bool initialized = false;
			if (!initialized) {
				house->reset();
				house->translate(0, 0, 0);
				cube->reset();
				cube->translate(2.0, 0, 0);
				pyramid->reset();
				pyramid->translate(2.5, 0, 0);
				initialized = true;
			}
			
			// Sun rotates around its own z-axis (WCS z-axis at origin)
			house->rotateOrigin(0, 0, 0, 0, 0, 1, 1.0);
			sunAngle += 1.0;
			
			// Earth: orbit around sun and rotate around its own axis
			cube->rotateOrigin(0, 0, 0, 0, 0, 1, 2.0); // Orbit
			GLfloat ex = cube->getMC().mat[0][3];
			GLfloat ey = cube->getMC().mat[1][3];
			GLfloat ez = cube->getMC().mat[2][3];
			GLfloat erx = cube->getMC().mat[0][2];
			GLfloat ery = cube->getMC().mat[1][2];
			GLfloat erz = cube->getMC().mat[2][2];
			cube->rotate(ex, ey, ez, erx, ery, erz, 3.0); // Self rotation
			earthOrbitAngle += 2.0;
			earthRotationAngle += 3.0;
			
			// Moon: orbit around earth and rotate around its own axis
			pyramid->rotateOrigin(ex, ey, ez, 0, 0, 1, 5.0); // Orbit around earth
			GLfloat mx = pyramid->getMC().mat[0][3];
			GLfloat my = pyramid->getMC().mat[1][3];
			GLfloat mz = pyramid->getMC().mat[2][3];
			GLfloat mrx = pyramid->getMC().mat[0][2];
			GLfloat mry = pyramid->getMC().mat[1][2];
			GLfloat mrz = pyramid->getMC().mat[2][2];
			pyramid->rotate(mx, my, mz, mrx, mry, mrz, 4.0); // Self rotation
			moonOrbitAngle += 5.0;
			moonRotationAngle += 4.0;
		}
	}
	
	if (animationMode > 0) {
		glutPostRedisplay();
		glutTimerFunc(16, animationUpdate, 0); // ~60 FPS
	}
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	selectObj = myWorld.searchById(1);  // Select cube by default
	
	// Initialize light
	myLight = new Light(POINT_LIGHT, Point(), 1.0);
	myLight->setPosition(5.0, 5.0, 5.0);
	myLight->setColor(1.0, 1.0, 1.0);
	
	// Verify object was found
	if (selectObj == NULL) {
		printf("ERROR: Could not find object with ID 1\n");
	} else {
		printf("Initial object selected: ID = %d\n", selectObj->getId());
	}
	
	printf("\n=== Controls ===\n");
	printf("Right-click for menu\n");
	printf("Left-click and drag to apply transformations\n");
	printf("\n=== Features ===\n");
	printf("1. Culling: No culling, My back-face, OpenGL\n");
	printf("2. Lighting: Turn on/off, change intensity, transform\n");
	printf("3. Shading: No shading, My constant, OpenGL flat, OpenGL smooth\n");
	printf("4. Animation: Single object, Solar system, Stop\n");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("3D Graphics with Lighting and Animation");
	init();
	menu();
	glutDisplayFunc(display);
	glutMotionFunc(mouseMotionFcn);
	glutMouseFunc(mouseActionFcn);
	glutReshapeFunc(winReshapeFcn);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	
	// Cleanup
	if (myLight != NULL) {
		delete myLight;
	}
	
	return 0;
}