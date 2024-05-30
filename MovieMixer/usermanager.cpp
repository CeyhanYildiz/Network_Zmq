#include "UserManager.h"
#include <fstream>

using namespace std;
using namespace fs;

bool UserManager::userExists(const string& username) {
    return exists(username);
}

void UserManager::createUser(const string& username, const string& password) {
    create_directory(username);
    ofstream passwordFile(username + "/password.txt");
    passwordFile << password;
    passwordFile.close();
}
