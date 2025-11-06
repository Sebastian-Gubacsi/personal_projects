/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 * Modified: Added face colors and shading support
 */

#include "Pyramid.hpp"
#include "Light.hpp"
#include "Cube.hpp"
#include <math.h>

extern int cullingMode;
extern int shadingMode;
extern Light* myLight;
extern bool lightOn;

void Pyramid::computeFaceNormals()
{
    // Compute face normals
    for (int i = 0; i < 4; i++) {
        // Get vertices of the face
        GLfloat* v0 = vertex[face[i][0]];
        GLfloat* v1 = vertex[face[i][1]];
        GLfloat* v2 = vertex[face[i][2]];

        // Calculate two vectors on the face
        GLfloat vec1[3] = {v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]};
        GLfloat vec2[3] = {v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]};

        // Cross product to get normal
        faceNormals[i][0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
        faceNormals[i][1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
        faceNormals[i][2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];

        // Normalize
        GLfloat len = sqrt(faceNormals[i][0]*faceNormals[i][0] + 
                          faceNormals[i][1]*faceNormals[i][1] + 
                          faceNormals[i][2]*faceNormals[i][2]);
        if (len > 1e-6f) {
            faceNormals[i][0] /= len;
            faceNormals[i][1] /= len;
            faceNormals[i][2] /= len;
        }
    }

    // Base normal points down (-z)
    baseNormal[0] = 0.0f;
    baseNormal[1] = 0.0f;
    baseNormal[2] = -1.0f;
}

bool Pyramid::isBackFace(int faceIndex, GLfloat* modelview)
{
    // Calculate face center
    GLfloat center[3] = {0, 0, 0};
    for (int j = 0; j < 3; j++) {
        int vertexIndex = face[faceIndex][j];
        center[0] += vertex[vertexIndex][0];
        center[1] += vertex[vertexIndex][1];
        center[2] += vertex[vertexIndex][2];
    }
    center[0] /= 3.0f;
    center[1] /= 3.0f;
    center[2] /= 3.0f;

    // Transform face normal to world coordinates
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

    // Get camera position (inverse translation)
    GLfloat camPos[3] = {-modelview[12], -modelview[13], -modelview[14]};

    // View vector from face center to camera
    GLfloat viewVec[3] = {
        camPos[0] - worldCenter[0],
        camPos[1] - worldCenter[1],
        camPos[2] - worldCenter[2]
    };

    // Dot product between normal and view vector
    GLfloat dot = worldNormal[0] * viewVec[0] +
                  worldNormal[1] * viewVec[1] +
                  worldNormal[2] * viewVec[2];

    return dot < 0;
}

Pyramid::Pyramid()
{
    // Set solid object color for flat shading
	// legacy per-object color; global flatColor will be used for shadingMode==2
	objectColor[0] = 0.2f;  // Red component
	objectColor[1] = 0.7f;  // Green component
	objectColor[2] = 0.2f;  // Blue component (making it greenish to contrast with red cube)

    // Set coordinate values for pyramid vertices
    // Base vertices (square base at z = -0.5)
    vertex[0][0] = -0.5; vertex[0][1] = -0.5; vertex[0][2] = -0.5;
    vertex[1][0] =  0.5; vertex[1][1] = -0.5; vertex[1][2] = -0.5;
    vertex[2][0] =  0.5; vertex[2][1] =  0.5; vertex[2][2] = -0.5;
    vertex[3][0] = -0.5; vertex[3][1] =  0.5; vertex[3][2] = -0.5;
    // Apex vertex
    vertex[4][0] =  0.0; vertex[4][1] =  0.0; vertex[4][2] =  0.5;

    // Initialize face indices
    // Four triangular faces connecting base to apex
    face[0][0] = 0; face[0][1] = 1; face[0][2] = 4;  // front-right
    face[1][0] = 1; face[1][1] = 2; face[1][2] = 4;  // back-right
    face[2][0] = 2; face[2][1] = 3; face[2][2] = 4;  // back-left
    face[3][0] = 3; face[3][1] = 0; face[3][2] = 4;  // front-left

    // Initialize face colors
    faceColors[0][0] = 1.0f; faceColors[0][1] = 0.0f; faceColors[0][2] = 0.0f;  // red
    faceColors[1][0] = 0.0f; faceColors[1][1] = 1.0f; faceColors[1][2] = 0.0f;  // green
    faceColors[2][0] = 0.0f; faceColors[2][1] = 0.0f; faceColors[2][2] = 1.0f;  // blue
    faceColors[3][0] = 1.0f; faceColors[3][1] = 1.0f; faceColors[3][2] = 0.0f;  // yellow

    // Base color (bottom)
    baseColor[0] = 0.5f; baseColor[1] = 0.5f; baseColor[2] = 1.0f;  // light blue

    // Compute face and base normals
    computeFaceNormals();

    // Legacy color (for wireframe)
    r = g = b = 1.0f;  // white
}

