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
    categoryspend={
        {"Food",0},
        {"Shopping",0},
        {"Entertainment",0},
        {"Personal",0},
        {"Health",0},
        {"Travel",0},
        {"Miscellaneous",0}
    };
}

void Transaction_history::new_transaction(Transaction *transaction, string username)
{
    Transaction_node *temp = new Transaction_node(transaction); // Creating a new node
    // Ensure correct linking
    temp->next = tail;
    temp->prev = tail->prev;
    (tail->prev)->next = temp;
    (tail->prev) = temp;
    // number_of_transactions++;
    categoryspend[transaction->category]+=transaction->amount;
    std::string directory = "user_transaction_details";
    // Create the directory if it doesn't exist
    #ifdef _WIN32
        _mkdir(directory.c_str()); // For Windows
    #else
        mkdir(directory.c_str(), 0777); // For UNIX-based systems
    #endif

    std::string filename = directory + "/" + username + ".txt";
    std::cout << "opening file: " << filename;
    std::ofstream file(filename, std::ios_base::app); 
    file << transaction->date << " " << transaction->amount << " " << transaction->category << " " << transaction->description << "\n";
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

bool Transaction_history::bringtransactions(string username) {
    string directory = "user_transaction_details";
    string filename = directory + "/" + username + ".txt";
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    string date, category;
    double amount;
    std::string description;

    while (file >> date >> amount >> category) {
        // Read the remaining line as the description
        std::getline(file >> std::ws, description); // Read the rest of the line including spaces
        Transaction *temp = new Transaction(date, amount, category, description);
        makedll(temp);
    }
    file.close();
    return true;
}

unordered_map<string,double> Transaction_history::piechartvalues(string username) {
    string directory = "user_transaction_details";
    string filename = directory + "/" + username + ".txt";
    std::ifstream file(filename);
    if (!file.is_open())
        return {};

    string date, category;
    double amount;
    std::string description;

    while (file >> date >> amount >> category) {
        // Read the remaining line as the description
        std::getline(file >> std::ws, description); // Read the rest of the line including spaces
        categoryspend[category]+=amount;
    }
    file.close();
    return categoryspend;
}


vector<string> Transaction_history::PrintAllTransactions(string username)
{
    vector<string> transactions;
    if (!bringtransactions(username))
    {
        transactions.push_back("No Transactions Available");
        return transactions;
    }
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

// vector<string> Transaction_history::PrintNTransactions(int n)
// {
//     vector<string> transactions;
//     if (empty() || number_of_transactions < n)
//     {
//         transactions.push_back("Invalid Number of Transactions");
//         return transactions;
//     }

//     int start_printing_from = number_of_transactions - n;
//     Transaction_node *curr = head->next;

//     while (start_printing_from)
//     {
//         curr = curr->next;
//         start_printing_from--;
//     }

//     while (curr != tail)
//     {
//         stringstream ss;
//         ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
//            << " INR, Category: " << curr->transaction->category
//            << ", Description: " << curr->transaction->description;
//         transactions.push_back(ss.str());
//         curr = curr->next;
//     }
//     return transactions;
// }

vector<string> Transaction_history::PrintCategoryWise(string target,string username)
{
    vector<string> transactions;
    if (!bringtransactions(username))
    {
        transactions.push_back("No Transactions Available");
        return transactions;
    }
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
    date = (currdate[8] - '0') * 10 + (currdate[9] - '0');
    month = (currdate[5] - '0') * 10 + (currdate[6] - '0');
    year = (currdate[0] - '0') * 1000 + (currdate[1] - '0') * 100 + (currdate[2] - '0') * 10 + (currdate[3] - '0');
    return {date, month, year};
}

// YYYY-MM-DD format
vector<string> Transaction_history::PrintDateRange(string from, string to,string username)
{
    vector<string> transactions;
    if (!bringtransactions(username))
    {
        transactions.push_back("No Transactions Available");
        return transactions;
    }
    if (empty())
    {
        transactions.push_back("No Transactions Available");
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

void Transaction_history::setbudget(string username,unordered_map<string,double>& budget){
    string directory="user_transaction_details";
    string filename = directory + "/" + username + "_budget.txt";
    std::cout << "opening file: " << filename;
    std::ofstream file(filename, std::ios_base::app); 
    for(auto it: budget){
        file << it.first << " " << it.second << "\n";
    }
    file.close();
}

unordered_map<string,double> Transaction_history::getbudget(string username){
    unordered_map<string,double> budget;
    string directory="user_transaction_details";
    string filename = directory + "/" + username + "budget.txt";
    std::cout << "opening file: " << filename;
    std::ifstream file(filename);
    if (!file.is_open())
        return {};
        string category;
        double budgetamount;
    while(file>>category>>budgetamount){
        budget[category]=budgetamount;
    }
    file.close();
    return budget;
}

priority_queue<pair<double,string>> Transaction_history::create_recommender(unordered_map<string,double> budget,string username){
    priority_queue<pair<double,string>> recommend;
    unordered_map<string,double> spend=piechartvalues(username);
    for(auto it: budget){
        recommend.push(make_pair(it.second-spend[it.first],it.first));
    }
}

vector<string> Transaction_history::give_recommendation(string username){
    unordered_map<string,double> budget=getbudget(username);
    priority_queue<pair<double,string>> recommender=create_recommender(budget,username);
    vector<string> recommendations;
    while(!recommender.empty() && recommender.top().first>0){
        double over=recommender.top().first;
        string category=recommender.top().second;
        recommendations.emplace_back("You have overspent in "+category+" by "+to_string(over));
        recommender.pop();
    }
    return recommendations;
}