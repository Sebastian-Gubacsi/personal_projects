/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added face colors and shading support
 */

#include "Cube.hpp"
#include "Light.hpp"
#include <math.h>

extern int cullingMode;
extern int shadingMode;
extern Light* myLight;
extern bool lightOn;

Cube::Cube()
{
	// legacy per-object color kept for non-flat modes; global flatColor used for shadingMode==2
	objectColor[0] = 0.7f;  // Red component
	objectColor[1] = 0.2f;  // Green component
	objectColor[2] = 0.2f;  // Blue component

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
	
	// Set face colors - arranged so no adjacent faces have same color
	// Bottom face (looking down)
	faceColors[0][0] = 1.0; faceColors[0][1] = 0.0; faceColors[0][2] = 0.0; // Red
	// Top face (looking up)
	faceColors[1][0] = 0.0; faceColors[1][1] = 1.0; faceColors[1][2] = 0.0; // Green
	// Front face
	faceColors[2][0] = 0.0; faceColors[2][1] = 0.0; faceColors[2][2] = 1.0; // Blue
	// Right face
	faceColors[3][0] = 1.0; faceColors[3][1] = 1.0; faceColors[3][2] = 0.0; // Yellow
	// Back face
	faceColors[4][0] = 0.0; faceColors[4][1] = 1.0; faceColors[4][2] = 1.0; // Cyan
	// Left face
	faceColors[5][0] = 1.0; faceColors[5][1] = 0.0; faceColors[5][2] = 1.0; // Magenta
	
	// Compute face normals
	computeFaceNormals();
}