void Pyramid::drawFace(int i)
{
	glBegin(GL_TRIANGLES);

	if (shadingMode == 2) {
		// Flat shading - use single solid color
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
		GLfloat mat_diffuse[] = {appliedColor[0], appliedColor[1], appliedColor[2], 1.0f};
		GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
		GLfloat mat_shininess[] = {30.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		// Use face normal for flat shading
		glNormal3fv(faceNormals[i]);
		

		// Draw face vertices
		for (int j = 0; j < 3; j++) {
			int vertexIndex = face[i][j];
			glVertex3fv(vertex[vertexIndex]);
		}
	}
	else if (shadingMode == 3) {
		// Smooth shading - per vertex colors and normals
		GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat mat_shininess[] = {50.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		// per-vertex processing
		for (int j = 0; j < 3; j++) {
			int vertexIndex = face[i][j];

			// compute averaged vertex normal
			GLfloat vertexNormal[3] = {0.0f, 0.0f, 0.0f};
			int facesCount = 0;
				for (int f = 0; f < 4; f++) {
				for (int v = 0; v < 3; v++) {
					if (face[f][v] == vertexIndex) {
						vertexNormal[0] += faceNormals[f][0];
						vertexNormal[1] += faceNormals[f][1];
						vertexNormal[2] += faceNormals[f][2];
						facesCount++;
						break;
					}
				}
			}
			if (vertexIndex < 4) { vertexNormal[0] += baseNormal[0]; vertexNormal[1] += baseNormal[1]; vertexNormal[2] += baseNormal[2]; facesCount++; }
			if (facesCount > 0) {
				GLfloat len = sqrt(vertexNormal[0]*vertexNormal[0] + vertexNormal[1]*vertexNormal[1] + vertexNormal[2]*vertexNormal[2]);
				if (len > 1e-6f) { vertexNormal[0] /= len; vertexNormal[1] /= len; vertexNormal[2] /= len; }
			}

			// compute averaged vertex color
			GLfloat vertexColor[3] = {0.0f, 0.0f, 0.0f};
			int colorCount = 0;
			for (int f = 0; f < 4; f++) {
				for (int v = 0; v < 3; v++) {
					if (face[f][v] == vertexIndex) {
						vertexColor[0] += faceColors[f][0];
						vertexColor[1] += faceColors[f][1];
						vertexColor[2] += faceColors[f][2];
						colorCount++;
						break;
					}
				}
			}
			if (vertexIndex < 4) { vertexColor[0] += baseColor[0]; vertexColor[1] += baseColor[1]; vertexColor[2] += baseColor[2]; colorCount++; }
			if (colorCount > 0) { vertexColor[0] /= colorCount; vertexColor[1] /= colorCount; vertexColor[2] /= colorCount; }

			// Apply color dimming if lighting is on
			if (lightOn) {
				vertexColor[0] *= colorDimFactor;
				vertexColor[1] *= colorDimFactor;
				vertexColor[2] *= colorDimFactor;
			}

			glNormal3fv(vertexNormal);
			glColor3fv(vertexColor);
			glVertex3fv(vertex[vertexIndex]);
		}
	}
	else {
		// legacy wireframe or non-OpenGL shading
		glNormal3fv(faceNormals[i]);
		GLfloat* color = getFaceColor(i);
		glColor3fv(color);
		for (int j = 0; j < 3; j++) {
			int vertexIndex = face[i][j];
			glVertex3fv(vertex[vertexIndex]);
		}
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
	} else if (shadingMode == 2) {
		// Flat shading - use global flat color
		glBegin(GL_QUADS);
		// Use the same material properties as faces
		GLfloat mat_diffuse[] = {flatColor[0], flatColor[1], flatColor[2], 1.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        
		glNormal3fv(baseNormal);
		glColor3fv(flatColor);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[2]);
		glVertex3fv(vertex[1]);
		glEnd();
	} else {
		// Smooth or other shading modes
		glBegin(GL_QUADS);
		glNormal3fv(baseNormal);
		if (shadingMode == 3) {
			glColor3fv(baseColor);  // Use interpolated color for smooth shading
		} else {
			glColor3fv(baseColor);  // Use base color for other modes
		}
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
}

void Pyramid::draw()
{
	glPushMatrix();
	this->ctmMultiply();
	glScalef(s, s, s);
	drawGeometry();
	glPopMatrix();
}

void Pyramid::drawWithCubeSeamNormals(Cube* neighbor)
{
	// Draw pyramid but smooth the seam by including neighbor cube's vertex normals
	glPushMatrix();
	this->ctmMultiply();
	glScalef(s, s, s);

	// fall back if no neighbor
	if (neighbor == NULL) { drawGeometry(); glPopMatrix(); return; }

	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	if (shadingMode == 0) {
		glColor3f(1.0, 1.0, 0.0);
		for (int i = 0; i < 4; i++) drawWireframeFace(i);
		drawBase();
		glPopMatrix();
		return;
	}

	// Draw triangular faces
	for (int i = 0; i < 4; i++) {
		if (cullingMode == 2 && isBackFace(i, modelview)) continue;

		if (shadingMode == 1) {
			// constant shading per face (as before)
			GLfloat* color = getFaceColor(i);
			GLfloat finalColor[3] = {color[0], color[1], color[2]};
			if (lightOn && myLight != NULL) {
				GLfloat center[3] = {0,0,0};
				for (int j=0;j<3;j++){ int vi = face[i][j]; center[0]+=vertex[vi][0]; center[1]+=vertex[vi][1]; center[2]+=vertex[vi][2]; }
				center[0]/=3.0; center[1]/=3.0; center[2]/=3.0;
				GLfloat worldCenter[3];
				worldCenter[0] = modelview[0]*center[0] + modelview[4]*center[1] + modelview[8]*center[2] + modelview[12];
				worldCenter[1] = modelview[1]*center[0] + modelview[5]*center[1] + modelview[9]*center[2] + modelview[13];
				worldCenter[2] = modelview[2]*center[0] + modelview[6]*center[1] + modelview[10]*center[2] + modelview[14];
				Point lightPos = myLight->getPosition();
				GLfloat lightVec[3] = { lightPos.x - worldCenter[0], lightPos.y - worldCenter[1], lightPos.z - worldCenter[2] };
				GLfloat lightDist = sqrt(lightVec[0]*lightVec[0]+lightVec[1]*lightVec[1]+lightVec[2]*lightVec[2]);
				if (lightDist>1e-6f){ lightVec[0]/=lightDist; lightVec[1]/=lightDist; lightVec[2]/=lightDist; }
				GLfloat* normal = getFaceNormal(i);
				GLfloat worldNormal[3];
				worldNormal[0] = modelview[0]*normal[0]+modelview[4]*normal[1]+modelview[8]*normal[2];
				worldNormal[1] = modelview[1]*normal[0]+modelview[5]*normal[1]+modelview[9]*normal[2];
				worldNormal[2] = modelview[2]*normal[0]+modelview[6]*normal[1]+modelview[10]*normal[2];
				GLfloat Rd=0.8f; GLfloat I=myLight->getIntensity(); GLfloat dotNS = worldNormal[0]*lightVec[0]+worldNormal[1]*lightVec[1]+worldNormal[2]*lightVec[2]; if (dotNS<0) dotNS=0;
				GLfloat intensity = Rd*I*dotNS;
				finalColor[0] = color[0]*(0.4f + intensity);
				finalColor[1] = color[1]*(0.4f + intensity);
				finalColor[2] = color[2]*(0.4f + intensity);
			}
			glColor3fv(finalColor);
			drawFace(i);
			continue;
		}

		// OpenGL flat/smooth shading: emit per-vertex data
		if (shadingMode == 2) {
			// Flat shading for seam-aware draw: use global flatColor and face normal
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			GLfloat mat_diffuse[] = {flatColor[0], flatColor[1], flatColor[2], 1.0f};
			GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
			GLfloat mat_shininess[] = {30.0f};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			glNormal3fv(faceNormals[i]);
			glColor3fv(flatColor);
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				int vertexIndex = face[i][j];
				glVertex3fv(vertex[vertexIndex]);
			}
			glEnd();
			continue;
		}
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++) {
			int vertexIndex = face[i][j];

			// compute averaged pyramid normals
			GLfloat vertexNormal[3] = {0.0f, 0.0f, 0.0f};
			int facesCount = 0;
			for (int f = 0; f < 4; f++) {
				for (int v = 0; v < 3; v++) {
					if (face[f][v] == vertexIndex) {
						vertexNormal[0] += faceNormals[f][0];
						vertexNormal[1] += faceNormals[f][1];
						vertexNormal[2] += faceNormals[f][2];
						facesCount++;
						break;
					}
				}
			}
			if (vertexIndex < 4) { vertexNormal[0] += baseNormal[0]; vertexNormal[1] += baseNormal[1]; vertexNormal[2] += baseNormal[2]; facesCount++; }

			// include cube neighbor normal for base vertices
			if (vertexIndex < 4) {
				GLfloat cubeN[3]; neighbor->getVertexNormal(vertexIndex, cubeN);
				vertexNormal[0] += cubeN[0]; vertexNormal[1] += cubeN[1]; vertexNormal[2] += cubeN[2]; facesCount++;
			}
			if (facesCount > 0) {
				GLfloat len = sqrt(vertexNormal[0]*vertexNormal[0] + vertexNormal[1]*vertexNormal[1] + vertexNormal[2]*vertexNormal[2]);
				if (len > 1e-6f) { vertexNormal[0] /= len; vertexNormal[1] /= len; vertexNormal[2] /= len; }
			}

			// compute vertex color average
			GLfloat vertexColor[3] = {0.0f, 0.0f, 0.0f};
			int colorCount = 0;
			for (int f = 0; f < 4; f++) {
				for (int v = 0; v < 3; v++) {
					if (face[f][v] == vertexIndex) {
						vertexColor[0] += faceColors[f][0]; vertexColor[1] += faceColors[f][1]; vertexColor[2] += faceColors[f][2]; colorCount++; break;
					}
				}
			}
			if (vertexIndex < 4) { vertexColor[0] += baseColor[0]; vertexColor[1] += baseColor[1]; vertexColor[2] += baseColor[2]; colorCount++; }
			if (colorCount > 0) { vertexColor[0] /= colorCount; vertexColor[1] /= colorCount; vertexColor[2] /= colorCount; }

			// Apply color dimming if lighting is on in smooth shading mode
			if (lightOn && shadingMode == 3) {
				vertexColor[0] *= colorDimFactor;
				vertexColor[1] *= colorDimFactor;
				vertexColor[2] *= colorDimFactor;
			}

			// select normal: smooth uses vertex normal, flat uses face normal
			if (shadingMode == 3) glNormal3fv(vertexNormal); else glNormal3fv(faceNormals[i]);

			glColor3fv(vertexColor);
			glVertex3fv(vertex[vertexIndex]);
		}
		glEnd();
	}

	// draw base
	if (shadingMode == 1) {
		GLfloat finalColor[3] = {baseColor[0], baseColor[1], baseColor[2]};
		if (lightOn && myLight != NULL) { finalColor[0]*=0.3f; finalColor[1]*=0.3f; finalColor[2]*=0.3f; }
		glColor3fv(finalColor);
	} else {
		GLfloat mat_diffuse[] = {baseColor[0], baseColor[1], baseColor[2], 1.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	}
	drawBase();

	glPopMatrix();
}