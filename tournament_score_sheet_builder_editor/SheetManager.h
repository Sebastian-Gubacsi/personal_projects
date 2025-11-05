#ifndef SHEET_MANAGER_H
#define SHEET_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "UserManager.h"

struct Tag {
    int id;
    std::string description;
    int pointValue;
    std::map<std::string, std::string> playerData; // playerId -> value
};

struct TagSection {
    std::string name;
    std::vector<Tag> tags;
    int maxPoints;
};

struct TournamentSheet {
    std::string sheetId;
    std::string name;
    std::string ownerId;
    std::vector<std::string> sharedWith;
    std::vector<std::string> players;
    std::vector<TagSection> sections;
    long long lastModified;
    
    TournamentSheet() : lastModified(0) {}
};

class SheetManager {
private:
    TournamentSheet currentSheet;
    UserManager* userManager;
    std::string saveDirectory;
    bool hasUnsavedChanges;
    
public:
    SheetManager(UserManager* um);
    ~SheetManager();
    
    bool createNewSheet(const std::string& sheetName);
    bool loadSheet(const std::string& sheetId);
    bool saveSheet();
    void autoSave();
    bool exportToExcel(const std::string& filename);
    
    bool addPlayer(const std::string& playerName);
    bool removePlayer(const std::string& playerId);
    bool addSection(const std::string& sectionName, int maxPoints);
    bool addTag(const std::string& sectionName, const std::string& description, int points);
    
    bool updateCellValue(const std::string& sectionName, int tagId, 
                        const std::string& playerId, const std::string& value);
    std::string getCellValue(const std::string& sectionName, int tagId, 
                            const std::string& playerId);
    
    bool shareSheet(const std::string& email);
    bool hasAccess(const std::string& userId);
    
    TournamentSheet& getCurrentSheet() { return currentSheet; }
    bool hasChanges() const { return hasUnsavedChanges; }
    
private:
    std::string generateSheetId();
    std::string getSheetFilePath(const std::string& sheetId);
    void loadFromFile(const std::string& filepath);
    void saveToFile(const std::string& filepath);
};

#endif