
#include "bankAccount.h"
#include <iostream>
using namespace std;
bankAccount::bankAccount() {
    this->accountNum = 0;
    this->balance = 0.0;
    this->firstName = "";
    this->lastName = "";
    this->pin = "";
    this->expirationDate = "";
}
bankAccount::bankAccount(int accNum, string fName, std::string lName, double startBalance) {
    this->accountNum = accNum;
    this->firstName = fName;
    this->lastName = lName;
    this->pin = "";
    this->expirationDate = "";
    this->balance = startBalance;
}

void bankAccount::withdraw(double amount) {
    if (amount <= 0) {
        cout << "Invalid amount.\n";
    }
    else if (amount > balance) {
        cout << "Insufficient funds. Please try again.\n";
    }
    else {
        balance -= amount;
        cout << "Withdraw: $" << amount << '\n';
        cout << "Balance Updated: $" << balance << '\n';
    }
}
void bankAccount::deposit(double amount) {
    if (amount <= 0) {
        cout << "Invalid amount.\n";
    }
    else {
        this->balance += amount;
        cout << "New Balance: $" << this->balance << '\n';
    }
}
double bankAccount::getBalance() const {
    return balance;
}
