#ifndef GUI_H
#define GUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H> 
#include <FL/Fl_Hold_Browser.H>
#include <bits/stdc++.h>
#include "Transactions.h"
#include "Transhist.h"
#include "Login_system.h"
using namespace std;

class FinanceTrackerGUI {
private:
    Fl_Window* window;
    Fl_Input *dayInput;   
    Fl_Input *monthInput;     
    Fl_Input *yearInput;   
    Fl_Input* amountInput;
    Fl_Choice *categoryChoice;
    Fl_Input* descriptionInput;
    Fl_Hold_Browser* transactionList;
    Fl_Input* nTransactionsInput;   // For last N transactions
    Fl_Choice* categoryFilterChoice;  // For category filter
    Fl_Input *fromdayInput;   
    Fl_Input *frommonthInput;     
    Fl_Input *fromyearInput;
    Fl_Input *todayInput;   
    Fl_Input *tomonthInput;     
    Fl_Input *toyearInput;   
    Fl_Input *usernameInput;
    Fl_Input *passwordInput;
    Fl_Button *loginButton;
    Fl_Button *registerButton;
    Fl_Box *statusBox;
    LoginSystem* loginSystem;
    // static void loginCallback(Fl_Widget* widget, void* data);
    // static void registerCallback(Fl_Widget* widget, void* data);
    void initializeFinanceTracker();
    Transaction_history* history;

public:
    FinanceTrackerGUI();
    static void addTransactionCallback(Fl_Widget* widget, void* data);
    static void printNTransactionsCallback(Fl_Widget* widget, void* data);
    static void printCategoryTransactionsCallback(Fl_Widget* widget, void* data);
    static void printDateRangeTransactionsCallback(Fl_Widget* widget, void* data);
    static void loginCallback(Fl_Widget* widget, void* data);
    static void registerCallback(Fl_Widget* widget, void* data);
    void displayStatus(const char* message, Fl_Color color = FL_WHITE);
    static void quitCallback(Fl_Widget* widget, void* data); // Declare the quit callback function
    void show();
};

#endif // GUI_H