void Cube::computeFaceNormals()
{
	// Compute normal for each face
	for (int i = 0; i < 6; i++) {
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
}

bool Cube::isBackFace(int faceIndex, GLfloat* modelview)
{
	// Get face center in world coordinates
	GLfloat center[3] = {0, 0, 0};
	for (int j = 0; j < 4; j++) {
		int vi = face[faceIndex][j];
		center[0] += vertex[vi][0];
		center[1] += vertex[vi][1];
		center[2] += vertex[vi][2];
	}
	center[0] /= 4.0;
	center[1] /= 4.0;
	center[2] /= 4.0;
	
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
	
	// Get camera position (inverse of modelview translation)
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

void Cube::drawFace(int i)
{
	glBegin(GL_QUADS);

	// If OpenGL shading modes:
	// - shadingMode == 2 => Flat: one solid color per face (flat-lit)
	// - shadingMode == 3 => Smooth: per-vertex normals and per-vertex colors (interpolated)
	if (shadingMode == 2) {
		// Enable depth test to prevent z-fighting
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
        
		// Determine displayed color (dim when lighting is on)
		GLfloat appliedColor[3];
		if (lightOn) {
			appliedColor[0] = flatColor[0] * colorDimFactor;
			appliedColor[1] = flatColor[1] * colorDimFactor;
			appliedColor[2] = flatColor[2] * colorDimFactor;
		} else {
			appliedColor[0] = flatColor[0]; appliedColor[1] = flatColor[1]; appliedColor[2] = flatColor[2];
		}

		// Set up material properties using applied color
		GLfloat mat_diffuse[] = {appliedColor[0], appliedColor[1], appliedColor[2], 1.0f};
		GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
		GLfloat mat_shininess[] = {30.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		// Use face normal for flat shading
		glNormal3fv(faceNormals[i]);
        
		// Apply the computed color
		glColor3fv(appliedColor);

		// Draw the face vertices
		for (int j = 0; j < 4; j++) {
			int vertexIndex = face[i][j];
			glVertex3fv(vertex[vertexIndex]);
		}
	}
	else if (shadingMode == 3) {
		// set some specular/shininess (diffuse will be driven by glColor when color material enabled)
		GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat mat_shininess[] = {50.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		// per-vertex processing
		for (int j = 0; j < 4; j++) {
			int vertexIndex = face[i][j];

			// compute averaged vertex normal (for smooth shading)
			GLfloat vertexNormal[3] = {0.0f, 0.0f, 0.0f};
			int facesCount = 0;
			for (int f = 0; f < 6; f++) {
				for (int v = 0; v < 4; v++) {
					if (face[f][v] == vertexIndex) {
						vertexNormal[0] += faceNormals[f][0];
						vertexNormal[1] += faceNormals[f][1];
						vertexNormal[2] += faceNormals[f][2];
						facesCount++;
						break;
					}
				}
			}
			if (facesCount > 0) {
				GLfloat len = sqrt(vertexNormal[0]*vertexNormal[0] + vertexNormal[1]*vertexNormal[1] + vertexNormal[2]*vertexNormal[2]);
				if (len > 1e-6f) {
					vertexNormal[0] /= len; vertexNormal[1] /= len; vertexNormal[2] /= len;
				}
			}

			// compute averaged vertex color from adjacent faces
			GLfloat vertexColor[3] = {0.0f, 0.0f, 0.0f};
			int colorCount = 0;
			for (int f = 0; f < 6; f++) {
				for (int v = 0; v < 4; v++) {
					if (face[f][v] == vertexIndex) {
						vertexColor[0] += faceColors[f][0];
						vertexColor[1] += faceColors[f][1];
						vertexColor[2] += faceColors[f][2];
						colorCount++;
						break;
					}
				}
			}
			if (colorCount > 0) {
				vertexColor[0] /= colorCount; vertexColor[1] /= colorCount; vertexColor[2] /= colorCount;
			}

			// For flat shading, use face normal; for smooth, use averaged vertex normal
			if (shadingMode == 3) {
				glNormal3fv(vertexNormal);
			} else {
				glNormal3fv(faceNormals[i]);
			}

			// set per-vertex color (used as material if GL_COLOR_MATERIAL is enabled)
			if (lightOn) {
				vertexColor[0] *= colorDimFactor; vertexColor[1] *= colorDimFactor; vertexColor[2] *= colorDimFactor;
			}
			glColor3fv(vertexColor);
			glVertex3fv(vertex[vertexIndex]);
		}
	}
	else {
		// legacy non-OpenGL shading modes: use face normal and face color (shadingMode 0 or 1 handled elsewhere)
		glNormal3fv(faceNormals[i]);
		GLfloat* color = getFaceColor(i);
		glColor3fv(color);
		for (int j = 0; j < 4; j++) {
			int vertexIndex = face[i][j];
			glVertex3fv(vertex[vertexIndex]);
		}
	}

	glEnd();
}

void Cube::drawWireframeFace(int i)
{
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 4; j++) {
		int vertexIndex = face[i][j];
		glVertex3fv(vertex[vertexIndex]);
	}
	glEnd();
}

GLfloat* Cube::getFaceColor(int faceIndex)
{
	return faceColors[faceIndex];
}

GLfloat* Cube::getFaceNormal(int faceIndex)
{
	return faceNormals[faceIndex];
}

void Cube::drawGeometry()
{
	// Get modelview matrix for backface culling
	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	// Set consistent polygon offset for depth calculations
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	
	// Draw based on shading mode
	if (shadingMode == 0) {
		// Wireframe mode
		glColor3f(1.0, 0.0, 0.0);
		for (int i = 0; i < 6; i++) {
			drawWireframeFace(i);
		}
	} else {
		// Filled faces with culling
		for (int i = 0; i < 6; i++) {
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
					for (int j = 0; j < 4; j++) {
						int vi = face[i][j];
						center[0] += vertex[vi][0];
						center[1] += vertex[vi][1];
						center[2] += vertex[vi][2];
					}
					center[0] /= 4.0;
					center[1] /= 4.0;
					center[2] /= 4.0;
					
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
					GLfloat Rd = 0.8;  // Increased diffuse reflection
					GLfloat I = myLight->getIntensity();
					GLfloat dotNS = worldNormal[0] * lightVec[0] +
					                worldNormal[1] * lightVec[1] +
					                worldNormal[2] * lightVec[2];
					if (dotNS < 0) dotNS = 0;
					
					GLfloat intensity = Rd * I * dotNS;
					finalColor[0] = color[0] * (0.4 + intensity);  // Increased ambient component
					finalColor[1] = color[1] * (0.4 + intensity);
					finalColor[2] = color[2] * (0.4 + intensity);
				}
				
				glColor3fv(finalColor);
			} else {
				// OpenGL shading modes
				GLfloat* color = getFaceColor(i);
				GLfloat appliedFaceColor[3] = {color[0], color[1], color[2]};
				if (lightOn) { appliedFaceColor[0] *= colorDimFactor; appliedFaceColor[1] *= colorDimFactor; appliedFaceColor[2] *= colorDimFactor; }
				GLfloat mat_diffuse[] = {appliedFaceColor[0], appliedFaceColor[1], appliedFaceColor[2], 1.0f};
				GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
				GLfloat mat_shininess[] = {50.0f};
		
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			}
			
			drawFace(i);
		}
	}
}

void Cube::draw()
{
	glPushMatrix();
	this->ctmMultiply();
	glScalef(s, s, s);
	drawGeometry();
	glPopMatrix();
}

void Cube::getVertexNormal(int vertexIndex, GLfloat out[3])
{
	// Average normals of faces that include this vertex
	out[0] = out[1] = out[2] = 0.0f;
	int count = 0;
	for (int f = 0; f < 6; f++) {
		for (int v = 0; v < 4; v++) {
			if (face[f][v] == vertexIndex) {
				out[0] += faceNormals[f][0];
				out[1] += faceNormals[f][1];
				out[2] += faceNormals[f][2];
				count++;
				break;
			}
		}
	}
	if (count > 0) {
		GLfloat len = sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
		if (len > 1e-6f) { out[0] /= len; out[1] /= len; out[2] /= len; }
	}
}