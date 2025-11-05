/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added culling, lighting, shading, and animation menus
 */

#include "Menu.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Light.hpp"

extern GLint csType;
extern Shape* selectObj;
extern GLint transType, xbegin;
extern World myWorld;
extern Camera myCamera;
extern Light* myLight;
extern bool lightOn;
extern int cullingMode;
extern int shadingMode;
extern int animationMode;

void animationUpdate(int value);

void menu() {
	GLint Object_Menu = glutCreateMenu(ObjSubMenu);
	glutAddMenuEntry("Cube", 1);
	glutAddMenuEntry("Pyramid", 2);
	glutAddMenuEntry("House", 3);

	GLint MCTrans_Menu = glutCreateMenu(MCSTransMenu);
	glutAddMenuEntry("Rotate x", 1);
	glutAddMenuEntry("Rotate y", 2);
	glutAddMenuEntry("Rotate z", 3);
	glutAddMenuEntry("Scale", 4);

	GLint WCTrans_Menu = glutCreateMenu(WCSTransMenu);
	glutAddMenuEntry("Rotate x", 1);
	glutAddMenuEntry("Rotate y", 2);
	glutAddMenuEntry("Rotate z", 3);
	glutAddMenuEntry("Translate x", 4);
	glutAddMenuEntry("Translate y", 5);
	glutAddMenuEntry("Translate z", 6);

	GLint VCTrans_Menu = glutCreateMenu(VCSTransMenu);
	glutAddMenuEntry("Rotate x", 1);
	glutAddMenuEntry("Rotate y", 2);
	glutAddMenuEntry("Rotate z", 3);
	glutAddMenuEntry("Translate x", 4);
	glutAddMenuEntry("Translate y", 5);
	glutAddMenuEntry("Translate z", 6);
	glutAddMenuEntry("Clipping Near", 7);
	glutAddMenuEntry("Clipping Far", 8);
	
	// Culling menu
	GLint Culling_Menu = glutCreateMenu(CullingMenu);
	glutAddMenuEntry("No culling", 1);
	glutAddMenuEntry("My back-face", 2);
	glutAddMenuEntry("OpenGL back-face & depth buffer", 3);
	
	// Lighting menu
	GLint Lighting_Menu = glutCreateMenu(LightingMenu);
	glutAddMenuEntry("Turn on light", 1);
	glutAddMenuEntry("Turn off light", 2);
	glutAddMenuEntry("Change intensity", 3);
	glutAddMenuEntry("Rotate x", 4);
	glutAddMenuEntry("Rotate y", 5);
	glutAddMenuEntry("Rotate z", 6);
	glutAddMenuEntry("Translate x", 7);
	glutAddMenuEntry("Translate y", 8);
	glutAddMenuEntry("Translate z", 9);
	
	// Shading menu
	GLint Shading_Menu = glutCreateMenu(ShadingMenu);
	glutAddMenuEntry("No shading", 1);
	glutAddMenuEntry("My constant shading", 2);
	glutAddMenuEntry("OpenGL flat shading", 3);
	glutAddMenuEntry("OpenGL smooth shading", 4);
	
	// Animation menu
	GLint Animation_Menu = glutCreateMenu(AnimationMenu);
	glutAddMenuEntry("Single object", 1);
	glutAddMenuEntry("Multiple objects (Solar system)", 2);
	glutAddMenuEntry("Stop animation", 3);

	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Reset", 1);
	glutAddSubMenu("Select Object", Object_Menu);
	glutAddSubMenu("MCS Transformations", MCTrans_Menu);
	glutAddSubMenu("WCS Transformations", WCTrans_Menu);
	glutAddSubMenu("VCS Transformations", VCTrans_Menu);
	glutAddSubMenu("Culling", Culling_Menu);
	glutAddSubMenu("Lighting", Lighting_Menu);
	glutAddSubMenu("Shading", Shading_Menu);
	glutAddSubMenu("Animation", Animation_Menu);
	glutAddMenuEntry("Quit", 2);
}

