#include "Login_system.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

vector<string> hash_small = {
    "ke3o1@#$bn", "zx8f&3lka!", "vg89&1@zxc", "we78@#9bnm", "bn3w4$ad!8",
    "cx4v9@asd3", "zxc1@!lkwe", "vbmn3@#$12", "lkwo@!a87s", "poiu8$#bn!",
    "qwe12@!mnb", "rty6@&3cvb", "bvc3@2g8@j", "poi4%8ljdf", "uyt8$2@qwe",
    "ghj3@!nmop", "mnbv3$%lkwe", "lkjf%8@cxv9", "zasq9@#$1ws", "qwe9$1asd%",
    "plok8@#$qwe"};
vector<string> hash_capital = {
    "QwEr6&%#Yt", "UiOp45@!Ma", "AsDf9$9Gh@", "JkLq82&$Zx", "MnBv!@PqEr",
    "CxZa1@#$Ui", "FgTy&3Bn!", "YhN7&1@QwEr", "OaSd@#9MnZ", "VcFd4$Ad!G",
    "ZnV1@!AdEf", "BnMq1@!XwEr", "CxTy3@#$12Z", "YhUo@!1AsDf", "QwEr8$#UiOp",
    "ZxAs2@!CvBn", "TyUi6@&3PoI", "MnBc3@2G8@J", "LkJi4%8PaSd", "RtYu8$2@QwEr",
    "FgHy3@!UiOp", "MnBc9$%CvBn", "CxVb%8@QwEr", "ZxAs9@#$1LkJ", "QwEr9$1MnBc",
    "PlOk8@#$QwEr"};

LoginSystem::LoginSystem()
{
    loadUserData();
}

LoginSystem::~LoginSystem()
{
    saveUserData();
}

const string LoginSystem::hashPassword(const string &password)
{
    string newpwd="";
    for(int i=0;i<password.size();i++){
        if(!isalpha(password[i])) newpwd+=password[i];
        else if(password[i]<='z' && password[i]>='a')  newpwd+=hash_small[password[i]-'a'];
        else newpwd+= hash_capital[password[i]-'A'];
    }
    return newpwd;
}

bool LoginSystem::loadUserData()
{
    ifstream file("user_data.txt");
    if (!file.is_open())
        return false;

    string username, password;
    while (file >> username >> password)
    {
        userDatabase[username] = password;
    }

    file.close();
    return true;
}

bool LoginSystem::saveUserData()
{
    ofstream file("user_data.txt");
    if (!file.is_open())
        return false;

    for (const auto &entry : userDatabase)
    {
        file << entry.first << " " << entry.second << "\n";
    }

    file.close();
    return true;
}

bool LoginSystem::registerUser(const string &username, string &password)
{
    if (userDatabase.find(username) != userDatabase.end())
    {
        cerr << "User already exists\n";
        return false;
    }

    password=hashPassword(password);

    userDatabase[username] = password;

    // Save the updated user data to the file after registration
    if (!saveUserData())
    {
        cerr << "Failed to save user data!\n";
        return false;
    }

    // cout << "User registered successfully\n";
    return true;
}

bool LoginSystem::loginUser(const string &username, string &password)
{
    auto it = userDatabase.find(username);
    password=hashPassword(password);
    if (it != userDatabase.end() && it->second == password)
    {
        // cout << "Login successful\n";
        return true;
    }

    cerr << "Invalid username or password\n";
    return false;
}