#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "SheetManager.h"
#include "UIRenderer.h"
#include "UserManager.h"

class InputHandler {
private:
    SheetManager* sheetManager;
    UIRenderer* uiRenderer;
    UserManager* userManager;
    
public:
    InputHandler(SheetManager* sm, UIRenderer* ui, UserManager* um);
    
    void handleKeyboard(unsigned char key, int x, int y);
    void handleSpecialKeys(int key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    
private:
    void handleLoginInput(unsigned char key);
    void handleMenuInput(unsigned char key);
    void handleEditorInput(unsigned char key);
    void handleDialogInput(unsigned char key);
    
    void handleEditorNavigation(int key);
};

#endif