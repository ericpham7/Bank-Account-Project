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
    cout << left << setw(20) << "Current time: ";
    cout << put_time(localTime, "%Y-%m-%d %H:%M:%S") << "\n";
};

void menu() {
    cout << left << setw(31) << "1. Select Account" << '\n';
    cout << left << setw(31) << "2. Create New Account" << '\n';
    cout << left << setw(31) << "3. Deposit" << '\n';
    cout << left << setw(31) << "4. Withdraw" << '\n';
    cout << left << setw(31) << "5. Check Balance" << '\n';
    cout << left << setw(31) << "6. View All Accounts" << '\n';
    cout << left << setw(31) << "7. Exit" << '\n';

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
 