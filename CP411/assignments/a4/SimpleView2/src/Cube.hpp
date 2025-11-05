/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added face colors and normals
 */

#ifndef CCUBE_H
#define CCUBE_H

#include <GL/glut.h>
#include "Shape.hpp"
#include "Vector.hpp"
#include "Light.hpp"

class Cube: public Shape {
protected:
	GLfloat vertex[8][3];      /* 2D array to store cube vertices */
	GLint face[6][4];          /* 2D array to store faces */
	GLfloat faceColors[6][3];  /* Colors for each face */
	GLfloat faceNormals[6][3]; /* Normal vectors for each face */
	GLfloat r, g, b;           /* legacy color cube */
	
	void computeFaceNormals();
	bool isBackFace(int faceIndex, GLfloat* modelview);
	void drawWireframeFace(int i);
	GLfloat* getFaceColor(int faceIndex);
	GLfloat* getFaceNormal(int faceIndex);
	
public:
	Cube();
	void draw();
	void drawFace(int);
};

#endif