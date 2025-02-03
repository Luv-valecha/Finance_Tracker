#include "Transhist.h"
#include <fstream>

// function to display all the transactions by a user
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
        std::stringstream ss;
        ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
           << " INR, Category: " << curr->transaction->category
           << ", Type: " << curr->transaction->transaction_type
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        curr = curr->next;
    }
    return transactions;
}

// functions to get all transactions in a particular category
vector<string> Transaction_history::PrintFiltered(string targetcat, string targettype, string from, string to, string username)
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
        // Date Range Checks

        if (from != "" && to != "")
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
            if (credentials[2] == from_credentials[2] && credentials[1] < from_credentials[1])
            {
                curr = curr->next;
                continue;
            }
            if (credentials[2] == from_credentials[2] && credentials[1] == from_credentials[1] && credentials[0] < from_credentials[0])
            {
                curr = curr->next;
                continue;
            }
            if (credentials[2] > to_credentials[2])
                break;
            if (credentials[2] == to_credentials[2] && credentials[1] > to_credentials[1])
                break;
            if (credentials[2] == to_credentials[2] && credentials[1] == to_credentials[1] && credentials[0] > to_credentials[0])
                break;
        }

        if ((targetcat == "" || curr->transaction->category == targetcat) && (targettype == "" || curr->transaction->transaction_type == targettype))
        {
            std::stringstream ss;
            ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
               << " INR, Category: " << curr->transaction->category
               << ", Type: " << curr->transaction->transaction_type
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

void processfile(std::string filename, vector<string> &alldata)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string date, category, transaction_type, description;
    double amount;

    string line;
    while (std::getline(file, line))
    {
        alldata.emplace_back(line);
    }
    file.close();
}

void getallusernames(vector<string>& usernames){
    std::ifstream file("user_data.txt");
    if(!file.is_open()){
        std::cerr << "Failed to open file: user_data.txt in getallusernames function" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) { 
        std::istringstream stream(line);
        std::string word;
        std::vector<std::string> words;

        // Read all words from the line
        while (stream >> word) {
            words.push_back(word);
        }

        if (words.size() > 1) {
            std::string combinedString="";
            for (size_t i = 0; i < words.size() - 1; ++i) {
                combinedString += words[i] + " ";
            }
            // Remove the trailing space
            combinedString.pop_back();
            usernames.push_back(combinedString);
        }
    }

    file.close();
}

void Transaction_history::fetchAllData(vector<string> &alldata)
{

    vector<string> usernames;
    getallusernames(usernames);

    for(auto username : usernames){
        std::string filename="user_transaction_details/"+username+".txt";
        processfile(filename,alldata);
    }
}

// function to get the transactions in a given date range
//  YYYY-MM-DD format
// vector<string> Transaction_history::PrintDateRange(string from, string to, string username)
// {
//     vector<string> transactions;
//     if (!bringtransactions(username))
//     {
//         transactions.push_back("No Transactions Available");
//         return transactions;
//     }
//     if (empty())
//     {
//         transactions.push_back("No Transactions Available");
//         return transactions;
//     }

//     int nothing_printed = 1;
//     Transaction_node *curr = head->next;
//     while (curr != tail)
//     {
//         string currdate = curr->transaction->date;
//         vector<int> credentials = extract(currdate);
//         vector<int> from_credentials = extract(from);
//         vector<int> to_credentials = extract(to);
//         if (credentials[2] < from_credentials[2])
//         {
//             curr = curr->next;
//             continue;
//         }
//         if (credentials[2] == from_credentials[2] && credentials[1] < from_credentials[1])
//         {
//             curr = curr->next;
//             continue;
//         }
//         if (credentials[2] == from_credentials[2] && credentials[1] == from_credentials[1] && credentials[0] < from_credentials[0])
//         {
//             curr = curr->next;
//             continue;
//         }
//         if (credentials[2] > to_credentials[2])
//             break;
//         if (credentials[2] == to_credentials[2] && credentials[1] > to_credentials[1])
//             break;
//         if (credentials[2] == to_credentials[2] && credentials[1] == to_credentials[1] && credentials[0] > to_credentials[0])
//             break;
//         std::stringstream ss;
//         ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
//            << " INR, Category: " << curr->transaction->category
//            << ", Type: " << curr->transaction->transaction_type
//            << ", Description: " << curr->transaction->description;
//         transactions.push_back(ss.str());
//         nothing_printed = 0;
//         curr = curr->next;
//     }

//     if (nothing_printed)
//         transactions.push_back("No Transactions Available");
//     return transactions;
// }