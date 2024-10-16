#ifndef TRANSHIST_H
#define TRANSHIST_H

#include <bits/stdc++.h>
#include <fstream>
#include "Transactions.h" 
using namespace std;

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
    unordered_map<string,double> categoryspend;
    unordered_map<string,double> budget;
    priority_queue<pair<double,string>> recommender; 
    // int number_of_transactions;

public:
    Transaction_history();

    void new_transaction(Transaction *transaction,string username);
    bool empty();
    vector<string> PrintAllTransactions(string username);
    // vector<string> PrintNTransactions(int n);
    vector<string> PrintCategoryWise(string target,string username);
    vector<string> PrintDateRange(string from, string to, string username);
    vector<int> extract(string currdate);
    bool bringtransactions(string username);
    void makedll(Transaction* transaction);
    unordered_map<string,double> piechartvalues(string username);
    unordered_map<string,double> getbudget(string username);
    priority_queue<pair<double,string>> create_recommender(unordered_map<string,double> budget,string username);
    void setbudget(string username,unordered_map<string,double>& budget);
    vector<string> give_recommendation(string username);
};

#endif // TRANSHIST_H