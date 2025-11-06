/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Fixed to properly use component geometry
 */

#include "House.hpp"

// Use the same global shading mode variable used across other shapes
extern int shadingMode;

House::House()
{
	// Create cube for the house body
	cube = new Cube();
	
	// Create pyramid for the roof
	pyramid = new Pyramid();
}

House::~House()
{
	delete cube;
	delete pyramid;
}

void House::draw()
{
	glPushMatrix();
	
	// Apply the house's transformation
	this->ctmMultiply();
	glScalef(s, s, s);
	
	// Set material properties for flat shading mode
	if (shadingMode == 2) {  // Flat shading mode
		GLfloat mat_ambient[] = {0.2f * flatColor[0], 0.2f * flatColor[1], 0.2f * flatColor[2], 1.0f};
		GLfloat mat_diffuse[] = {flatColor[0], flatColor[1], flatColor[2], 1.0f};
		GLfloat mat_specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
		GLfloat mat_shininess[] = {25.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	}

	// Draw cube body at origin
	cube->drawGeometry();
	
	// Draw pyramid roof translated up by 1.0
	glPushMatrix();
	glTranslatef(0, 0, 1.0);
	// Draw pyramid roof and smooth seam normals with the cube body
	pyramid->drawWithCubeSeamNormals(cube);
	glPopMatrix();
	
	glPopMatrix();
}