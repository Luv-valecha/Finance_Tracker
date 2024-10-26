#include <bits/stdc++.h>
#include <fstream>
#include "Transactions.h"
#include "Transhist.h"
#ifdef _WIN32
#include <direct.h> // For creating directories on Windows
#else
#include <sys/stat.h> // For creating directories on UNIX-based systems
#include <sys/types.h>
#endif

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
    // number_of_transactions = 0;
    categoryspend = {
        {"Food", 0},
        {"Shopping", 0},
        {"Entertainment", 0},
        {"Personal", 0},
        {"Health", 0},
        {"Travel", 0},
        {"Miscellaneous", 0}};
}

void Transaction_history::new_transaction(Transaction *transaction, std::string username)
{
    Transaction_node *temp = new Transaction_node(transaction); // Creating a new node
    // // Ensure correct linking
    // temp->next = tail;
    // temp->prev = tail->prev;
    // (tail->prev)->next = temp;
    // (tail->prev) = temp;
    // // number_of_transactions++;
    categoryspend[transaction->category] += transaction->amount;
    std::string directory = "user_transaction_details";
// Create the directory if it doesn't exist
#ifdef _WIN32
    _mkdir(directory.c_str()); // For Windows
#else
    mkdir(directory.c_str(), 0777); // For UNIX-based systems
#endif

    std::string filename = directory + "/" + username + ".txt";
    std::cout << "opening file: " << filename;

    // store the transaction arranged in order of transaction date
    bringtransactions(username);
    Transaction_node *curr = head->next;
    vector<int> tdate = extract(transaction->date);
    while (curr != tail)
    {
        vector<int> cdate = extract(curr->transaction->date);

        // Compare year
        if (cdate[2] > tdate[2])
            break;
        if (cdate[2] < tdate[2])
        {
            curr = curr->next;
            continue;
        }

        // Compare month
        if (cdate[1] > tdate[1])
            break;
        if (cdate[1] < tdate[1])
        {
            curr = curr->next;
            continue;
        }

        // Compare day
        if (cdate[0] > tdate[0])
            break;
        if (cdate[0] < tdate[0])
        {
            curr = curr->next;
            continue;
        }

        curr = curr->next;
    }

    temp->next = curr;
    (curr->prev)->next = temp;
    temp->prev = curr->prev;
    temp->next = curr;
    curr->prev = temp;
    std::ofstream file(filename);
    Transaction_node *node = head->next;
    while (node != tail)
    {
        file << node->transaction->date << " " << node->transaction->amount << " "
             << node->transaction->category << " " << node->transaction->description << "\n";
        node = node->next;
    }
    file.close();
}

void Transaction_history::makedll(Transaction *transaction)
{
    Transaction_node *temp = new Transaction_node(transaction); // Creating a new node
    temp->next = tail;
    temp->prev = tail->prev;
    (tail->prev)->next = temp;
    (tail->prev) = temp;
}

bool Transaction_history::empty()
{
    return head->next == tail;
}

bool Transaction_history::bringtransactions(std::string username)
{
    std::string directory = "user_transaction_details";
    std::string filename = directory + "/" + username + ".txt";
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string date, category;
    double amount;
    std::string description;

    while (file >> date >> amount >> category)
    {
        // Read the remaining line as the description
        std::getline(file >> std::ws, description); // Read the rest of the line including spaces
        Transaction *temp = new Transaction(date, amount, category, description);
        makedll(temp);
    }
    file.close();
    return true;
}

unordered_map<string, double> Transaction_history::piechartvalues(string username)
{
    std::string directory = "user_transaction_details";
    std::string filename = directory + "/" + username + ".txt";
    std::ifstream file(filename);
    if (!file.is_open())
        return {};

    std::string date, category;
    double amount;
    std::string description;

    while (file >> date >> amount >> category)
    {
        // Read the remaining line as the description
        std::getline(file >> std::ws, description); // Read the rest of the line including spaces
        categoryspend[category] += amount;
    }
    file.close();
    return categoryspend;
}

vector<int> Transaction_history::extract(string currdate)
{
    int date = 0, month = 0, year = 0;
    date = (currdate[8] - '0') * 10 + (currdate[9] - '0');
    month = (currdate[5] - '0') * 10 + (currdate[6] - '0');
    year = (currdate[0] - '0') * 1000 + (currdate[1] - '0') * 100 + (currdate[2] - '0') * 10 + (currdate[3] - '0');
    return {date, month, year};
}