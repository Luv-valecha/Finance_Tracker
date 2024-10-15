#include "Login_system.h"
#include <iostream>
#include <fstream>
#include <sstream>

LoginSystem::LoginSystem() {
    loadUserData();
}

LoginSystem::~LoginSystem() {
    saveUserData();
}

bool LoginSystem::loadUserData() {
    std::ifstream file("user_data.txt");
    if (!file.is_open()) return false;

    std::string username, password;
    while (file >> username >> password) {
        userDatabase[username] = password;
    }
    
    file.close();
    return true;
}

bool LoginSystem::saveUserData() {
    std::ofstream file("user_data.txt");
    if (!file.is_open()) return false;

    for (const auto& entry : userDatabase) {
        file << entry.first << " " << entry.second << "\n";
    }

    file.close();
    return true;
}

bool LoginSystem::registerUser(const std::string& username, const std::string& password) {
    if (userDatabase.find(username) != userDatabase.end()) {
        std::cerr << "User already exists\n";
        return false;
    }

    userDatabase[username] = password;

    // Save the updated user data to the file after registration
    if (!saveUserData()) {
        std::cerr << "Failed to save user data!\n";
        return false;
    }

    // std::cout << "User registered successfully\n";
    return true;
}

bool LoginSystem::loginUser(const std::string& username, const std::string& password) {
    auto it = userDatabase.find(username);
    if (it != userDatabase.end() && it->second == password) {
        // std::cout << "Login successful\n";
        return true;
    }

    std::cerr << "Invalid username or password\n";
    return false;
}