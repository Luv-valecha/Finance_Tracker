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

    void new_transaction(Transaction *transaction,std::string username);
    bool empty();
    vector<string> PrintAllTransactions(std::string username);
    // vector<string> PrintNTransactions(int n);
    vector<string> PrintCategoryWise(std::string target,std::string username);
    vector<string> PrintDateRange(std::string from, std::string to, std::string username);
    vector<int> extract(std::string currdate);
    bool bringtransactions(std::string username);
    void makedll(Transaction* transaction);
    unordered_map<string,double> piechartvalues(std::string username);
    unordered_map<string,double> getbudget(std::string username);
    priority_queue<pair<double,string>> create_recommender(unordered_map<string,double> budget,std::string username);
    void setbudget(std::string username,unordered_map<string,double>& budget);
    vector<string> give_recommendation(std::string username);
};

#endif // TRANSHIST_H