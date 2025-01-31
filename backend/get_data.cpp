#include "Transhist.h"

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
vector<string> Transaction_history::PrintCategoryWise(string target, string username)
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

// function to get the transactions in a given date range
//  YYYY-MM-DD format
vector<string> Transaction_history::PrintDateRange(string from, string to, string username)
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
        std::stringstream ss;
        ss << "Date: " << curr->transaction->date << ", Amount: " << curr->transaction->amount
           << " INR, Category: " << curr->transaction->category
           << ", Type: " << curr->transaction->transaction_type
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        nothing_printed = 0;
        curr = curr->next;
    }

    if (nothing_printed)
        transactions.push_back("No Transactions Available");
    return transactions;
}