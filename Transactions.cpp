#include <bits/stdc++.h>
using namespace std;

// This class represents individual transactions, with attributes such as date, amount, category, and description.
class Transaction{
public:
    string date; // DD-MM-YYYY
    double amount;
    string category;
    string description;

    //add category after converting the string in lowercase
    Transaction(const string &date, double amount, const string &cat)
        : date(date), amount(amount), category(cat), description("") {};

    Transaction(const string &date, double amount, const string &cat, const string &desc)
        : date(date), amount(amount), category(cat), description(desc) {};

    // Helper to display a transaction
    void printTransaction() const{
        cout << "Date: " << date << ", Amount: " << amount << "INR"
                  << ", Category: " << category << ", Description: " << description << endl;
    }
};