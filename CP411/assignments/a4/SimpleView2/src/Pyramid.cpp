/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added face colors and shading support
 */

#include "Pyramid.hpp"
#include "Light.hpp"
#include <math.h>

extern int cullingMode;
extern int shadingMode;
extern Light* myLight;
extern bool lightOn;

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
	
	// Set face colors: orange, purple, pink, lime
	faceColors[0][0] = 1.0; faceColors[0][1] = 0.5; faceColors[0][2] = 0.0; // Orange
	faceColors[1][0] = 0.5; faceColors[1][1] = 0.0; faceColors[1][2] = 0.5; // Purple
	faceColors[2][0] = 1.0; faceColors[2][1] = 0.4; faceColors[2][2] = 0.7; // Pink
	faceColors[3][0] = 0.5; faceColors[3][1] = 1.0; faceColors[3][2] = 0.0; // Lime
	
	// Base color
	baseColor[0] = 0.7; baseColor[1] = 0.7; baseColor[2] = 0.7; // Gray
	
	// Compute face normals
	computeFaceNormals();
}

void Pyramid::computeFaceNormals()
{
	// Compute normal for each triangular face
	for (int i = 0; i < 4; i++) {
		// Get three vertices of the face
		GLfloat* v0 = vertex[face[i][0]];
		GLfloat* v1 = vertex[face[i][1]];
		GLfloat* v2 = vertex[face[i][2]];
		
		// Compute two edge vectors
		GLfloat e1[3] = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
		GLfloat e2[3] = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};
		
		// Cross product
		faceNormals[i][0] = e1[1] * e2[2] - e1[2] * e2[1];
		faceNormals[i][1] = e1[2] * e2[0] - e1[0] * e2[2];
		faceNormals[i][2] = e1[0] * e2[1] - e1[1] * e2[0];
		
		// Normalize
		GLfloat len = sqrt(faceNormals[i][0] * faceNormals[i][0] +
		                   faceNormals[i][1] * faceNormals[i][1] +
		                   faceNormals[i][2] * faceNormals[i][2]);
		if (len > 0.0001) {
			faceNormals[i][0] /= len;
			faceNormals[i][1] /= len;
			faceNormals[i][2] /= len;
		}
	}
	
	// Base normal (pointing down)
	baseNormal[0] = 0.0;
	baseNormal[1] = 0.0;
	baseNormal[2] = -1.0;
}

bool Pyramid::isBackFace(int faceIndex, GLfloat* modelview)
{
	// Get face center in world coordinates
	GLfloat center[3] = {0, 0, 0};
	for (int j = 0; j < 3; j++) {
		int vi = face[faceIndex][j];
		center[0] += vertex[vi][0];
		center[1] += vertex[vi][1];
		center[2] += vertex[vi][2];
	}
	center[0] /= 3.0;
	center[1] /= 3.0;
	center[2] /= 3.0;
	
	// Transform normal to world coordinates
	GLfloat worldNormal[3];
	worldNormal[0] = modelview[0] * faceNormals[faceIndex][0] +
	                 modelview[4] * faceNormals[faceIndex][1] +
	                 modelview[8] * faceNormals[faceIndex][2];
	worldNormal[1] = modelview[1] * faceNormals[faceIndex][0] +
	                 modelview[5] * faceNormals[faceIndex][1] +
	                 modelview[9] * faceNormals[faceIndex][2];
	worldNormal[2] = modelview[2] * faceNormals[faceIndex][0] +
	                 modelview[6] * faceNormals[faceIndex][1] +
	                 modelview[10] * faceNormals[faceIndex][2];
	
	// Transform center to world coordinates
	GLfloat worldCenter[3];
	worldCenter[0] = modelview[0] * center[0] + modelview[4] * center[1] +
	                 modelview[8] * center[2] + modelview[12];
	worldCenter[1] = modelview[1] * center[0] + modelview[5] * center[1] +
	                 modelview[9] * center[2] + modelview[13];
	worldCenter[2] = modelview[2] * center[0] + modelview[6] * center[1] +
	                 modelview[10] * center[2] + modelview[14];
	
	// Get camera position
	GLfloat camPos[3] = {-modelview[12], -modelview[13], -modelview[14]};
	
	// View vector from face center to camera
	GLfloat viewVec[3] = {
		camPos[0] - worldCenter[0],
		camPos[1] - worldCenter[1],
		camPos[2] - worldCenter[2]
	};
	
	// Dot product
	GLfloat dot = worldNormal[0] * viewVec[0] +
	              worldNormal[1] * viewVec[1] +
	              worldNormal[2] * viewVec[2];
	
	return dot < 0;
}

void Pyramid::drawFace(int i)
{
	glBegin(GL_TRIANGLES);
	glNormal3fv(faceNormals[i]);
	for (int j = 0; j < 3; j++) {
		int vertexIndex = face[i][j];
		glVertex3fv(vertex[vertexIndex]);
	}
	glEnd();
}

void Pyramid::drawWireframeFace(int i)
{
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 3; j++) {
		int vertexIndex = face[i][j];
		glVertex3fv(vertex[vertexIndex]);
	}
	glEnd();
}

