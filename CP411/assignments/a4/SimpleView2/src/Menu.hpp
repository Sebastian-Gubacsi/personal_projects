/*
 * Author: Sebastian Gubacsi
 * Version: 2025-10-22
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include <GL/glut.h>

// Function to reset all states to initial values
void resetAll(void);
void menu();
void mainMenu(GLint option);
void ObjSubMenu(GLint objectOption);
void MCSTransMenu(GLint transOption);
void WCSTransMenu(GLint transOption);
void VCSTransMenu(GLint transOption);
void CullingMenu(GLint option);
void LightingMenu(GLint option);
void ShadingMenu(GLint option);
void AnimationMenu(GLint option);
void MCSTransform(GLint);
void WCSTransform(GLint);
void VCSTransform(GLint);
void LightTransform(GLint);

#endif