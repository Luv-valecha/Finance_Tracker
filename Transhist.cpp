#include <bits/stdc++.h>
#include "Transactions.h"
#include "Transhist.h"
using namespace std;

// This class maintains a doubly linked list of transactions for efficient transaction management.
/*
Features:
-Add transaction
-Remove last transaction ***
-Summary of last n transactions
-Summary filtered by category
-Summary for range of date
-Statistical summary
-Graphical Report
*/

Transaction_history::Transaction_history()
{
    // dummy head and tail
    head = new Transaction_node(new Transaction("", -1, "", ""));
    tail = new Transaction_node(new Transaction("", -1, "", ""));
    head->next = tail;
    tail->prev = head;
    number_of_transactions = 0;
}

void Transaction_history::new_transaction(Transaction *transaction)
{
    Transaction_node *temp = new Transaction_node(transaction); // Creating a new node
    // Ensure correct linking
    temp->next = tail;
    temp->prev = tail->prev;
    (tail->prev)->next = temp;
    (tail->prev) = temp; // Correctly link the tail
    number_of_transactions++;
}

bool Transaction_history::empty()
{
    return head->next == tail;
}

vector<string> Transaction_history::PrintAllTransactions()
{
    vector<string> transactions;
    if (empty())
    {
        transactions.push_back("No Transactions Available");
        return transactions;
    }
    Transaction_node *curr = head->next;
    while (curr != tail)
    {
        stringstream ss;
        ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
           << " INR, Category: " << curr->transaction->category
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        curr = curr->next;
    }
    return transactions;
}

vector<string> Transaction_history::PrintNTransactions(int n)
{
    vector<string> transactions;
    if (empty() || number_of_transactions < n)
    {
        transactions.push_back("Invalid Number of Transactions");
        return transactions;
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
        stringstream ss;
        ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
           << " INR, Category: " << curr->transaction->category
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        curr = curr->next;
    }
    return transactions;
}

vector<string> Transaction_history::PrintCategoryWise(string target)
{
    vector<string> transactions;
    if (empty())
    {
        transactions.push_back("No Transactions Available");
        return transactions;
    }

    int nothing_printed = 1;
    Transaction_node *curr = head->next;
    while (curr != tail)
    {
        if (curr->transaction->category == target)
        {
            stringstream ss;
            ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
               << " INR, Category: " << curr->transaction->category
               << ", Description: " << curr->transaction->description;
            transactions.push_back(ss.str());
            nothing_printed = 0;
        }
        curr = curr->next;
    }

    if (nothing_printed)
        transactions.push_back("No Transactions Available");
    return transactions;
}

vector<int> Transaction_history::extract(string currdate)
{
    int date = 0, month = 0, year = 0;
    date = (currdate[0] - '0') * 10 + (currdate[1] - '0');
    month = (currdate[3] - '0') * 10 + (currdate[4] - '0');
    year = (currdate[6] - '0') * 1000 + (currdate[7] - '0') * 100 + (currdate[8] - '0') * 10 + (currdate[9] - '0');
    return {date, month, year};
}

// DD-MM-YYYY format
vector<string> Transaction_history::PrintDateRange(string from, string to)
{
    vector<string> transactions;
    if (empty())
    {
        transactions.push_back("No transactions Available");
        return transactions;
    }

    int nothing_printed = 1;
    Transaction_node *curr = head->next;
    while (curr != tail)
    {
        string currdate = curr->transaction->date;
        vector<int> credentials = extract(currdate);
        vector<int> from_credentials = extract(from);
        vector<int> to_credentials = extract(to);
        if (credentials[2] < from_credentials[2])
        {
            curr = curr->next;
            continue;
        }
        if (credentials[1] < from_credentials[1])
        {
            curr = curr->next;
            continue;
        }
        if (credentials[0] < from_credentials[0])
        {
            curr = curr->next;
            continue;
        }
        if (credentials[2] > to_credentials[2])
            break;
        if (credentials[1] > to_credentials[1])
            break;
        if (credentials[0] > to_credentials[0])
            break;
        stringstream ss;
        ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
           << " INR, Category: " << curr->transaction->category
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        nothing_printed = 0;
        curr = curr->next;
    }

    if (nothing_printed)
        transactions.push_back("No Transactions Available");
    return transactions;
}