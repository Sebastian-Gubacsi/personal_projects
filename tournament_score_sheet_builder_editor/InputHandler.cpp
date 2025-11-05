#include "InputHandler.h"
#include <GL/glut.h>
#include <iostream>

InputHandler::InputHandler(SheetManager* sm, UIRenderer* ui, UserManager* um)
    : sheetManager(sm), uiRenderer(ui), userManager(um) {
}

void InputHandler::handleKeyboard(unsigned char key, int x, int y) {
    if (uiRenderer->isDialogActive()) {
        handleDialogInput(key);
        return;
    }
    
    switch (uiRenderer->getState()) {
        case UIState::LOGIN:
            handleLoginInput(key);
            break;
        case UIState::MAIN_MENU:
            handleMenuInput(key);
            break;
        case UIState::SHEET_EDITOR:
            handleEditorInput(key);
            break;
        default:
            break;
    }
}

void InputHandler::handleLoginInput(unsigned char key) {
    if (key == GLUT_KEY_F1 || key == 241) { // F1
        uiRenderer->showDialog("Enter email:");
        uiRenderer->setState(UIState::DIALOG);
    } else if (userManager->isLoggedIn()) {
        uiRenderer->setState(UIState::MAIN_MENU);
    }
}

void InputHandler::handleMenuInput(unsigned char key) {
    if (key == GLUT_KEY_F2 || key == 242) { // F2 - New Sheet
        uiRenderer->showDialog("Enter sheet name:");
        uiRenderer->setState(UIState::DIALOG);
    } else if (key == GLUT_KEY_F5 || key == 245) { // F5 - Export
        if (!sheetManager->getCurrentSheet().sheetId.empty()) {
            sheetManager->exportToExcel("tournament_export.csv");
        }
    } else if (key == 13) { // Enter - Edit sheet
        if (!sheetManager->getCurrentSheet().sheetId.empty()) {
            uiRenderer->setState(UIState::SHEET_EDITOR);
        }
    }
}

void InputHandler::handleEditorInput(unsigned char key) {
    if (key == 27) { // ESC - Back to menu
        uiRenderer->setState(UIState::MAIN_MENU);
    } else if (key == 13) { // Enter - Edit cell
        int sec, tag, player;
        uiRenderer->getSelection(sec, tag, player);
        uiRenderer->showDialog("Enter value:");
        uiRenderer->setState(UIState::DIALOG);
    } else if (key == '+') {
        uiRenderer->showDialog("Enter player name:");
        uiRenderer->setState(UIState::DIALOG);
    }
}

void InputHandler::handleDialogInput(unsigned char key) {
    if (key == 27) { // ESC - Cancel
        uiRenderer->closeDialog();
        uiRenderer->setState(UIState::MAIN_MENU);
    } else if (key == 13) { // Enter - Confirm
        std::string input = uiRenderer->getDialogInput();
        
        if (uiRenderer->getState() == UIState::DIALOG) {
            if (!userManager->isLoggedIn()) {
                // Login/Register
                if (!userManager->login(input)) {
                    userManager->registerUser(input, input);
                }
                uiRenderer->closeDialog();
                uiRenderer->setState(UIState::MAIN_MENU);
            } else if (sheetManager->getCurrentSheet().sheetId.empty()) {
                // Create new sheet
                sheetManager->createNewSheet(input);
                uiRenderer->closeDialog();
                uiRenderer->setState(UIState::MAIN_MENU);
            } else {
                // Update cell or add player
                int sec, tag, player;
                uiRenderer->getSelection(sec, tag, player);
                
                TournamentSheet& sheet = sheetManager->getCurrentSheet();
                if (sec < sheet.sections.size()) {
                    if (player < sheet.players.size()) {
                        sheetManager->updateCellValue(
                            sheet.sections[sec].name,
                            sheet.sections[sec].tags[tag].id,
                            sheet.players[player],
                            input
                        );
                    }
                }
                
                uiRenderer->closeDialog();
                uiRenderer->setState(UIState::SHEET_EDITOR);
            }
        }
    } else if (key == 8 || key == 127) { // Backspace
        std::string& input = uiRenderer->getDialogInput();
        if (!input.empty()) {
            input.pop_back();
        }
    } else if (key >= 32 && key <= 126) {
        uiRenderer->getDialogInput() += key;
    }
}

void InputHandler::handleSpecialKeys(int key, int x, int y) {
    if (uiRenderer->getState() == UIState::SHEET_EDITOR) {
        handleEditorNavigation(key);
    } else if (key == GLUT_KEY_F1) {
        if (uiRenderer->getState() == UIState::LOGIN) {
            uiRenderer->showDialog("Enter email:");
            uiRenderer->setState(UIState::DIALOG);
        }
    } else if (key == GLUT_KEY_F2) {
        if (uiRenderer->getState() == UIState::MAIN_MENU) {
            uiRenderer->showDialog("Enter sheet name:");
            uiRenderer->setState(UIState::DIALOG);
        }
    } else if (key == GLUT_KEY_F5) {
        if (uiRenderer->getState() == UIState::MAIN_MENU) {
            sheetManager->exportToExcel("tournament_export.csv");
        }
    } else if (key == GLUT_KEY_PAGE_UP) {
        uiRenderer->scroll(-50);
    } else if (key == GLUT_KEY_PAGE_DOWN) {
        uiRenderer->scroll(50);
    }
}

void InputHandler::handleEditorNavigation(int key) {
    int sec, tag, player;
    uiRenderer->getSelection(sec, tag, player);
    
    TournamentSheet& sheet = sheetManager->getCurrentSheet();
    
    switch (key) {
        case GLUT_KEY_UP:
            if (tag > 0) tag--;
            else if (sec > 0) {
                sec--;
                tag = sheet.sections[sec].tags.size() - 1;
            }
            break;
        case GLUT_KEY_DOWN:
            if (tag < (int)sheet.sections[sec].tags.size() - 1) tag++;
            else if (sec < (int)sheet.sections.size() - 1) {
                sec++;
                tag = 0;
            }
            break;
        case GLUT_KEY_LEFT:
            if (player > 0) player--;
            break;
        case GLUT_KEY_RIGHT:
            if (player < (int)sheet.players.size() - 1) player++;
            break;
    }
    
    uiRenderer->setSelection(sec, tag, player);
}

void InputHandler::handleMouse(int button, int state, int x, int y) {
    // Mouse handling can be expanded for button clicks
}