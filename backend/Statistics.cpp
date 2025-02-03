#include "Transhist.h"

// function to get current month and year
pair<int, int> Transaction_history::getCurrentMonthYear()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);
    return make_pair(now_tm->tm_mon + 1, now_tm->tm_year + 1900);
}

// function to get the total number of days in a month
int Transaction_history::getDaysInMonth(int month, int year)
{
    std::tm time_in = {0, 0, 0, 1, month - 1, year - 1900};   // Initialize to the 1st day of the given month and year
    std::time_t next_month = std::mktime(&time_in);           // Convert to time_t
    time_in.tm_mon += 1;                                      // Move to the next month
    std::time_t end_of_month = std::mktime(&time_in) - 86400; // Backtrack 1 day (86400 seconds)
    std::tm *last_day = std::localtime(&end_of_month);        // Get the last day of the month
    return last_day->tm_mday;                                 // Return day of month (which will be the last day)
}

// KMP preprocessing to create the lps array
std::vector<int> Transaction_history::computeLPSArray(std::string &pattern)
{
    int m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// KMP search function
int Transaction_history::KMPSearch(std::string &text, std::string &pattern)
{
    int n = text.size();
    int m = pattern.size();
    std::vector<int> lps = computeLPSArray(pattern);
    int i = 0; // index for text
    int j = 0; // index for pattern

    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == m)
        {
            return i - j; // Match found at index (i - j)
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    return -1; // No match found
}

// Function to extract amount from the transaction string
double Transaction_history::extractAmountHelper(std::string &transaction)
{
    std::string pattern = "Amount: ";
    std::string transaction_type_pattern = "Type: ";
    int pos = KMPSearch(transaction, pattern);
    int typ_pos = KMPSearch(transaction, transaction_type_pattern);

    if (typ_pos != -1)
    {
        typ_pos += transaction_type_pattern.size();
        // Skip any whitespace after the marker.
        while (typ_pos < transaction.size() && std::isspace(static_cast<unsigned char>(transaction[typ_pos])))
        {
            typ_pos++;
        }
        char transType = transaction[typ_pos];
        if (transType == 'C')
        {
            return 0.0; // Ignore amount for Credit transactions
        }
    }
    else
    {
        std::cout << "[DEBUG] Transaction type marker not found.\n";
    }

    if (pos != -1)
    {
        pos += pattern.size(); // Move index to the start of the number
        // Extract numeric value
        std::string amountStr;
        while (pos < transaction.size() && (std::isdigit(transaction[pos]) || transaction[pos] == '.'))
        {
            amountStr += transaction[pos];
            pos++;
        }
        return !amountStr.empty() ? std::stod(amountStr) : 0.0;
    }
    return 0.0; // Return 0 if "Amount:" not found or no valid number follows
}

vector<double> Transaction_history::extract_amount(vector<std::string> &transaction_data)
{
    std::vector<double> amounts;

    for (auto &transaction : transaction_data)
    {
        amounts.emplace_back(extractAmountHelper(transaction));
    }

    return amounts;
}

// function to get total money spent
double Transaction_history::getTotalSpend(vector<std::string> &transaction_data)
{
    vector<double> amounts = extract_amount(transaction_data);
    double total = 0.0;
    for (auto it : amounts)
    {
        total += it;
    }
    return total;
}

// function to get standard deviation of a month
double Transaction_history::getSD(vector<std::string> &transaction_data)
{
    vector<double> amount = extract_amount(transaction_data);
    const double avg = getTotalSpend(transaction_data) / transaction_data.size();
    double squareDiffSum = 0.0;
    for (auto it : amount)
    {
        squareDiffSum += ((it - avg) * (it - avg));
    }
    return std::sqrt(squareDiffSum / transaction_data.size());
}

// function to get money spent in each category in a month
unordered_map<std::string, double> Transaction_history::extract_categories(vector<std::string> &transaction_data)
{
    unordered_map<std::string, double> category_spend;
    std::string pattern = "Category: ";
    vector<double> amount = extract_amount(transaction_data);
    int curr_amount = 0;
    for (auto it : transaction_data)
    {
        int pos = KMPSearch(it, pattern);
        pos += pattern.size();
        std::string curr_cat = "";
        while (it[pos] != ',')
            curr_cat += it[pos++];
        category_spend[curr_cat] += amount[curr_amount++];
    }
    return category_spend;
}

// function to get monthly statistics for the "monthly stats" tab
unordered_map<string, double> Transaction_history::monthlysummary(int month, int year, string username)
{
    unordered_map<string, double> stats;
    std::string str_month = "";
    if (month <= 9)
        str_month = "0" + to_string(month);
    else
        str_month = to_string(month);
    std::string from = to_string(year) + "-" + str_month + "-" + "01";
    std::string to = to_string(year) + "-" + str_month + "-" + to_string(getDaysInMonth(month, year));
    vector<std::string> transaction_data = PrintFiltered("", "", from, to, username);
    if (transaction_data[0] == "No Transactions Available")
        return stats;
    stats["Total_Spend"] = getTotalSpend(transaction_data);
    stats["Month_Average"] = stats["Total_Spend"] / transaction_data.size();
    stats["Month_Standard_Deviation"] = getSD(transaction_data);
    unordered_map<std::string, double> categoryData = extract_categories(transaction_data);
    for (auto it : categoryData)
    {
        stats[it.first] = it.second;
    }
    return stats;
}