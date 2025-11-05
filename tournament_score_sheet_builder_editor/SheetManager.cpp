#include "SheetManager.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <sys/stat.h>

SheetManager::SheetManager(UserManager* um) 
    : userManager(um), saveDirectory("./sheets/"), hasUnsavedChanges(false) {
    // Create sheets directory if it doesn't exist
#ifdef _WIN32
    mkdir(saveDirectory.c_str());
#else
    mkdir(saveDirectory.c_str(), 0755);
#endif
}

SheetManager::~SheetManager() {
    if (hasUnsavedChanges) {
        saveSheet();
    }
}

std::string SheetManager::generateSheetId() {
    std::stringstream ss;
    ss << "sheet_" << time(nullptr) << "_" << rand() % 10000;
    return ss.str();
}

bool SheetManager::createNewSheet(const std::string& sheetName) {
    if (!userManager->isLoggedIn()) {
        std::cerr << "Must be logged in to create sheet" << std::endl;
        return false;
    }
    
    currentSheet = TournamentSheet();
    currentSheet.sheetId = generateSheetId();
    currentSheet.name = sheetName;
    currentSheet.ownerId = userManager->getCurrentUserId();
    currentSheet.lastModified = time(nullptr);
    
    // Create default structure based on tournament format
    addSection("Visible Tags", 16);
    addSection("Hidden Tags", 16);
    
    hasUnsavedChanges = true;
    return saveSheet();
}

bool SheetManager::loadSheet(const std::string& sheetId) {
    std::string filepath = getSheetFilePath(sheetId);
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to load sheet: " << sheetId << std::endl;
        return false;
    }
    
    loadFromFile(filepath);
    
    if (!hasAccess(userManager->getCurrentUserId())) {
        std::cerr << "Access denied to sheet" << std::endl;
        currentSheet = TournamentSheet();
        return false;
    }
    
    hasUnsavedChanges = false;
    return true;
}

bool SheetManager::saveSheet() {
    if (currentSheet.sheetId.empty()) {
        return false;
    }
    
    currentSheet.lastModified = time(nullptr);
    std::string filepath = getSheetFilePath(currentSheet.sheetId);
    saveToFile(filepath);
    hasUnsavedChanges = false;
    
    std::cout << "Sheet saved: " << currentSheet.name << std::endl;
    return true;
}

void SheetManager::autoSave() {
    if (hasUnsavedChanges && !currentSheet.sheetId.empty()) {
        saveSheet();
        std::cout << "Auto-saved sheet" << std::endl;
    }
}

bool SheetManager::addPlayer(const std::string& playerName) {
    if (playerName.empty()) return false;
    
    currentSheet.players.push_back(playerName);
    hasUnsavedChanges = true;
    return true;
}

bool SheetManager::removePlayer(const std::string& playerId) {
    auto it = std::find(currentSheet.players.begin(), 
                       currentSheet.players.end(), playerId);
    if (it != currentSheet.players.end()) {
        currentSheet.players.erase(it);
        hasUnsavedChanges = true;
        return true;
    }
    return false;
}

bool SheetManager::addSection(const std::string& sectionName, int maxPoints) {
    TagSection section;
    section.name = sectionName;
    section.maxPoints = maxPoints;
    currentSheet.sections.push_back(section);
    hasUnsavedChanges = true;
    return true;
}

bool SheetManager::addTag(const std::string& sectionName, 
                          const std::string& description, int points) {
    for (auto& section : currentSheet.sections) {
        if (section.name == sectionName) {
            Tag tag;
            tag.id = section.tags.size() + 1;
            tag.description = description;
            tag.pointValue = points;
            section.tags.push_back(tag);
            hasUnsavedChanges = true;
            return true;
        }
    }
    return false;
}

bool SheetManager::updateCellValue(const std::string& sectionName, int tagId,
                                   const std::string& playerId, const std::string& value) {
    for (auto& section : currentSheet.sections) {
        if (section.name == sectionName) {
            for (auto& tag : section.tags) {
                if (tag.id == tagId) {
                    tag.playerData[playerId] = value;
                    hasUnsavedChanges = true;
                    return true;
                }
            }
        }
    }
    return false;
}

std::string SheetManager::getCellValue(const std::string& sectionName, int tagId,
                                       const std::string& playerId) {
    for (const auto& section : currentSheet.sections) {
        if (section.name == sectionName) {
            for (const auto& tag : section.tags) {
                if (tag.id == tagId) {
                    auto it = tag.playerData.find(playerId);
                    if (it != tag.playerData.end()) {
                        return it->second;
                    }
                }
            }
        }
    }
    return "";
}

bool SheetManager::shareSheet(const std::string& email) {
    std::string userId = userManager->getUserIdByEmail(email);
    if (userId.empty()) {
        std::cerr << "User not found: " << email << std::endl;
        return false;
    }
    
    if (std::find(currentSheet.sharedWith.begin(), 
                  currentSheet.sharedWith.end(), userId) 
        == currentSheet.sharedWith.end()) {
        currentSheet.sharedWith.push_back(userId);
        hasUnsavedChanges = true;
        std::cout << "Sheet shared with: " << email << std::endl;
        return true;
    }
    return false;
}

bool SheetManager::hasAccess(const std::string& userId) {
    if (currentSheet.ownerId == userId) return true;
    return std::find(currentSheet.sharedWith.begin(),
                    currentSheet.sharedWith.end(), userId) 
           != currentSheet.sharedWith.end();
}

std::string SheetManager::getSheetFilePath(const std::string& sheetId) {
    return saveDirectory + sheetId + ".tscore";
}

