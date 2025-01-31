#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <bits/stdc++.h>
#include <iostream>
using namespace std;

class Transaction {
public:
    std::string date;      // DD-MM-YYYY
    double amount;
    std::string category;
    std::string transaction_type;
    std::string description;

    // Constructors
    // Transaction(string date, double amount, string cat);
    Transaction(string date, double amount, string cat, string transaction_type, string desc);

    // Helper to display a transaction
    void printTransaction() const;
};

#endif // TRANSACTIONS_H