void Pyramid::drawBase()
{
	if (shadingMode == 0) {
		// Wireframe
		glBegin(GL_LINE_LOOP);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[2]);
		glVertex3fv(vertex[1]);
		glEnd();
	} else {
		// Filled
		glBegin(GL_QUADS);
		glNormal3fv(baseNormal);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[2]);
		glVertex3fv(vertex[1]);
		glEnd();
	}
}

GLfloat* Pyramid::getFaceColor(int faceIndex)
{
	return faceColors[faceIndex];
}

GLfloat* Pyramid::getFaceNormal(int faceIndex)
{
	return faceNormals[faceIndex];
}

void Pyramid::drawGeometry()
{
	// Get modelview matrix for backface culling
	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	// Draw based on shading mode
	if (shadingMode == 0) {
		// Wireframe mode
		glColor3f(1.0, 1.0, 0.0);
		for (int i = 0; i < 4; i++) {
			drawWireframeFace(i);
		}
		drawBase();
	} else {
		// Filled faces with culling
		for (int i = 0; i < 4; i++) {
			// Check culling mode
			if (cullingMode == 2 && isBackFace(i, modelview)) {
				continue; // Skip back faces
			}
			
			// Set color based on shading mode
			if (shadingMode == 1) {
				// My constant shading
				GLfloat* color = getFaceColor(i);
				GLfloat finalColor[3] = {color[0], color[1], color[2]};
				
				if (lightOn && myLight != NULL) {
					// Calculate face center
					GLfloat center[3] = {0, 0, 0};
					for (int j = 0; j < 3; j++) {
						int vi = face[i][j];
						center[0] += vertex[vi][0];
						center[1] += vertex[vi][1];
						center[2] += vertex[vi][2];
					}
					center[0] /= 3.0;
					center[1] /= 3.0;
					center[2] /= 3.0;
					
					// Transform center to world coordinates
					GLfloat worldCenter[3];
					worldCenter[0] = modelview[0] * center[0] + modelview[4] * center[1] +
					                 modelview[8] * center[2] + modelview[12];
					worldCenter[1] = modelview[1] * center[0] + modelview[5] * center[1] +
					                 modelview[9] * center[2] + modelview[13];
					worldCenter[2] = modelview[2] * center[0] + modelview[6] * center[1] +
					                 modelview[10] * center[2] + modelview[14];
					
					// Light vector
					Point lightPos = myLight->getPosition();
					GLfloat lightVec[3] = {
						lightPos.x - worldCenter[0],
						lightPos.y - worldCenter[1],
						lightPos.z - worldCenter[2]
					};
					GLfloat lightDist = sqrt(lightVec[0]*lightVec[0] + 
					                         lightVec[1]*lightVec[1] + 
					                         lightVec[2]*lightVec[2]);
					if (lightDist > 0.0001) {
						lightVec[0] /= lightDist;
						lightVec[1] /= lightDist;
						lightVec[2] /= lightDist;
					}
					
					// Transform normal to world coordinates
					GLfloat* normal = getFaceNormal(i);
					GLfloat worldNormal[3];
					worldNormal[0] = modelview[0] * normal[0] + modelview[4] * normal[1] + modelview[8] * normal[2];
					worldNormal[1] = modelview[1] * normal[0] + modelview[5] * normal[1] + modelview[9] * normal[2];
					worldNormal[2] = modelview[2] * normal[0] + modelview[6] * normal[1] + modelview[10] * normal[2];
					
					// Diffuse lighting: Id = Rd * I * (n · s)
					GLfloat Rd = 0.5;
					GLfloat I = myLight->getIntensity();
					GLfloat dotNS = worldNormal[0] * lightVec[0] +
					                worldNormal[1] * lightVec[1] +
					                worldNormal[2] * lightVec[2];
					if (dotNS < 0) dotNS = 0;
					
					GLfloat intensity = Rd * I * dotNS;
					finalColor[0] = color[0] * (0.2 + intensity);
					finalColor[1] = color[1] * (0.2 + intensity);
					finalColor[2] = color[2] * (0.2 + intensity);
				}
				
				glColor3fv(finalColor);
			} else {
				// OpenGL shading modes
				GLfloat* color = getFaceColor(i);
				GLfloat mat_diffuse[] = {color[0], color[1], color[2], 1.0f};
				GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
				GLfloat mat_shininess[] = {50.0f};
				
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			}
			
			drawFace(i);
		}
		
		// Draw base
		if (shadingMode == 1) {
			GLfloat finalColor[3] = {baseColor[0], baseColor[1], baseColor[2]};
			if (lightOn && myLight != NULL) {
				// Simple ambient lighting for base
				finalColor[0] *= 0.3;
				finalColor[1] *= 0.3;
				finalColor[2] *= 0.3;
			}
			glColor3fv(finalColor);
		} else {
			GLfloat mat_diffuse[] = {baseColor[0], baseColor[1], baseColor[2], 1.0f};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		}
		drawBase();
	}
	
	glPopMatrix();
}