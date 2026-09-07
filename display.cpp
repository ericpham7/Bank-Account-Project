#include <iostream>
#include "display.h"
#include <iomanip>
#include <ctime>
using namespace std;
void displayClock() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    cout << '\n' << setw(20) << "Current time: "
         << put_time(localTime, "%Y-%m-%d %H:%M:%S") << "\n";
};

void menu() {
    cout << "\n" << setw(20) << setfill('*') << '*';
    cout << setfill('*') << "CHASE" << setw(20) << setfill('*') << "*\n";
    cout << setfill(' ');
    cout << right;
    cout << setw(31) << "1. Select Account" << '\n';
    cout << setw(33) << "2. Create New Account" << '\n';
    cout << setw(27) << "3. Deposit" << '\n';
    cout << setw(28) << "4. Withdraw" << '\n';
    cout << setw(30) << "5. Check Balance" << '\n';
    cout << setw(32) << "6. View All Accounts" << '\n';
    cout << setw(26) << "7. Exit" << '\n';
    cout << setw(45) << setfill('*') << '*';

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
 