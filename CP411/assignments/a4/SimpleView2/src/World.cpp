/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

 #include "World.hpp"
#include "Cube.hpp"
#include "Pyramid.hpp"
#include "House.hpp"

using namespace std;

World::World() {
	/* Add House into the world object list (in the middle) */
	Shape *obj = new House();
	obj->setId(3);
	objlist.push_back(obj);

	/* Add Cube into the world object list (right of house) */
	obj = new Cube();
	obj->setId(1);
	obj->translate(2.0, 0, 0);  // Position to the right of house
	objlist.push_back(obj);

	/* Add Pyramid into the world object list (right of cube) */
	obj = new Pyramid();
	obj->setId(2);
	obj->translate(3.5, 0, 0);  // Position to the right of cube
	objlist.push_back(obj);
}

World::~World(){
	Shape *obj;
	while (!objlist.empty()) {
		obj = objlist.front();
		objlist.pop_front();
		delete obj;  // Use delete instead of free
	}
}

void lineSegment(float x1, float y1, float z1, float x2, float y2, float z2) {
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
}

void World::draw() {
	/* draw WCS coordinate axis */
	glColor3f(1.0, 0.0, 0.0);
	lineSegment(-2, 0, 0, 4, 0, 0); /* x-axis in red */
 	glColor3f(0.0, 1.0, 0.0);
	lineSegment(0, -2, 0, 0, 4, 0); /* y-axis in green */
	glColor3f(0.0, 0.0, 1.0);
	lineSegment(0, 0, -2, 0, 0, 4); /* z-axis in blue */

	std::list<Shape*>::iterator it;
	for (it = objlist.begin(); it !=  objlist.end(); ++it) {
	  (*it)->draw();
    }
}

void World::reset(){
	std::list<Shape*>::iterator it;
	for (it = objlist.begin(); it !=  objlist.end(); ++it) {
		(*it)->reset();  // First reset the object to clear all transformations
	}

	// Re-apply initial positions
	Shape* house = searchById(3);
	Shape* cube = searchById(1);
	Shape* pyramid = searchById(2);

	if (house) house->translate(0, 0, 0);     // House in middle
	if (cube) cube->translate(2.0, 0, 0);     // Cube 2 units to right of house
	if (pyramid) pyramid->translate(3.5, 0, 0); // Pyramid 3.5 units to right of house
}

Shape* World::searchById(GLint i) {
	std::list<Shape*>::iterator it;
	for (it = objlist.begin(); it !=  objlist.end(); ++it) {
	  if ((*it)->getId() == i) return *it;
    }
	return NULL;
}