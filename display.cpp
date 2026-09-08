#include <iostream>
#include "display.h"
#include <iomanip>
#include <ctime>
using namespace std;

void displayClock() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    // Display the menu header with a border
    cout << '\n' << setw(26) << setfill('*') << "CHASE";
    cout << setw(20) << setfill('*') << '\n';
    
    // reset fill character to space
    cout << setfill(' '); 

    // Display the current time
    cout << setw(20) << right << "Current time: ";
    cout << put_time(localTime, "%Y-%m-%d %H:%M:%S") << "\n";
};

void menu() {
    cout << setw(31) << right << "1. Select Account" << '\n';
    cout << setw(33) << right << "2. Create New Account" << '\n';
    cout << setw(27) << right << "3. Deposit" << '\n';
    cout << setw(28) << right << "4. Withdraw" << '\n';
    cout << setw(30) << right << "5. Check Balance" << '\n';
    cout << setw(32) << right << "6. View All Accounts" << '\n';
    cout << setw(26) << right << "7. Exit" << '\n';

    cout << setw(45) << setfill('*') << '*'; // End of menu border

    displayKeypad(); // displays keypad
};

void displayKeypad() { // visual keypad
    cout << "\n             +-----+-----+-----+\n";
    cout << "             |  1  |  2  |  3  |\n";
    cout << "             +-----+-----+-----+\n";
    cout << "             |  4  |  5  |  6  |\n";
    cout << "             +-----+-----+-----+\n";
    cout << "             |  7  |  8  |  9  |\n";
    cout << "             +-----+-----+-----+\n";
    cout << "             |  C  |  0  |Enter|\n";
    cout << "             +-----+-----+-----+\n";
};
 