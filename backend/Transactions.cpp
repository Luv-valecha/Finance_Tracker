#include <bits/stdc++.h>
#include "Transactions.h"
using namespace std;

// Transaction::Transaction(string date, double amount, string category)
//     : date(date), amount(amount), category(category), description("") {
// }

Transaction::Transaction(string date, double amount, string category, string description)
    : date(date), amount(amount), category(category), description(description) {
}


void Transaction:: printTransaction() const{
        cout << "Date: " << date << ", Amount: " << amount << "INR"
                  << ", Category: " << category << ", Description: " << description << endl;
    }