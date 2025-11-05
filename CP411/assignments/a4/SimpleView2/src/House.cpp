/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include "House.hpp"

House::House()
{
	// Create cube for the house body
	cube = new Cube();
	
	// Create pyramid for the roof
	pyramid = new Pyramid();
	
	// Position pyramid on top of cube
	// Cube is from -0.5 to 0.5 in z
	// Move pyramid up by 1.0 so its base sits on top of cube
	pyramid->translate(0, 0, 1.0);
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
	
	// Draw cube body
	glColor3f(1.0, 0.0, 0.0); // Red for body
	glPushMatrix();
	// Cube draws at its own position (no additional transform needed)
	for (int i = 0; i < 6; i++) {
		cube->drawFace(i);
	}
	glPopMatrix();
	
	// Draw pyramid roof
	glColor3f(1.0, 1.0, 0.0); // Yellow for roof
	glPushMatrix();
	// Apply pyramid's translation (it's positioned 1.0 unit up)
	glTranslatef(0, 0, 1.0);
	for (int i = 0; i < 4; i++) {
		pyramid->drawFace(i);
	}
	pyramid->drawBase();
	glPopMatrix();
	
	glPopMatrix();
}