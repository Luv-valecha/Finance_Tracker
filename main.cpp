#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <iostream>
#include "GUI.h" // Include your GUI header

int main() {
    // Create the main window for the Finance Tracker application
    // You can create an instance of your GUI class
    FinanceTrackerGUI gui; // Assuming your GUI class constructor sets up the window
    gui.show(); // Show the GUI window

    std::cout << "Starting Finance Tracker GUI..." << std::endl;

    // Run the FLTK event loop
    return Fl::run(); // This will enter the FLTK event loop
}
