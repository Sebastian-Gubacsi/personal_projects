/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added Light class for lighting system
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <GL/glut.h>
#include "Point.hpp"

enum LightType {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    AMBIENT_LIGHT
};

class Light {
private:
    LightType type;
    Point position;
    GLfloat intensity;
    GLfloat r, g, b;  // Light color
    
public:
    Light();
    Light(LightType t, Point pos, GLfloat intens);
    
    void setType(LightType t);
    void setPosition(Point pos);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setIntensity(GLfloat intens);
    void setColor(GLfloat red, GLfloat green, GLfloat blue);
    
    LightType getType() const;
    Point getPosition() const;
    GLfloat getIntensity() const;
    
    void translate(GLfloat dx, GLfloat dy, GLfloat dz);
    void rotate(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle);
    void changeIntensity(GLfloat delta);
    
    void draw();  // Draw light as a sphere
    void applyOpenGLLight(GLenum lightNum);  // Setup OpenGL lighting
};

#endif