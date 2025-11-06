/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added Light class implementation
 */

#include "Light.hpp"
#include "Matrix.hpp"
#include <math.h>

Light::Light()
{
	type = POINT_LIGHT;
	position.set(0.0, 0.0, 5.0);
	intensity = 4.0; // brighter default
	r = 1.0;
	g = 1.0;
	b = 1.0;
}

Light::Light(LightType t, Point pos, GLfloat intens)
{
	type = t;
	position.set(pos);
	intensity = intens;
	r = 1.0;
	g = 1.0;
	b = 1.0;
}

void Light::setType(LightType t)
{
	type = t;
}

void Light::setPosition(Point pos)
{
	position.set(pos);
}

void Light::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.set(x, y, z);
}

void Light::setIntensity(GLfloat intens)
{
	intensity = intens;
	if (intensity < 0.0) intensity = 0.0;
	if (intensity > 8.0) intensity = 8.0; // allow much brighter lights
}

void Light::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
	r = red;
	g = green;
	b = blue;
}

LightType Light::getType() const
{
	return type;
}

Point Light::getPosition() const
{
	return position;
}

GLfloat Light::getIntensity() const
{
	return intensity;
}

void Light::translate(GLfloat dx, GLfloat dy, GLfloat dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;
}

void Light::rotate(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle)
{
	// Rotate light position around origin
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	
	GLfloat v[4];
	v[0] = position.x;
	v[1] = position.y;
	v[2] = position.z;
	v[3] = 1.0;
	
	m.multiplyVector(v);
	
	position.x = v[0];
	position.y = v[1];
	position.z = v[2];
}

void Light::changeIntensity(GLfloat delta)
{
	intensity += delta;
	if (intensity < 0.0) intensity = 0.0;
	if (intensity > 2.0) intensity = 2.0;
}

void Light::draw()
{
	// Draw light as a yellow sphere
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.2, 16, 16);
	glPopMatrix();
}

void Light::applyOpenGLLight(GLenum lightNum)
{
	GLfloat light_position[] = {position.x, position.y, position.z, 1.0f};
	GLfloat light_ambient[] = {0.2f * r, 0.2f * g, 0.2f * b, 1.0f};
	GLfloat light_diffuse[] = {intensity * r, intensity * g, intensity * b, 1.0f};
	GLfloat light_specular[] = {intensity * r, intensity * g, intensity * b, 1.0f};
	
	glLightfv(lightNum, GL_POSITION, light_position);
	glLightfv(lightNum, GL_AMBIENT, light_ambient);
	glLightfv(lightNum, GL_DIFFUSE, light_diffuse);
	glLightfv(lightNum, GL_SPECULAR, light_specular);
}