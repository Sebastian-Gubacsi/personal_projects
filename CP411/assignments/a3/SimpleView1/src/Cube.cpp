/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include "Cube.hpp"

Cube::Cube()
{
	// Set coordinate values for all vertices of a unit cube centered at origin
	// Bottom face (z = -0.5)
	vertex[0][0] = -0.5; vertex[0][1] = -0.5; vertex[0][2] = -0.5;
	vertex[1][0] =  0.5; vertex[1][1] = -0.5; vertex[1][2] = -0.5;
	vertex[2][0] =  0.5; vertex[2][1] =  0.5; vertex[2][2] = -0.5;
	vertex[3][0] = -0.5; vertex[3][1] =  0.5; vertex[3][2] = -0.5;
	
	// Top face (z = 0.5)
	vertex[4][0] = -0.5; vertex[4][1] = -0.5; vertex[4][2] = 0.5;
	vertex[5][0] =  0.5; vertex[5][1] = -0.5; vertex[5][2] = 0.5;
	vertex[6][0] =  0.5; vertex[6][1] =  0.5; vertex[6][2] = 0.5;
	vertex[7][0] = -0.5; vertex[7][1] =  0.5; vertex[7][2] = 0.5;
	
	// Define faces (each face has 4 vertices in counter-clockwise order)
	// Bottom face
	face[0][0] = 0; face[0][1] = 3; face[0][2] = 2; face[0][3] = 1;
	// Top face
	face[1][0] = 4; face[1][1] = 5; face[1][2] = 6; face[1][3] = 7;
	// Front face
	face[2][0] = 0; face[2][1] = 1; face[2][2] = 5; face[2][3] = 4;
	// Right face
	face[3][0] = 1; face[3][1] = 2; face[3][2] = 6; face[3][3] = 5;
	// Back face
	face[4][0] = 2; face[4][1] = 3; face[4][2] = 7; face[4][3] = 6;
	// Left face
	face[5][0] = 3; face[5][1] = 0; face[5][2] = 4; face[5][3] = 7;
	
	// Set color to red
	r = 1.0;
	g = 0.0;
	b = 0.0;
}

void Cube::drawFace(int i)
{
	// Draw face i as wireframe using GL_LINE_LOOP
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 4; j++) {
		int vertexIndex = face[i][j];
		glVertex3fv(vertex[vertexIndex]);
	}
	glEnd();
}

void Cube::draw()
{
	glPushMatrix();
	this->ctmMultiply();
	glScalef(s, s, s);
	
	// Set color
	glColor3f(r, g, b);
	
	// Draw all faces
	for (int i = 0; i < 6; i++) {
		drawFace(i);
	}
	
	glPopMatrix();
}