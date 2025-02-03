#include "Transhist.h"
#include <chrono>
#include <iomanip>

// function to save the budget set by a user
void Transaction_history::setbudget(std::string username, std::unordered_map<std::string, double> &budget)
{
    std::string directory = "user_transaction_details";
    std::string filename = directory + "/" + username + "_budget.txt";

    std::cout << "Opening file: " << filename << std::endl;

    std::unordered_map<std::string, double> oldbudget = getbudget(username);

    // Open the file for writing (overwrites existing content)
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return; // Exit if file can't be opened
    }

    // Write the new budget entries
    for (const auto &it : budget)
    {
        file << it.first << " " << it.second << "\n";
    }

    // Write any old budget entries that aren't in the new budget
    for (const auto &it : oldbudget)
    {
        if (budget.find(it.first) == budget.end())
        {
            file << it.first << " " << it.second << "\n";
        }
    }

    file.close();
    std::cout << "Budget successfully set for user: " << username << std::endl;
}

// function to create the priority queue for recommendation generating
priority_queue<pair<double, string>> Transaction_history::create_recommender(unordered_map<string, double> budget, std::string username)
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm *localTime = std::localtime(&currentTime);
    int month = localTime->tm_mon + 1;
    int year = localTime->tm_year + 1900; 

    priority_queue<pair<double, string>> recommend;
    unordered_map<string, double> spend = recommenderValues(username,month,year);

    // std::cout << "Spending values retrieved for user: " << username << std::endl;
    for (auto it : budget)
    {
        double remaining = spend[it.first] - it.second;
        // std::cout << "Budget for " << it.first << ": " << it.second << ", Spent: " << spend[it.first] << ", Remaining: " << remaining << std::endl;
        recommend.push(make_pair(remaining, it.first));
    }

    return recommend;
}

// function to process the priority queue and generate recommendations for the frontend
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
        if (over > 0)
            recommendations.emplace_back("You have overspent in " + category + " by " + std::to_string(over) + " this month");
        else if (over < 0)
            recommendations.emplace_back("You are close to the budget limit in " + category + " by " + std::to_string(abs(over)) + " for this month");
        else
            recommendations.emplace_back("You have reached the budget limit in " + category + " this month");
        recommender.pop();
    }

    // std::cout << "Recommendations generated: " << recommendations.size() << std::endl;
    return recommendations;
}

// function to retrieve the saved budget of a user
std::unordered_map<std::string, double> Transaction_history::getbudget(std::string username)
{
    std::unordered_map<std::string, double> budget;
    std::string directory = "user_transaction_details";
    std::string filename = directory + "/" + username + "_budget.txt";

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {}; // Returning empty budget if file is not found
    }

    std::string category;
    double budgetamount;
    while (file >> category >> budgetamount)
    {
        budget[category] = budgetamount;
    }

    file.close();
    // std::cout << "Budget loaded for user: " << username << std::endl;
    return budget;
}