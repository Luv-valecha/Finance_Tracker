#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <bits/stdc++.h>
using namespace std;

class LoginSystem {
private:
    unordered_map<string, string> userDatabase; // Store usernames and hashed passwords
    bool loadUserData(); // Loads user data from a file
    bool saveUserData(); // Saves user data to a file
    const string hashPassword(const string &password); // Hash password for security

public:
    LoginSystem(); // Constructor to load user data
    ~LoginSystem(); // Destructor to save user data

    bool registerUser(const string &username, string &password);
    bool loginUser(const string &username, string &password);
};

#endif