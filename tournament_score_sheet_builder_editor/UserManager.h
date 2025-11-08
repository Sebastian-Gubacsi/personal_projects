#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <map>

struct User {
    std::string userId;
    std::string email;
    std::string displayName;
};

class UserManager {
private:
    std::map<std::string, User> users;
    std::string currentUserId;
    bool loggedIn;
    std::string userDataFile;
    
public:
    UserManager();
    ~UserManager();
    
    bool login(const std::string& email);
    bool registerUser(const std::string& email, const std::string& displayName);
    void logout();
    
    bool isLoggedIn() const { return loggedIn; }
    std::string getCurrentUserId() const { return currentUserId; }
    std::string getCurrentUserEmail() const;
    std::string getUserIdByEmail(const std::string& email);
    
    User* getCurrentUser();
    
private:
    void loadUsers();
    void saveUsers();
    std::string generateUserId();
};

#endif