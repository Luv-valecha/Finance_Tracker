#include "GUI.h"
using namespace std;

// Helper function to validate the date
bool isValidDate(int day, int month, int year)
{
    if (month < 1 || month > 12)
        return false;

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (month == 2)
    {
        bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (is_leap)
        {
            days_in_month[1] = 29;
        }
    }

    return day > 0 && day <= days_in_month[month - 1];
}

// Constructor
FinanceTrackerGUI::FinanceTrackerGUI()
{
    history = new Transaction_history();
    window = new Fl_Window(1800, 800, "Personal Finance Tracker");
    window->color(fl_rgb_color(50, 50, 50)); // Dark gray background

    // Add a heading
    Fl_Box *headingBox = new Fl_Box(0, 10, 1800, 70, "Personal Finance Tracker");
    headingBox->labelsize(36);                   // Set font size
    headingBox->labelcolor(FL_WHITE);            // Set label color to white
    headingBox->color(fl_rgb_color(30, 30, 30)); // Dark background
    headingBox->align(FL_ALIGN_CENTER);          // Center the text
    headingBox->box(FL_UP_BOX);                  // Add a border to the heading box

    // Inputs for day, month, year
    dayInput = new Fl_Input(200, 90, 60, 35, "Day:");
    dayInput->box(FL_BORDER_BOX);
    dayInput->color(fl_rgb_color(240, 240, 240)); // Input background
    dayInput->labelcolor(FL_WHITE);               // White text

    monthInput = new Fl_Input(320, 90, 60, 35, "Month:");
    monthInput->box(FL_BORDER_BOX);
    monthInput->color(fl_rgb_color(240, 240, 240)); // Input background
    monthInput->labelcolor(FL_WHITE);               // White text

    yearInput = new Fl_Input(430, 90, 100, 35, "Year:");
    yearInput->box(FL_BORDER_BOX);
    yearInput->color(fl_rgb_color(240, 240, 240)); // Input background
    yearInput->labelcolor(FL_WHITE);               // White text

    amountInput = new Fl_Input(200, 140, 200, 30, "Amount:");
    amountInput->box(FL_BORDER_BOX);
    amountInput->color(fl_rgb_color(240, 240, 240));
    amountInput->labelcolor(FL_WHITE);

    // Change category input to dropdown
    categoryChoice = new Fl_Choice(200, 190, 200, 30, "Category:");
    categoryChoice->box(FL_BORDER_BOX);
    categoryChoice->color(fl_rgb_color(240, 240, 240));
    categoryChoice->labelcolor(FL_WHITE);

    // Add categories to the choice menu
    categoryChoice->add("Food");
    categoryChoice->add("Transport");
    categoryChoice->add("Utilities");
    categoryChoice->add("Shopping");
    categoryChoice->add("Entertainment");
    categoryChoice->add("Other");

    descriptionInput = new Fl_Input(200, 240, 200, 30, "Description:");
    descriptionInput->box(FL_BORDER_BOX);
    descriptionInput->color(fl_rgb_color(240, 240, 240));
    descriptionInput->labelcolor(FL_WHITE);

    // Button to add transaction
    Fl_Button *addButton = new Fl_Button(200, 290, 200, 30, "Add Transaction");
    addButton->callback(addTransactionCallback, this);
    addButton->color(fl_rgb_color(0, 150, 0)); // Dark green button
    addButton->labelcolor(FL_WHITE);           // White text color

    // Button to quit the application
    Fl_Button *quitButton = new Fl_Button(420, 290, 100, 30, "Quit");
    quitButton->callback(quitCallback, this);
    quitButton->color(fl_rgb_color(200, 0, 0)); // Dark red button
    quitButton->labelcolor(FL_WHITE);           // White text color

    // Transaction List
    transactionList = new Fl_Hold_Browser(850, 90, 800, 500);
    transactionList->type(FL_HOLD_BROWSER);
    transactionList->box(FL_BORDER_BOX);
    transactionList->color(fl_rgb_color(230, 230, 230)); // Dark gray for transaction list
    transactionList->labelcolor(FL_WHITE);               // White text for the transaction list

    // Input and Button for Last N Transactions
    nTransactionsInput = new Fl_Input(200, 340, 250, 30, "Last N Transactions:");
    nTransactionsInput->box(FL_BORDER_BOX);
    nTransactionsInput->color(fl_rgb_color(240, 240, 240));
    nTransactionsInput->labelcolor(FL_WHITE);

    Fl_Button *printNButton = new Fl_Button(200, 390, 250, 30, "Show Last N Transactions");
    printNButton->callback(printNTransactionsCallback, this);
    printNButton->color(fl_rgb_color(0, 102, 204)); // Dark blue button
    printNButton->labelcolor(FL_WHITE);

    // Input and Button for Category Filter
    categoryFilterChoice = new Fl_Choice(200, 440, 250, 30, "Category Filter:");
    categoryFilterChoice->box(FL_BORDER_BOX);
    categoryFilterChoice->color(fl_rgb_color(240, 240, 240));
    categoryFilterChoice->labelcolor(FL_WHITE);

    categoryFilterChoice->add("Food");
    categoryFilterChoice->add("Transport");
    categoryFilterChoice->add("Utilities");
    categoryFilterChoice->add("Shopping");
    categoryFilterChoice->add("Entertainment");
    categoryFilterChoice->add("Other");

    Fl_Button *categoryButton = new Fl_Button(200, 490, 250, 30, "Show Category Transactions");
    categoryButton->callback(printCategoryTransactionsCallback, this);
    categoryButton->color(fl_rgb_color(0, 102, 204)); // Dark blue button
    categoryButton->labelcolor(FL_WHITE);

    // Input and Button for Date Range
    fromdayInput = new Fl_Input(200, 540, 60, 35, "Day:");
    fromdayInput->box(FL_BORDER_BOX);
    fromdayInput->color(fl_rgb_color(240, 240, 240)); // Input background
    fromdayInput->labelcolor(FL_WHITE);               // White text

    frommonthInput = new Fl_Input(320, 540, 60, 35, "Month:");
    frommonthInput->box(FL_BORDER_BOX);
    frommonthInput->color(fl_rgb_color(240, 240, 240)); // Input background
    frommonthInput->labelcolor(FL_WHITE);               // White text

    fromyearInput = new Fl_Input(430, 540, 100, 35, "Year:");
    fromyearInput->box(FL_BORDER_BOX);
    fromyearInput->color(fl_rgb_color(240, 240, 240)); // Input background
    fromyearInput->labelcolor(FL_WHITE);               // White text

    todayInput = new Fl_Input(200, 580, 60, 35, "Day:");
    todayInput->box(FL_BORDER_BOX);
    todayInput->color(fl_rgb_color(240, 240, 240)); // Input background
    todayInput->labelcolor(FL_WHITE);               // White text

    tomonthInput = new Fl_Input(320, 580, 60, 35, "Month:");
    tomonthInput->box(FL_BORDER_BOX);
    tomonthInput->color(fl_rgb_color(240, 240, 240)); // Input background
    tomonthInput->labelcolor(FL_WHITE);               // White text

    toyearInput = new Fl_Input(430, 580, 100, 35, "Year:");
    toyearInput->box(FL_BORDER_BOX);
    toyearInput->color(fl_rgb_color(240, 240, 240)); // Input background
    toyearInput->labelcolor(FL_WHITE);               // White text

    Fl_Button *dateRangeButton = new Fl_Button(200, 620, 250, 30, "Show Transactions by Date Range");
    dateRangeButton->callback(printDateRangeTransactionsCallback, this);
    dateRangeButton->color(fl_rgb_color(0, 102, 204)); // Dark blue button
    dateRangeButton->labelcolor(FL_WHITE);

    window->end();
    window->show();
}

