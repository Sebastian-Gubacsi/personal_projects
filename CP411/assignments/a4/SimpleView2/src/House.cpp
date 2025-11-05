/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Fixed to properly use component geometry
 */

#include "House.hpp"

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
	
	// Draw cube body at origin
	cube->drawGeometry();
	
	// Draw pyramid roof translated up by 1.0
	glPushMatrix();
	glTranslatef(0, 0, 1.0);
	pyramid->drawGeometry();
	glPopMatrix();
	
	glPopMatrix();
}