void SheetManager::saveToFile(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return;
    
    // Save sheet metadata
    file << "SHEET_ID:" << currentSheet.sheetId << "\n";
    file << "NAME:" << currentSheet.name << "\n";
    file << "OWNER:" << currentSheet.ownerId << "\n";
    file << "MODIFIED:" << currentSheet.lastModified << "\n";
    
    // Save shared users
    file << "SHARED:";
    for (size_t i = 0; i < currentSheet.sharedWith.size(); i++) {
        if (i > 0) file << ",";
        file << currentSheet.sharedWith[i];
    }
    file << "\n";
    
    // Save players
    file << "PLAYERS:";
    for (size_t i = 0; i < currentSheet.players.size(); i++) {
        if (i > 0) file << ",";
        file << currentSheet.players[i];
    }
    file << "\n";
    
    // Save sections and tags
    file << "SECTIONS:" << currentSheet.sections.size() << "\n";
    for (const auto& section : currentSheet.sections) {
        file << "SECTION:" << section.name << ":" << section.maxPoints << "\n";
        file << "TAGS:" << section.tags.size() << "\n";
        for (const auto& tag : section.tags) {
            file << "TAG:" << tag.id << ":" << tag.pointValue << ":" 
                 << tag.description << "\n";
            file << "DATA:" << tag.playerData.size() << "\n";
            for (const auto& data : tag.playerData) {
                file << data.first << "=" << data.second << "\n";
            }
        }
    }
    
    file.close();
}

void SheetManager::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    
    currentSheet = TournamentSheet();
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.find("SHEET_ID:") == 0) {
            currentSheet.sheetId = line.substr(9);
        } else if (line.find("NAME:") == 0) {
            currentSheet.name = line.substr(5);
        } else if (line.find("OWNER:") == 0) {
            currentSheet.ownerId = line.substr(6);
        } else if (line.find("MODIFIED:") == 0) {
            currentSheet.lastModified = std::stoll(line.substr(9));
        } else if (line.find("SHARED:") == 0) {
            std::string shared = line.substr(7);
            std::stringstream ss(shared);
            std::string userId;
            while (std::getline(ss, userId, ',')) {
                if (!userId.empty()) {
                    currentSheet.sharedWith.push_back(userId);
                }
            }
        } else if (line.find("PLAYERS:") == 0) {
            std::string players = line.substr(8);
            std::stringstream ss(players);
            std::string player;
            while (std::getline(ss, player, ',')) {
                if (!player.empty()) {
                    currentSheet.players.push_back(player);
                }
            }
        } else if (line.find("SECTIONS:") == 0) {
            int sectionCount = std::stoi(line.substr(9));
            for (int i = 0; i < sectionCount; i++) {
                std::getline(file, line);
                if (line.find("SECTION:") == 0) {
                    TagSection section;
                    size_t pos1 = line.find(':', 8);
                    section.name = line.substr(8, pos1 - 8);
                    section.maxPoints = std::stoi(line.substr(pos1 + 1));
                    
                    std::getline(file, line);
                    int tagCount = std::stoi(line.substr(5));
                    
                    for (int j = 0; j < tagCount; j++) {
                        std::getline(file, line);
                        Tag tag;
                        size_t p1 = line.find(':', 4);
                        size_t p2 = line.find(':', p1 + 1);
                        tag.id = std::stoi(line.substr(4, p1 - 4));
                        tag.pointValue = std::stoi(line.substr(p1 + 1, p2 - p1 - 1));
                        tag.description = line.substr(p2 + 1);
                        
                        std::getline(file, line);
                        int dataCount = std::stoi(line.substr(5));
                        for (int k = 0; k < dataCount; k++) {
                            std::getline(file, line);
                            size_t eq = line.find('=');
                            std::string key = line.substr(0, eq);
                            std::string val = line.substr(eq + 1);
                            tag.playerData[key] = val;
                        }
                        
                        section.tags.push_back(tag);
                    }
                    
                    currentSheet.sections.push_back(section);
                }
            }
        }
    }
    
    file.close();
}

bool SheetManager::exportToExcel(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to create export file" << std::endl;
        return false;
    }
    
    // Write header
    file << "Tournament: " << currentSheet.name << "\n\n";
    
    for (const auto& section : currentSheet.sections) {
        file << section.name << " (MAX: " << section.maxPoints << ")\n";
        file << "Tag ID,Description,Point Value";
        
        for (const auto& player : currentSheet.players) {
            file << "," << player;
        }
        file << "\n";
        
        for (const auto& tag : section.tags) {
            file << tag.id << ",\"" << tag.description << "\"," << tag.pointValue;
            
            for (const auto& player : currentSheet.players) {
                file << ",";
                auto it = tag.playerData.find(player);
                if (it != tag.playerData.end()) {
                    file << it->second;
                }
            }
            file << "\n";
        }
        file << "\n";
    }
    
    // Calculate totals
    file << "\nPlayer Totals\n";
    file << "Player";
    for (const auto& section : currentSheet.sections) {
        file << "," << section.name;
    }
    file << ",Grand Total\n";
    
    for (const auto& player : currentSheet.players) {
        file << player;
        int grandTotal = 0;
        
        for (const auto& section : currentSheet.sections) {
            int sectionTotal = 0;
            for (const auto& tag : section.tags) {
                auto it = tag.playerData.find(player);
                if (it != tag.playerData.end() && !it->second.empty()) {
                    // Try to parse as number for scoring
                    if (it->second == "Yes" || it->second == "YES") {
                        sectionTotal += tag.pointValue;
                    }
                }
            }
            file << "," << sectionTotal;
            grandTotal += sectionTotal;
        }
        file << "," << grandTotal << "\n";
    }
    
    file.close();
    std::cout << "Exported to: " << filename << std::endl;
    return true;
}