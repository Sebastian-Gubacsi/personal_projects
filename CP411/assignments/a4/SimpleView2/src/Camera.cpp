/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#include <stdio.h>
#include "Camera.hpp"
#include "Matrix.hpp"
#include <math.h>

bool DEBUG = false;

Camera::Camera(){
	eye.set(5.0, 5.0, 10.0);
	ref.set(0, 0, 0);
	viewup.set(0, 0, 1);
	aspect = 1.0, vangle = 40.0, dnear = 1.0, dfar = 15.0;
	setViewNorm();
}

void Camera::reset(void) { // make default camera
	eye.set(5.0, 5.0, 10.0);
	ref.set(0, 0, 0);
	viewup.set(0, 0, 1);
	aspect = 1.0, vangle = 40.0, dnear = 1.0, dfar = 15.0;
	setViewNorm();
}

void Camera::set(Point Eye, Point Look, Vector Up) {
	eye.set(Eye);
	ref.set(Look);
	viewup.set(Up);
	aspect = 1.0, vangle = 40.0, dnear = 1.0, dfar = 15.0;
	setViewNorm();
}

void Camera::setViewNorm() {
	GLfloat x, y, z, sr;
	x = ref.x - eye.x;
	y = ref.y - eye.y;
	z = ref.z - eye.z;
	sr = sqrt(x*x + y*y + z*z);
	x = x/sr;
	y = y/sr;
	z = z/sr;
	viewNorm.set(x, y, z);
}

void Camera::lookAt(Point target) {
	// Calculate distance from eye to current reference point
	GLfloat dx = ref.x - eye.x;
	GLfloat dy = ref.y - eye.y;
	GLfloat dz = ref.z - eye.z;
	GLfloat currentDist = sqrt(dx*dx + dy*dy + dz*dz);
	
	// Set reference point to target
	ref.set(target);
	
	// Calculate direction from target to eye
	GLfloat dirX = eye.x - target.x;
	GLfloat dirY = eye.y - target.y;
	GLfloat dirZ = eye.z - target.z;
	GLfloat dist = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
	
	// If eye is too close or at the target, move it back
	if (dist < 0.1) {
		// Place eye at a good viewing distance from target
		eye.x = target.x + 5.0;
		eye.y = target.y + 5.0;
		eye.z = target.z + 5.0;
	} else {
		// Maintain approximately the same viewing distance
		GLfloat desiredDist = (currentDist > 3.0) ? currentDist : 8.0;
		GLfloat scale = desiredDist / dist;
		eye.x = target.x + dirX * scale;
		eye.y = target.y + dirY * scale;
		eye.z = target.z + dirZ * scale;
	}
	
	setViewNorm();
}

void Camera::rotate(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle){
	// Rotate eye position around the reference point in WCS
	// The axis of rotation passes through 'ref' with direction (rx, ry, rz)
	
	// Create rotation matrix
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	
	// Calculate eye position relative to reference point
	GLfloat v[4];
	v[0] = eye.x - ref.x;
	v[1] = eye.y - ref.y;
	v[2] = eye.z - ref.z;
	v[3] = 1.0;
	
	// Rotate the relative position
	m.multiplyVector(v);
	
	// Update eye position (add back the reference point)
	eye.x = v[0] + ref.x;
	eye.y = v[1] + ref.y;
	eye.z = v[2] + ref.z;
	
	// Also rotate the view up vector (it's a direction, not a position)
	GLfloat up[4];
	up[0] = viewup.x;
	up[1] = viewup.y;
	up[2] = viewup.z;
	up[3] = 0.0;  // Direction vector, not point
	
	m.multiplyVector(up);
	
	viewup.x = up[0];
	viewup.y = up[1];
	viewup.z = up[2];
	
	// Update view normal
	setViewNorm();
}

void Camera::translate(GLfloat tx, GLfloat ty, GLfloat tz) {
	// Translate eye position in WCS
	eye.x += tx;
	eye.y += ty;
	eye.z += tz;
	
	// Also translate the reference point to maintain the view direction
	ref.x += tx;
	ref.y += ty;
	ref.z += tz;
	
	setViewNorm();
}

void Camera::setProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vangle, aspect, dnear, dfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, ref.x, ref.y, ref.z, viewup.x, viewup.y, viewup.z);
}