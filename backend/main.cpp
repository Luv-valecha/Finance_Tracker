#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Login_system.h"
#include "Transactions.h"
#include "Transhist.h"

using namespace std;

//                                                 Main file connecting frontend and backend

LoginSystem loginSystem;
Transaction_history transactionHistory;

void printUsage()
{
    cout << "Usage:" << endl;
    cout << "  register <username> <password>" << endl;
    cout << "  login <username> <password>" << endl;
    cout << "  add_transaction <date> <amount> <category> <description>" << endl;
    cout << "  get_transactions" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    string command = argv[1];

    if (command == "register")
    {
        if (argc != 4)
        {
            cout << "Invalid number of arguments for register" << endl;
            return 1;
        }
        string username = argv[2];
        string password = argv[3];
        if (loginSystem.registerUser(username, password))
        {
            cout << "User registered successfully" << endl;
        }
        else
        {
            cout << "Failed to register user" << endl;
        }
    }
    else if (command == "login")
    {
        if (argc != 4)
        {
            cout << "Invalid number of arguments for login" << endl;
            return 1;
        }
        string username = argv[2];
        string password = argv[3];
        if (loginSystem.loginUser(username, password))
        {
            cout << "Login successful" << endl;
        }
        else
        {
            cout << "Login failed" << endl;
        }
    }
    else if (command == "add_transaction")
    {
        if (argc != 7)
        {
            cout << "Invalid number of arguments for add_transaction" << endl;
            return 1;
        }
        string username = argv[2];
        string date = argv[3];
        double amount = stod(argv[4]);
        string category = argv[5];
        string description = argv[6];
        Transaction *transaction = new Transaction(date, amount, category, description);
        transactionHistory.new_transaction(transaction, username);
        cout << "Transaction added successfully" << endl;
    }
    else if (command == "get_transactions")
    {
        if (argc != 3)
        {
            cout << "Invalid number of arguments for get_transaction" << endl;
            return 1;
        }
        string username = argv[2];
        vector<string> transactions = transactionHistory.PrintAllTransactions(username);
        for (const auto &transaction : transactions)
        {
            cout << transaction << "\n";
        }
    }
    else if (command == "get_category_transactions")
    {
        if (argc != 4)
        {
            cout << "Invalid number of arguments for get_category_transaction" << endl;
            return 1;
        }
        string username = argv[2];
        string targetcat = argv[3];
        vector<string> transactions = transactionHistory.PrintCategoryWise(targetcat, username);
        for (const auto &transaction : transactions)
        {
            cout << transaction << "\n";
        }
    }
    else if (command == "get_date_transactions")
    {
        if (argc != 5)
        {
            cout << "Invalid number of arguments for get_date_transactions" << endl;
            return 1;
        }
        string username = argv[2];
        string from = argv[3];
        string to = argv[4];
        vector<string> transactions = transactionHistory.PrintDateRange(from, to, username);
        for (const auto &transaction : transactions)
        {
            cout << transaction << "\n";
        }
    }
    else if (command == "get_piechart_values")
    {
        if (argc != 3)
        {
            cout << "Invalid number of arguments for get_piechart_values" << endl;
            return 1;
        }
        string username = argv[2];
        unordered_map<string, double> piechartvals = transactionHistory.piechartvalues(username);
        for (const auto &it : piechartvals)
        {
            cout << it.first << " " << it.second << endl;
        }
    }
    else if (command == "set_budget")
    {
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " set_budget <username> <category1> <amount1> [<category2> <amount2> ...]\n";
            return 1;
        }

        std::string username = argv[2];
        std::unordered_map<std::string, double> budget;

        for (int i = 3; i < argc; i += 2)
        {
            std::string category = argv[i];
            double amount = std::stod(argv[i + 1]);
            budget[category] = amount;
        }

        transactionHistory.setbudget(username, budget);
        std::cout << "Budget set successfully for user: " << username << "\n";
    }
    else if (command == "fetch_recommendation")
    {
        // std::cout<<"Command started";
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " fetch_recommendation <username>";
            return 1;
        }

        std::string username = argv[2];
        // std::cout << "Fetching recommendations for: " << username << std::endl;
        // Ensure the function to get recommendations is correctly called
        vector<string> recommendations = transactionHistory.give_recommendation(username);
        // std::cout<<"main test1 ";
        for (const auto &it : recommendations)
        {
            std::cout << it << endl;
        }
    }
    else if (command == "get_statistics")
    {
        // std::cout<<"Command started";
        if (argc != 5)
        {
            std::cerr << "Usage: " << argv[0] << " fetch_recommendation <username> <month> <year>";
            return 1;
        }

        std::string username = argv[2];
        int month = stoi(argv[3]);
        int year = stoi(argv[4]);
        unordered_map<std::string, double> stats = transactionHistory.monthlysummary(month, year, username);
        for (const auto &it : stats)
        {
            std::cout << it.first << " " << it.second << endl;
        }
    }

    else
    {
        cout << "Unknown command: " << command << endl;
        printUsage();
        return 1;
    }

    return 0;
}