// Callback to add a transaction
void FinanceTrackerGUI::addTransactionCallback(Fl_Widget *widget, void *data)
{
    FinanceTrackerGUI *gui = static_cast<FinanceTrackerGUI *>(data);

    // Extract day, month, and year from inputs
    int day = 0, month = 0, year = 0;
    try
    {
        day = std::stoi(gui->dayInput->value());
        month = std::stoi(gui->monthInput->value());
        year = std::stoi(gui->yearInput->value());
    }
    catch (...)
    {
        gui->transactionList->add("Invalid date input!"); // Add error message to the transaction list
        return;
    }

    if (year < 1000 || year > 9999)
    {
        gui->transactionList->add("Invalid year! Year should be a 4-digit number.");
        return;
    }
    // Validate the date
    if (!isValidDate(day, month, year))
    {
        gui->transactionList->add("Invalid date!"); // Add error message to the transaction list
        return;
    }

    // Format the date as DD-MM-YYYY
    std::ostringstream dateStream;
    dateStream << (day < 10 ? "0" : "") << day << "-"
               << (month < 10 ? "0" : "") << month << "-"
               << year;
    std::string date = dateStream.str();

    // Handle amount input
    double amount = 0.0;
    try
    {
        amount = std::stod(gui->amountInput->value());
    }
    catch (...)
    {
        gui->transactionList->add("Invalid amount input!"); // Add error message to the transaction list
        return;
    }

    std::string category = gui->categoryChoice->text(gui->categoryChoice->value());
    std::string description = gui->descriptionInput->value();

    // Create a new Transaction object
    Transaction *newTransaction = new Transaction(date, amount, category, description);
    if (newTransaction == nullptr)
    {
        gui->transactionList->add("Failed to create new Transaction!"); // Add error message to the transaction list
        return;
    }

    // Add the new transaction to history
    gui->history->new_transaction(newTransaction);
    gui->transactionList->add(("Transaction added for date: " + date).c_str()); // Add success message to the transaction list
}

