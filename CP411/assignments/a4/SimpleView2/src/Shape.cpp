/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include "Shape.hpp"
#include <stdio.h>

using namespace std;

Shape::Shape() {
	reset();
}

Shape::~Shape() { };

Matrix Shape::getMC() {
	return  mc;
};

void Shape::reset(){
	mc.loadIdentity();
	s = 1;
}

void Shape::translate(GLfloat tx, GLfloat ty, GLfloat tz) {
	mc.mat[0][3] += tx;
	mc.mat[1][3] += ty;
	mc.mat[2][3] += tz;
	mc.mat[3][3] = 1;
}

void Shape::rotate(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	mc.matrixPreMultiply(&m);
	// After rotation, normalize to prevent accumulation of numerical errors
	mc.normalize();
}

void Shape::rotate(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	// Rotate MC (both axes and origin) with respect to axis passing through (x0, y0, z0) 
	// with direction (rx, ry, rz) in WCS
	
	// Create rotation matrix
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	
	// Step 1: Translate origin to rotation point
	GLfloat tx = mc.mat[0][3] - x0;
	GLfloat ty = mc.mat[1][3] - y0;
	GLfloat tz = mc.mat[2][3] - z0;
	
	// Step 2: Rotate the position
	GLfloat v[4];
	v[0] = tx;
	v[1] = ty;
	v[2] = tz;
	v[3] = 1.0;
	m.multiplyVector(v);
	
	// Step 3: Update position
	mc.mat[0][3] = v[0] + x0;
	mc.mat[1][3] = v[1] + y0;
	mc.mat[2][3] = v[2] + z0;
	
	// Step 4: Also rotate the MC axes (orientation)
	// Rotate each axis vector (columns 0, 1, 2 of mc)
	for (int col = 0; col < 3; col++) {
		GLfloat axis[4];
		axis[0] = mc.mat[0][col];
		axis[1] = mc.mat[1][col];
		axis[2] = mc.mat[2][col];
		axis[3] = 0.0;  // It's a direction vector, not a point
		
		m.multiplyVector(axis);
		
		mc.mat[0][col] = axis[0];
		mc.mat[1][col] = axis[1];
		mc.mat[2][col] = axis[2];
	}
}

void Shape::rotateOrigin(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	// Rotate the origin of MCS with respect to axis (x0, y0, z0) + u(rx, ry, rz) in WCS
	// This rotates only the position of the MC origin, not the axes
	
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	
	GLfloat v[4];
	v[0] = mc.mat[0][3] - x0;
	v[1] = mc.mat[1][3] - y0;
	v[2] = mc.mat[2][3] - z0;
	v[3] = mc.mat[3][3];
	
	m.multiplyVector(v);
	
	mc.mat[0][3] = v[0] + x0;
	mc.mat[1][3] = v[1] + y0;
	mc.mat[2][3] = v[2] + z0;
	mc.mat[3][3] = v[3];
}

void Shape::setScale(GLfloat x) {
	s = x;
}

void Shape::scaleChange(GLfloat x) {
	s += x;
}

void Shape::setId(GLint i) {
	id = i;
}

GLint Shape::getId() {
	return id;
}

void Shape::ctmMultiply() {
	GLfloat M[16];
	M[0] = mc.mat[0][0];
	M[1] = mc.mat[1][0];
	M[2] = mc.mat[2][0];
	M[3] = 0;
	M[4] = mc.mat[0][1];
	M[5] = mc.mat[1][1];
	M[6] = mc.mat[2][1];
	M[7] = 0;
	M[8] = mc.mat[0][2];
	M[9] = mc.mat[1][2];
	M[10] = mc.mat[2][2];
	M[11] = 0;
	M[12] = mc.mat[0][3];
	M[13] = mc.mat[1][3];
	M[14] = mc.mat[2][3];
	M[15] = 1;
	glMultMatrixf(M);
}