#include "Transhist.h"

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
           << ", Description: " << curr->transaction->description;
        transactions.push_back(ss.str());
        curr = curr->next;
    }
    return transactions;
}

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

// YYYY-MM-DD format
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
        std::stringstream ss;
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


priority_queue<pair<double, string>> Transaction_history::create_recommender(unordered_map<string, double> budget, std::string username)
{
    priority_queue<pair<double, string>> recommend;
    unordered_map<string, double> spend = piechartvalues(username); // Ensure this function is defined and works correctly

    // std::cout << "Spending values retrieved for user: " << username << std::endl;
    for (auto it : budget)
    {
        double remaining = spend[it.first] - it.second;
        // std::cout << "Budget for " << it.first << ": " << it.second << ", Spent: " << spend[it.first] << ", Remaining: " << remaining << std::endl;
        recommend.push(make_pair(remaining, it.first));
    }

    return recommend;
}


vector<string> Transaction_history::give_recommendation(std::string username)
{
    // std::cout << "Function called " << std::endl;
    unordered_map<string, double> budget = getbudget(username);

    if (budget.empty())
    {
        std::cerr << "No budget found for user: " << username << endl;
        return {"No budget data available."};
    }

    // std::cout << "Budget retrieved: " << budget.size() << " categories" << std::endl;

    priority_queue<pair<double, string>> recommender = create_recommender(budget, username);
    // std::cout << "Recommender created with " << recommender.size() << " entries" << std::endl;

    vector<string> recommendations;
    while (!recommender.empty() && recommender.top().first > -100)
    {
        double over = recommender.top().first;
        std::string category = recommender.top().second;
        if(over>0) recommendations.emplace_back("You have overspent in " + category + " by " + std::to_string(over));
        else if(over<0) recommendations.emplace_back("You are close to the budget limit in " + category + " by " + std::to_string(abs(over)));
        else recommendations.emplace_back("You have reached the budeget limit in " + category);
        recommender.pop();
    }

    // std::cout << "Recommendations generated: " << recommendations.size() << std::endl;
    return recommendations;
}
