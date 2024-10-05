#include <bits/stdc++.h>
#include "Transactions.cpp"
using namespace std;

// Linked list node definition
class Transaction_node
{
public:
    Transaction *transaction;
    Transaction_node *next;
    Transaction_node *prev;

    Transaction_node(Transaction *transaction)
        : transaction(transaction), prev(nullptr), next(nullptr) {}
};

// This class maintains a doubly linked list of transactions for efficient transaction management.
/*
Features:
-Add transaction
-Remove last transaction ***
-Summary of last n transactions
-Summary filtered by category
-Summary for range of date
-Search by description or amount
-Statistical summary
-Graphical Report
*/

class Transaction_history
{
private:
    Transaction_node *head;
    Transaction_node *tail;
    int number_of_transactions;

public:
    Transaction_history()
    {
        // dummy head and tail
        head = new Transaction_node(new Transaction("", -1, ""));
        tail = new Transaction_node(new Transaction("", -1, ""));
        number_of_transactions = 0;
    }

    void new_transaction(Transaction *transaction){}
};