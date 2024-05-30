#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

class UserManager {
public:
    bool userExists(const string& username);
    void createUser(const string& username, const string& password);
};

#endif /* USERMANAGER_H */
