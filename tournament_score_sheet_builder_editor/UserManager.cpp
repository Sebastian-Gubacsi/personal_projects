#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

UserManager::UserManager() : loggedIn(false), userDataFile("users.dat") {
    loadUsers();
}

UserManager::~UserManager() {
    saveUsers();
}

std::string UserManager::generateUserId() {
    std::stringstream ss;
    ss << "user_" << time(nullptr) << "_" << rand() % 1000;
    return ss.str();
}

bool UserManager::login(const std::string& email) {
    for (auto& pair : users) {
        if (pair.second.email == email) {
            currentUserId = pair.first;
            loggedIn = true;
            std::cout << "Logged in as: " << email << std::endl;
            return true;
        }
    }
    std::cerr << "User not found: " << email << std::endl;
    return false;
}

bool UserManager::registerUser(const std::string& email, const std::string& displayName) {
    // Check if email already exists
    for (const auto& pair : users) {
        if (pair.second.email == email) {
            std::cerr << "Email already registered" << std::endl;
            return false;
        }
    }
    
    User newUser;
    newUser.userId = generateUserId();
    newUser.email = email;
    newUser.displayName = displayName;
    
    users[newUser.userId] = newUser;
    saveUsers();
    
    std::cout << "User registered: " << email << std::endl;
    return login(email);
}

void UserManager::logout() {
    currentUserId = "";
    loggedIn = false;
    std::cout << "Logged out" << std::endl;
}

std::string UserManager::getCurrentUserEmail() const {
    if (!loggedIn) return "";
    auto it = users.find(currentUserId);
    if (it != users.end()) {
        return it->second.email;
    }
    return "";
}

std::string UserManager::getUserIdByEmail(const std::string& email) {
    for (const auto& pair : users) {
        if (pair.second.email == email) {
            return pair.first;
        }
    }
    return "";
}

User* UserManager::getCurrentUser() {
    if (!loggedIn) return nullptr;
    auto it = users.find(currentUserId);
    if (it != users.end()) {
        return &it->second;
    }
    return nullptr;
}

void UserManager::loadUsers() {
    std::ifstream file(userDataFile);
    if (!file.is_open()) return;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("USER:") == 0) {
            User user;
            user.userId = line.substr(5);
            
            std::getline(file, line);
            user.email = line.substr(6);
            
            std::getline(file, line);
            user.displayName = line.substr(5);
            
            users[user.userId] = user;
        }
    }
    file.close();
}

void UserManager::saveUsers() {
    std::ofstream file(userDataFile);
    if (!file.is_open()) return;
    
    for (const auto& pair : users) {
        file << "USER:" << pair.second.userId << "\n";
        file << "EMAIL:" << pair.second.email << "\n";
        file << "NAME:" << pair.second.displayName << "\n";
    }
    file.close();
}