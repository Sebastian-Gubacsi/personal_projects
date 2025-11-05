// main.cpp
#include "SheetManager.h"
#include "UIRenderer.h"
#include "InputHandler.h"
#include "UserManager.h"
#include <GL/glut.h>
#include <iostream>

// Global instances
SheetManager* sheetManager = nullptr;
UIRenderer* uiRenderer = nullptr;
InputHandler* inputHandler = nullptr;
UserManager* userManager = nullptr;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (uiRenderer) {
        uiRenderer->render();
    }
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (inputHandler) {
        inputHandler->handleKeyboard(key, x, y);
        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    if (inputHandler) {
        inputHandler->handleSpecialKeys(key, x, y);
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (inputHandler) {
        inputHandler->handleMouse(button, state, x, y);
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    if (uiRenderer) {
        uiRenderer->updateWindowSize(w, h);
    }
}

void timer(int value) {
    // Auto-save every 30 seconds
    if (sheetManager) {
        sheetManager->autoSave();
    }
    glutTimerFunc(30000, timer, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1400, 900);
    glutCreateWindow("Tournament Scoring Sheet Manager");
    
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    
    // Initialize managers
    userManager = new UserManager();
    sheetManager = new SheetManager(userManager);
    uiRenderer = new UIRenderer(sheetManager, userManager);
    inputHandler = new InputHandler(sheetManager, uiRenderer, userManager);
    
    // Set up callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(30000, timer, 0);
    
    std::cout << "Tournament Scoring Sheet Manager Started!" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "  F1  - Login/Register" << std::endl;
    std::cout << "  F2  - Create New Sheet" << std::endl;
    std::cout << "  F3  - Load Sheet" << std::endl;
    std::cout << "  F4  - Share Sheet" << std::endl;
    std::cout << "  F5  - Export to Excel" << std::endl;
    std::cout << "  Tab - Switch between sections" << std::endl;
    std::cout << "  Arrow Keys - Navigate cells" << std::endl;
    std::cout << "  Enter - Edit cell / Confirm" << std::endl;
    std::cout << "  ESC - Cancel editing" << std::endl;
    
    glutMainLoop();
    
    // Cleanup
    delete inputHandler;
    delete uiRenderer;
    delete sheetManager;
    delete userManager;
    
    return 0;
}