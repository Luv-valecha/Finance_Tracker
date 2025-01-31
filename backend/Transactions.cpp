#include <bits/stdc++.h>
#include "Transactions.h"
using namespace std;

// Transaction class

Transaction::Transaction(string date, double amount, string category, string transaction_type, string description)
    : date(date), amount(amount), category(category), transaction_type(transaction_type), description(description)
{
}

void Transaction::printTransaction() const
{
    cout << "Date: " << date << ", Amount: " << amount << "INR"
         << ", Category: " << category << ", Description: " << description << endl;
}