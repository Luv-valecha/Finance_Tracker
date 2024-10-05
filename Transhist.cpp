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

    void new_transaction(Transaction *transaction)
    {
        Transaction_node *temp = new Transaction_node(transaction);
        (tail->prev)->next = temp;
        temp->next = tail;
        temp->prev = tail->prev;
        tail->prev = temp;
        number_of_transactions++;
    }

    bool empty()
    {
        return head->next == tail;
    }

    void PrintAllTransactions()
    {
        if (empty())
        {
            cout << "No Transactions Available" << endl;
            return;
        }

        Transaction_node *curr = head->next;

        while (curr != tail)
        {
            curr->transaction->printTransaction();
            curr = curr->next;
        }
    }

    void PrintNTransactions(int n)
    {
        if (empty() || number_of_transactions < n)
        {
            cout << "Invalid Number of Transactions" << endl;
            return;
        }

        int start_printing_from = number_of_transactions - n;
        Transaction_node *curr = head->next;

        while (start_printing_from)
        {
            curr = curr->next;
            start_printing_from--;
        }

        while (curr != tail)
        {
            curr->transaction->printTransaction();
            curr = curr->next;
        }
    }
};