void mainMenu(GLint option) {
	switch (option){
		case 1:
			myWorld.reset();
			myCamera.reset();
			animationMode = 0;
			break;
		case 2:
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void ObjSubMenu(GLint objectOption)
{
	selectObj = myWorld.searchById(objectOption);
	printf("Selected object: ID = %d\n", objectOption);
	glutPostRedisplay();
}

void MCSTransMenu(GLint transOption) {
	csType = 1;
	transType = transOption;
	glutPostRedisplay();
}

void WCSTransMenu(GLint transOption) {
	csType = 2;
	transType = transOption;
	glutPostRedisplay();
}

void VCSTransMenu(GLint transOption) {
	csType = 3;
	transType = transOption;
	glutPostRedisplay();
}

void CullingMenu(GLint option) {
	switch(option) {
		case 1:
			cullingMode = 0; // No culling
			printf("Culling: No culling\n");
			break;
		case 2:
			cullingMode = 2; // My back-face
			printf("Culling: My back-face algorithm\n");
			break;
		case 3:
			cullingMode = 1; // OpenGL
			printf("Culling: OpenGL back-face & depth buffer\n");
			break;
	}
	glutPostRedisplay();
}

void LightingMenu(GLint option) {
	csType = 4;
	switch(option) {
		case 1:
			lightOn = true;
			printf("Light: ON\n");
			break;
		case 2:
			lightOn = false;
			printf("Light: OFF\n");
			break;
		case 3:
			transType = 10; // Change intensity
			printf("Light: Change intensity mode (drag mouse)\n");
			break;
		case 4:
			transType = 1; // Rotate x
			printf("Light: Rotate X mode (drag mouse)\n");
			break;
		case 5:
			transType = 2; // Rotate y
			printf("Light: Rotate Y mode (drag mouse)\n");
			break;
		case 6:
			transType = 3; // Rotate z
			printf("Light: Rotate Z mode (drag mouse)\n");
			break;
		case 7:
			transType = 4; // Translate x
			printf("Light: Translate X mode (drag mouse)\n");
			break;
		case 8:
			transType = 5; // Translate y
			printf("Light: Translate Y mode (drag mouse)\n");
			break;
		case 9:
			transType = 6; // Translate z
			printf("Light: Translate Z mode (drag mouse)\n");
			break;
	}
	glutPostRedisplay();
}

void ShadingMenu(GLint option) {
	switch(option) {
		case 1:
			shadingMode = 0; // Wireframe
			printf("Shading: No shading (wireframe)\n");
			break;
		case 2:
			shadingMode = 1; // My constant shading
			printf("Shading: My constant shading\n");
			break;
		case 3:
			shadingMode = 2; // OpenGL flat
			printf("Shading: OpenGL flat shading\n");
			break;
		case 4:
			shadingMode = 3; // OpenGL smooth
			printf("Shading: OpenGL smooth shading\n");
			break;
	}
	glutPostRedisplay();
}

void AnimationMenu(GLint option) {
	switch(option) {
		case 1:
			animationMode = 1;
			printf("Animation: Single object rotating\n");
			glutTimerFunc(16, animationUpdate, 0);
			break;
		case 2:
			// Reset objects for solar system
			myWorld.reset();
			Shape* house = myWorld.searchById(3);
			Shape* cube = myWorld.searchById(1);
			Shape* pyramid = myWorld.searchById(2);
			if (house) house->translate(0, 0, 0);
			if (cube) cube->translate(2.0, 0, 0);
			if (pyramid) pyramid->translate(2.5, 0, 0);
			
			animationMode = 2;
			printf("Animation: Solar system (House=Sun, Cube=Earth, Pyramid=Moon)\n");
			glutTimerFunc(16, animationUpdate, 0);
			break;
		case 3:
			animationMode = 0;
			printf("Animation: STOPPED\n");
			break;
	}
	glutPostRedisplay();
}

void MCSTransform(GLint x){
	if (selectObj == NULL) return;
	
	GLfloat x0, y0, z0, rx, ry, rz, theta;
	theta = (xbegin - x > 0) ? 1 : -1;

	x0 = selectObj->getMC().mat[0][3];
	y0 = selectObj->getMC().mat[1][3];
	z0 = selectObj->getMC().mat[2][3];

	if (transType == 1) { //model rotate x
		rx = selectObj->getMC().mat[0][0];
		ry = selectObj->getMC().mat[1][0];
		rz = selectObj->getMC().mat[2][0];
		selectObj->rotate(x0, y0, z0, rx, ry, rz, theta * 0.5);
	}
	else if (transType == 2) { //model rotate y
		rx = selectObj->getMC().mat[0][1];
		ry = selectObj->getMC().mat[1][1];
		rz = selectObj->getMC().mat[2][1];
		selectObj->rotate(x0, y0, z0, rx, ry, rz, theta * 0.5);
	}else if (transType == 3){  //model rotate z
		rx = selectObj->getMC().mat[0][2];
		ry = selectObj->getMC().mat[1][2];
		rz = selectObj->getMC().mat[2][2];
		selectObj->rotate(x0, y0, z0, rx, ry, rz, theta * 0.5);
	}
	else if (transType == 4) { //model scale
		selectObj->scaleChange(theta * 0.02);
	}
}

void WCSTransform(GLint x){
	if (selectObj == NULL) return;
	
	GLfloat theta = (xbegin - x > 0) ? 1 : -1;
	if (transType == 1) {
		selectObj->rotateOrigin(0, 0, 0, 1, 0, 0, theta * 0.5);
	}
	else if (transType == 2) {
		selectObj->rotateOrigin(0, 0, 0, 0, 1, 0, theta * 0.5);
	}
	else if(transType == 3){
		selectObj->rotateOrigin(0, 0, 0, 0, 0, 1, theta * 0.5);
	}
	else if (transType == 4) {
		selectObj->translate(theta * 0.02, 0, 0);
	}
	else if(transType == 5){ //WC translate y
		selectObj->translate(0, theta * 0.02, 0);
	}
	else if(transType == 6){ //WC translate z
		selectObj->translate(0, 0, theta * 0.02);
	}
}

void VCSTransform(GLint x){
	GLfloat theta = (xbegin - x > 0) ? 1 : -1;
	if (transType == 1) {  //eye rotate x
		myCamera.rotate(1.0, 0.0, 0.0, theta*0.5);
	}
	else if (transType == 2) { // eye rotate y
		myCamera.rotate(0.0, 1.0, 0.0, theta*0.5);
	}
	else if(transType == 3){ //eye rotate z
		myCamera.rotate(0.0, 0.0, 1.0, theta*0.5);
	}
	else if (transType == 4) { //eye translate x
		myCamera.translate(theta* 0.02, 0.0, 0.0);
	}
	else if(transType == 5){   // eye translate y
		myCamera.translate(0.0, theta* 0.02, 0.0);
	}
	else if(transType == 6){ // eye translate z
		myCamera.translate(0.0, 0.0, theta* 0.02);
	}
	else if(transType == 7){  // change dnear
		myCamera.dnear += theta * .1;
		if (myCamera.dnear < 0.1) myCamera.dnear = 0.1;
	}
	else if(transType == 8){  // change dfar
		myCamera.dfar += theta *.1;
		if (myCamera.dfar < myCamera.dnear + 0.1) myCamera.dfar = myCamera.dnear + 0.1;
	}
}

void LightTransform(GLint x){
	if (myLight == NULL) return;
	
	GLfloat theta = (xbegin - x > 0) ? 1 : -1;
	
	if (transType == 1) {  // rotate x
		myLight->rotate(1.0, 0.0, 0.0, theta * 0.5);
	}
	else if (transType == 2) {  // rotate y
		myLight->rotate(0.0, 1.0, 0.0, theta * 0.5);
	}
	else if (transType == 3) {  // rotate z
		myLight->rotate(0.0, 0.0, 1.0, theta * 0.5);
	}
	else if (transType == 4) {  // translate x
		myLight->translate(theta * 0.05, 0.0, 0.0);
	}
	else if (transType == 5) {  // translate y
		myLight->translate(0.0, theta * 0.05, 0.0);
	}
	else if (transType == 6) {  // translate z
		myLight->translate(0.0, 0.0, theta * 0.05);
	}
	else if (transType == 10) {  // change intensity
		myLight->changeIntensity(theta * 0.01);
		printf("Light intensity: %.2f\n", myLight->getIntensity());
	}
}