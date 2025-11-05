#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "SheetManager.h"
#include "UserManager.h"
#include <string>

enum class UIState {
    LOGIN,
    MAIN_MENU,
    SHEET_EDITOR,
    DIALOG
};

class UIRenderer {
private:
    SheetManager* sheetManager;
    UserManager* userManager;
    UIState currentState;
    int windowWidth;
    int windowHeight;
    
    // Scrolling and view
    int scrollOffsetY;
    int selectedSection;
    int selectedTag;
    int selectedPlayer;
    
    // Dialog state
    std::string dialogTitle;
    std::string dialogInput;
    bool dialogActive;
    
public:
    UIRenderer(SheetManager* sm, UserManager* um);
    
    void render();
    void updateWindowSize(int w, int h);
    
    UIState getState() const { return currentState; }
    void setState(UIState state) { currentState = state; }
    
    void setSelection(int section, int tag, int player);
    void getSelection(int& section, int& tag, int& player);
    
    void showDialog(const std::string& title);
    void closeDialog();
    bool isDialogActive() const { return dialogActive; }
    std::string& getDialogInput() { return dialogInput; }
    
    void scroll(int delta);
    
private:
    void renderLogin();
    void renderMainMenu();
    void renderSheetEditor();
    void renderDialog();
    
    void drawText(float x, float y, const std::string& text, bool small = false);
    void drawButton(float x, float y, float w, float h, const std::string& text, bool highlighted = false);
    void drawRect(float x, float y, float w, float h, float r, float g, float b);
    void drawBorder(float x, float y, float w, float h, float r, float g, float b, float lineWidth = 1.0f);
};

#endif