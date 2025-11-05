/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include "Pyramid.hpp"

Pyramid::Pyramid()
{
	// Set coordinate values for pyramid vertices
	// Base vertices (square base at z = -0.5)
	vertex[0][0] = -0.5; vertex[0][1] = -0.5; vertex[0][2] = -0.5;
	vertex[1][0] =  0.5; vertex[1][1] = -0.5; vertex[1][2] = -0.5;
	vertex[2][0] =  0.5; vertex[2][1] =  0.5; vertex[2][2] = -0.5;
	vertex[3][0] = -0.5; vertex[3][1] =  0.5; vertex[3][2] = -0.5;
	
	// Apex vertex (top at z = 0.5)
	vertex[4][0] =  0.0; vertex[4][1] =  0.0; vertex[4][2] = 0.5;
	
	// Define triangular faces (each face has 3 vertices)
	// Front face
	face[0][0] = 0; face[0][1] = 1; face[0][2] = 4;
	// Right face
	face[1][0] = 1; face[1][1] = 2; face[1][2] = 4;
	// Back face
	face[2][0] = 2; face[2][1] = 3; face[2][2] = 4;
	// Left face
	face[3][0] = 3; face[3][1] = 0; face[3][2] = 4;
	// Base is handled separately (square)
	
	// Set color to yellow
	r = 1.0;
	g = 1.0;
	b = 0.0;
}

void Pyramid::drawFace(int i)
{
	// Draw triangular face i
	glBegin(GL_TRIANGLES);
	for (int j = 0; j < 3; j++) {
		int vertexIndex = face[i][j];
		glVertex3fv(vertex[vertexIndex]);
	}
	glEnd();
}

void Pyramid::drawBase()
{
	// Draw square base
	glBegin(GL_QUADS);
	glVertex3fv(vertex[0]);
	glVertex3fv(vertex[3]);
	glVertex3fv(vertex[2]);
	glVertex3fv(vertex[1]);
	glEnd();
}

void Pyramid::draw()
{
	glPushMatrix();
	this->ctmMultiply();
	glScalef(s, s, s);
	
	// Set color
	glColor3f(r, g, b);
	
	// Draw all triangular faces
	for (int i = 0; i < 4; i++) {
		drawFace(i);
	}
	
	// Draw base
	drawBase();
	
	glPopMatrix();
}