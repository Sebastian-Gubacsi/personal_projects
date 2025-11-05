/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include "Matrix.hpp"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

Matrix::Matrix() {
	loadIdentity();
}

// mat <- identity matrix
void Matrix::loadIdentity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				mat[i][j] = 1;
			else
				mat[i][j] = 0;
		}
	}
}

// multiplication  mat <- m * mat
void Matrix::matrixPreMultiply(Matrix* m) {
	GLfloat result[4][4];
	
	// Compute m * this->mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result[i][j] += m->mat[i][k] * mat[k][j];
			}
		}
	}
	
	// Copy result back to mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = result[i][j];
		}
	}
}

// transpose  mat <- mat'
void Matrix::transpose() {
	GLfloat temp;
	for (int i = 0; i < 4; i++) {
		for (int j = i + 1; j < 4; j++) {
			temp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = temp;
		}
	}
}

// normalize MC - make the first three columns unit vectors and orthogonal
void Matrix::normalize() {
	// Normalize the first three columns (X, Y, Z axes)
	for (int col = 0; col < 3; col++) {
		GLfloat length = 0.0;
		
		// Calculate length of column vector
		for (int row = 0; row < 3; row++) {
			length += mat[row][col] * mat[row][col];
		}
		length = sqrt(length);
		
		// Normalize column
		if (length > 0.0001) {
			for (int row = 0; row < 3; row++) {
				mat[row][col] /= length;
			}
		}
	}
	
	// Gram-Schmidt orthogonalization to ensure orthogonality
	// X axis stays as is (already normalized)
	
	// Y axis: make it perpendicular to X
	GLfloat dotXY = 0.0;
	for (int i = 0; i < 3; i++) {
		dotXY += mat[i][0] * mat[i][1];
	}
	for (int i = 0; i < 3; i++) {
		mat[i][1] -= dotXY * mat[i][0];
	}
	
	// Normalize Y axis again
	GLfloat lengthY = 0.0;
	for (int i = 0; i < 3; i++) {
		lengthY += mat[i][1] * mat[i][1];
	}
	lengthY = sqrt(lengthY);
	if (lengthY > 0.0001) {
		for (int i = 0; i < 3; i++) {
			mat[i][1] /= lengthY;
		}
	}
	
	// Z axis: make it perpendicular to both X and Y using cross product
	mat[0][2] = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];
	mat[1][2] = mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1];
	mat[2][2] = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
	
	// Normalize Z axis
	GLfloat lengthZ = 0.0;
	for (int i = 0; i < 3; i++) {
		lengthZ += mat[i][2] * mat[i][2];
	}
	lengthZ = sqrt(lengthZ);
	if (lengthZ > 0.0001) {
		for (int i = 0; i < 3; i++) {
			mat[i][2] /= lengthZ;
		}
	}
}

// v  <- mat * v
void Matrix::multiplyVector(GLfloat* v) {
	GLfloat result[4];
	
	for (int i = 0; i < 4; i++) {
		result[i] = 0;
		for (int j = 0; j < 4; j++) {
			result[i] += mat[i][j] * v[j];
		}
	}
	
	// Copy result back to v
	for (int i = 0; i < 4; i++) {
		v[i] = result[i];
	}
}

void Matrix::rotateMatrix(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	// Create rotation matrix using Rodrigues' rotation formula
	// R = I + sin(θ)K + (1-cos(θ))K²
	// where K is the skew-symmetric matrix of the rotation axis
	
	// Normalize the rotation axis
	GLfloat length = sqrt(rx*rx + ry*ry + rz*rz);
	if (length < 0.0001) {
		loadIdentity();
		return;
	}
	
	rx /= length;
	ry /= length;
	rz /= length;
	
	// Convert angle to radians
	GLfloat theta = angle * M_PI / 180.0;
	GLfloat cosTheta = cos(theta);
	GLfloat sinTheta = sin(theta);
	GLfloat oneMinusCos = 1.0 - cosTheta;
	
	// Build rotation matrix
	mat[0][0] = cosTheta + rx*rx*oneMinusCos;
	mat[0][1] = rx*ry*oneMinusCos - rz*sinTheta;
	mat[0][2] = rx*rz*oneMinusCos + ry*sinTheta;
	mat[0][3] = 0.0;
	
	mat[1][0] = ry*rx*oneMinusCos + rz*sinTheta;
	mat[1][1] = cosTheta + ry*ry*oneMinusCos;
	mat[1][2] = ry*rz*oneMinusCos - rx*sinTheta;
	mat[1][3] = 0.0;
	
	mat[2][0] = rz*rx*oneMinusCos - ry*sinTheta;
	mat[2][1] = rz*ry*oneMinusCos + rx*sinTheta;
	mat[2][2] = cosTheta + rz*rz*oneMinusCos;
	mat[2][3] = 0.0;
	
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}