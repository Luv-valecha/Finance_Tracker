#include "Login_system.h"
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <openssl/sha.h> // For password hashing

LoginSystem:: LoginSystem(){
    loadUserData();
}

LoginSystem:: ~LoginSystem(){
    saveUserData();
}

bool LoginSystem:: loadUserData(){
    ifstream file("user_data.txt");
    if(!file.is_open()) return false;

    string username,password;
    while(file>>username>>password){
        userDatabase[username]=password;
    }
    
    file.close();
    return true;
}

bool LoginSystem:: saveUserData(){
    ofstream file("user_data.txt");
    if (!file.is_open()) return false;

    for (auto entry : userDatabase) {
        file<<entry.first<<" "<<entry.second<<"\n";
    }

    file.close();
    return true;
}

bool LoginSystem:: registerUser(const string& username,const string& password){
    if(userDatabase.find(username)!=userDatabase.end()){
        cerr<<"User already exists";
        return false;
    }

    userDatabase[username]=hashPassword(password);

    // Save the updated user data to the file after registration
    if (!saveUserData()) {
        cerr << "Failed to save user data!";
        return false;
    }

    return true;
}

bool LoginSystem:: loginUser(const string& username,const string& password){
    
    auto it = userDatabase.find(username);
    if (it != userDatabase.end() && it->second == hashPassword(password)) {
        cout << "Login successful" <<endl;
        return true;
    }

    cerr << "Invalid username or password" <<endl;
    return false;
}

string LoginSystem::hashPassword(const std::string &password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}
