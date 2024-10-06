#ifndef TRANSHIST_H
#define TRANSHIST_H

#include <bits/stdc++.h>
#include "Transactions.h" 
using namespace std;// Include the header for Transaction class

// Linked list node definition
class Transaction_node {
public:
    Transaction *transaction;
    Transaction_node *next;
    Transaction_node *prev;

    Transaction_node(Transaction *transaction)
        : transaction(transaction), prev(nullptr), next(nullptr) {}
};

// This class maintains a doubly linked list of transactions for efficient transaction management.
class Transaction_history {
private:
    Transaction_node *head;
    Transaction_node *tail;
    int number_of_transactions;

public:
    Transaction_history();

    void new_transaction(Transaction *transaction);
    bool empty();
    vector<string> PrintAllTransactions();
    vector<string> PrintNTransactions(int n);
    vector<string> PrintCategoryWise(string target);
    vector<string> PrintDateRange(string from, string to);
    vector<int> extract(string currdate);
};

#endif // TRANSHIST_H