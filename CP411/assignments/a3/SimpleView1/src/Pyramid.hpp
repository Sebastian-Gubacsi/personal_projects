/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#ifndef CPYRAMID_H
#define CPYRAMID_H

#include <GL/glut.h>
#include "Shape.hpp"
#include "Vector.hpp"

class Pyramid: public Shape {
protected:
	GLfloat vertex[5][3];  /* 2D array to store pyramid vertices */
	GLint face[4][3];      /* 2D array to store triangular faces */
	GLfloat r, g, b;       /* color pyramid */
public:
	Pyramid();
	void draw();
	void drawFace(int);
	void drawBase();
};

#endif