// Callback to print last N transactions
void FinanceTrackerGUI::printNTransactionsCallback(Fl_Widget *widget, void *data)
{
    FinanceTrackerGUI *gui = static_cast<FinanceTrackerGUI *>(data);
    int n = std::stoi(gui->nTransactionsInput->value());

    // Clear the list first
    gui->transactionList->clear();
    // Get and print the last N transactions
    std::vector<std::string> transactions = gui->history->PrintNTransactions(n);
    for (const auto &transaction : transactions)
    {
        gui->transactionList->add(transaction.c_str());
    }
}

// Callback to print transactions by category
void FinanceTrackerGUI::printCategoryTransactionsCallback(Fl_Widget *widget, void *data)
{
    FinanceTrackerGUI *gui = static_cast<FinanceTrackerGUI *>(data);
    std::string category = gui->categoryFilterChoice->text(gui->categoryFilterChoice->value());

    // Clear the list first
    gui->transactionList->clear();
    // Get and print transactions by category
    std::vector<std::string> transactions = gui->history->PrintCategoryWise(category);
    for (const auto &transaction : transactions)
    {
        gui->transactionList->add(transaction.c_str());
    }
}

// Callback to print transactions by date range
void FinanceTrackerGUI::printDateRangeTransactionsCallback(Fl_Widget *widget, void *data)
{
    FinanceTrackerGUI *gui = static_cast<FinanceTrackerGUI *>(data);
    // Extract day, month, and year from inputs
    int fromday = 0, frommonth = 0, fromyear = 0;
    try
    {
        fromday = std::stoi(gui->fromdayInput->value());
        frommonth = std::stoi(gui->frommonthInput->value());
        fromyear = std::stoi(gui->fromyearInput->value());
    }
    catch (...)
    {
        gui->transactionList->add("Invalid date input!"); // Add error message to the transaction list
        return;
    }

    if (fromyear < 1000 || fromyear > 9999)
    {
        gui->transactionList->add("Invalid year! Year should be a 4-digit number.");
        return;
    }
    // Validate the date
    if (!isValidDate(fromday, frommonth, fromyear))
    {
        gui->transactionList->add("Invalid date!"); // Add error message to the transaction list
        return;
    }

    // Format the date as DD-MM-YYYY
    std::ostringstream fromdateStream;
    fromdateStream << (fromday < 10 ? "0" : "") << fromday << "-"
               << (frommonth < 10 ? "0" : "") << frommonth << "-"
               << fromyear;
    std::string from = fromdateStream.str();

    // Extract day, month, and year from inputs
    int today = 0, tomonth = 0, toyear = 0;
    try
    {
        today = std::stoi(gui->todayInput->value());
        tomonth = std::stoi(gui->tomonthInput->value());
        toyear = std::stoi(gui->toyearInput->value());
    }
    catch (...)
    {
        gui->transactionList->add("Invalid date input!"); // Add error message to the transaction list
        return;
    }

    if (toyear < 1000 || toyear > 9999)
    {
        gui->transactionList->add("Invalid year! Year should be a 4-digit number.");
        return;
    }
    // Validate the date
    if (!isValidDate(today, tomonth, toyear))
    {
        gui->transactionList->add("Invalid date!"); // Add error message to the transaction list
        return;
    }

    // Format the date as DD-MM-YYYY
    std::ostringstream todateStream;
    todateStream << (today < 10 ? "0" : "") << today << "-"
               << (tomonth < 10 ? "0" : "") << tomonth << "-"
               << toyear;
    std::string to = todateStream.str();
    // Clear the list first
    gui->transactionList->clear();
    // Get and print transactions by date range
    vector<string> transactions = gui->history->PrintDateRange(from, to);
    for (const auto &transaction : transactions)
    {
        gui->transactionList->add(transaction.c_str());
    }
}

// Callback to quit the application
void FinanceTrackerGUI::quitCallback(Fl_Widget *widget, void *data)
{
    FinanceTrackerGUI *gui = static_cast<FinanceTrackerGUI *>(data);
    Fl::delete_widget(gui->window); // Correct way to close the application
}

void FinanceTrackerGUI::show()
{
    Fl::run();
}
