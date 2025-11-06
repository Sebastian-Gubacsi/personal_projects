/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added face colors and normals
 */

#ifndef CPYRAMID_H
#define CPYRAMID_H

#include <GL/glut.h>
#include "Shape.hpp"
#include "Vector.hpp"
#include "Point.hpp"
#include "Light.hpp"

class Pyramid: public Shape {
protected:
	GLfloat vertex[5][3];      /* 2D array to store pyramid vertices */
	GLint face[4][3];          /* 2D array to store triangular faces */
	GLfloat faceColors[4][3];  /* Colors for each face */
	GLfloat faceNormals[4][3]; /* Normal vectors for each face */
	GLfloat baseColor[3];      /* Color for base */
	GLfloat baseNormal[3];     /* Normal for base */
	GLfloat objectColor[3];    /* Single solid color for flat shading */
	GLfloat r, g, b;           /* legacy color pyramid */
	
	void computeFaceNormals();
	bool isBackFace(int faceIndex, GLfloat* modelview);
	void drawWireframeFace(int i);
	GLfloat* getFaceColor(int faceIndex);
	GLfloat* getFaceNormal(int faceIndex);
	
public:
	Pyramid();
	void draw();
	void drawFace(int);
	void drawBase();
	void drawGeometry();  // Made public so House can use it
	// Draw pyramid but average seam normals with a neighbor cube to smooth across the roof-body seam
	void drawWithCubeSeamNormals(class Cube* neighbor);
};

#endif