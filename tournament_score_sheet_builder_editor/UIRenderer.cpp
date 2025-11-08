#include "UIRenderer.h"
#include <GL/glut.h>
#include <sstream>

UIRenderer::UIRenderer(SheetManager* sm, UserManager* um)
    : sheetManager(sm), userManager(um), currentState(UIState::LOGIN),
      windowWidth(1400), windowHeight(900), scrollOffsetY(0),
      selectedSection(0), selectedTag(0), selectedPlayer(0),
      dialogActive(false) {
}

void UIRenderer::updateWindowSize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
}

void UIRenderer::render() {
    switch (currentState) {
        case UIState::LOGIN:
            renderLogin();
            break;
        case UIState::MAIN_MENU:
            renderMainMenu();
            break;
        case UIState::SHEET_EDITOR:
            renderSheetEditor();
            break;
        case UIState::DIALOG:
            renderDialog();
            break;
    }
}

void UIRenderer::drawText(float x, float y, const std::string& text, bool small) {
    glRasterPos2f(x, y);
    void* font = small ? GLUT_BITMAP_HELVETICA_10 : GLUT_BITMAP_HELVETICA_12;
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void UIRenderer::drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void UIRenderer::drawBorder(float x, float y, float w, float h, float r, float g, float b, float lineWidth) {
    glColor3f(r, g, b);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void UIRenderer::drawButton(float x, float y, float w, float h, const std::string& text, bool highlighted) {
    if (highlighted) {
        drawRect(x, y, w, h, 0.3f, 0.7f, 1.0f);
    } else {
        drawRect(x, y, w, h, 0.2f, 0.6f, 0.9f);
    }
    drawBorder(x, y, w, h, 0.1f, 0.4f, 0.7f, 2.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    float textX = x + (w - text.length() * 7) / 2;
    float textY = y + h / 2 + 4;
    drawText(textX, textY, text);
}

void UIRenderer::renderLogin() {
    float centerX = windowWidth / 2;
    float centerY = windowHeight / 2;
    
    // Title
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(centerX - 150, 100, "Tournament Scoring Sheet Manager");
    
    // Login box
    drawRect(centerX - 200, centerY - 100, 400, 200, 1.0f, 1.0f, 1.0f);
    drawBorder(centerX - 200, centerY - 100, 400, 200, 0.5f, 0.5f, 0.5f, 2.0f);
    
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(centerX - 100, centerY - 60, "Press F1 to Login/Register");
    drawText(centerX - 100, centerY - 30, "Enter email address in dialog");
    
    if (userManager->isLoggedIn()) {
        std::string msg = "Logged in as: " + userManager->getCurrentUserEmail();
        drawText(centerX - 100, centerY, msg);
        drawText(centerX - 100, centerY + 30, "Press any key to continue...");
    }
}

void UIRenderer::renderMainMenu() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(50, 50, "Main Menu");
    
    std::string user = "User: " + userManager->getCurrentUserEmail();
    drawText(windowWidth - 250, 50, user);
    
    float buttonY = 100;
    drawButton(50, buttonY, 200, 40, "F2: New Sheet");
    drawButton(270, buttonY, 200, 40, "F3: Load Sheet");
    drawButton(490, buttonY, 200, 40, "F4: Share Sheet");
    drawButton(710, buttonY, 200, 40, "F5: Export Excel");
    
    // Display current sheet info
    if (!sheetManager->getCurrentSheet().sheetId.empty()) {
        drawText(50, 180, "Current Sheet: " + sheetManager->getCurrentSheet().name);
        
        if (sheetManager->hasChanges()) {
            glColor3f(1.0f, 0.5f, 0.0f);
            drawText(50, 210, "*Unsaved changes*");
        }
        
        glColor3f(0.0f, 0.0f, 0.0f);
        drawText(50, 240, "Press Enter to edit sheet");
    }
}

void UIRenderer::renderSheetEditor() {
    TournamentSheet& sheet = sheetManager->getCurrentSheet();
    
    // Header
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(20, 30, "Sheet: " + sheet.name);
    drawText(windowWidth - 200, 30, "ESC: Back to Menu");
    
    // Player headers
    float startX = 350;
    float headerY = 70;
    int cellWidth = 100;
    
    glColor3f(0.3f, 0.3f, 0.3f);
    for (size_t i = 0; i < sheet.players.size() && i < 10; i++) {
        std::string playerName = sheet.players[i];
        if (playerName.length() > 12) {
            playerName = playerName.substr(0, 12);
        }
        drawText(startX + i * cellWidth + 10, headerY, playerName);
    }
    
    // Render sections
    float y = 100 - scrollOffsetY;
    
    for (size_t secIdx = 0; secIdx < sheet.sections.size(); secIdx++) {
        const TagSection& section = sheet.sections[secIdx];
        
        if (y > windowHeight) break;
        if (y < -100) {
            y += (section.tags.size() * 35 + 60);
            continue;
        }
        
        // Section header
        drawRect(20, y, windowWidth - 40, 40, 0.8f, 0.9f, 1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        std::stringstream ss;
        ss << section.name << " (Max: " << section.maxPoints << ")";
        drawText(30, y + 25, ss.str());
        
        y += 45;
        
        // Tags
        for (size_t tagIdx = 0; tagIdx < section.tags.size(); tagIdx++) {
            const Tag& tag = section.tags[tagIdx];
            
            // Tag description
            std::string desc = tag.description;
            if (desc.length() > 40) {
                desc = desc.substr(0, 40) + "...";
            }
            
            glColor3f(0.0f, 0.0f, 0.0f);
            std::stringstream tagLabel;
            tagLabel << tag.id << ". [" << tag.pointValue << "pt] " << desc;
            drawText(30, y + 20, tagLabel.str());
            
            // Player cells
            for (size_t pIdx = 0; pIdx < sheet.players.size() && pIdx < 10; pIdx++) {
                float cellX = startX + pIdx * cellWidth;
                bool isSelected = (secIdx == selectedSection && 
                                 tagIdx == selectedTag && 
                                 pIdx == selectedPlayer);
                
                if (isSelected) {
                    drawRect(cellX, y, cellWidth - 2, 30, 1.0f, 1.0f, 0.7f);
                } else {
                    drawRect(cellX, y, cellWidth - 2, 30, 1.0f, 1.0f, 1.0f);
                }
                drawBorder(cellX, y, cellWidth - 2, 30, 0.5f, 0.5f, 0.5f);
                
                std::string value = sheetManager->getCellValue(section.name, tag.id, 
                                                               sheet.players[pIdx]);
                if (!value.empty()) {
                    if (value.length() > 10) {
                        value = value.substr(0, 10);
                    }
                    glColor3f(0.0f, 0.0f, 0.0f);
                    drawText(cellX + 5, y + 18, value);
                }
            }
            
            y += 35;
        }
        
        y += 15;
    }
    
    // Instructions
    glColor3f(0.3f, 0.3f, 0.3f);
    drawText(20, windowHeight - 30, 
             "Arrow Keys: Navigate | Enter: Edit | +/-: Add Player/Tag | PgUp/PgDn: Scroll");
}

void UIRenderer::renderDialog() {
    // Dim background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(0, windowHeight);
    glEnd();
    glDisable(GL_BLEND);
    
    // Dialog box
    float boxW = 500;
    float boxH = 200;
    float boxX = (windowWidth - boxW) / 2;
    float boxY = (windowHeight - boxH) / 2;
    
    drawRect(boxX, boxY, boxW, boxH, 1.0f, 1.0f, 1.0f);
    drawBorder(boxX, boxY, boxW, boxH, 0.2f, 0.2f, 0.2f, 3.0f);
    
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(boxX + 20, boxY + 40, dialogTitle);
    
    // Input box
    drawRect(boxX + 20, boxY + 70, boxW - 40, 35, 0.95f, 0.95f, 0.95f);
    drawBorder(boxX + 20, boxY + 70, boxW - 40, 35, 0.5f, 0.5f, 0.5f);
    
    std::string displayInput = dialogInput + "_";
    drawText(boxX + 30, boxY + 92, displayInput);
    
    drawText(boxX + 20, boxY + 130, "Press Enter to confirm, ESC to cancel");
}

void UIRenderer::setSelection(int section, int tag, int player) {
    selectedSection = section;
    selectedTag = tag;
    selectedPlayer = player;
}

void UIRenderer::getSelection(int& section, int& tag, int& player) {
    section = selectedSection;
    tag = selectedTag;
    player = selectedPlayer;
}

void UIRenderer::showDialog(const std::string& title) {
    dialogTitle = title;
    dialogInput = "";
    dialogActive = true;
}

void UIRenderer::closeDialog() {
    dialogActive = false;
    dialogInput = "";
}

void UIRenderer::scroll(int delta) {
    scrollOffsetY += delta;
    if (scrollOffsetY < 0) scrollOffsetY = 0;
}