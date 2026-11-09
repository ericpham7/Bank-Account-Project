
#include "bankAccount.h"
#include <cmath>

bankAccount::bankAccount() {
    this->accountNum = 0;
    this->balance = 0.0;
    this->firstName = "";
    this->lastName = "";
    this->pin = "";
    this->expirationDate = "";
}
bankAccount::bankAccount(int accNum, std::string fName, std::string lName, double startBalance) {
    this->accountNum = accNum;
    this->firstName = fName;
    this->lastName = lName;
    this->pin = "";
    this->expirationDate = "";
    this->balance = startBalance;
}

bool bankAccount::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0 || amount > balance) {
        return false;
    }

    balance -= amount;
    return true;
}

bool bankAccount::deposit(double amount) {
    if (!std::isfinite(amount) || amount <= 0) {
        return false;
    }

    this->balance += amount;
    return true;
}

double bankAccount::getBalance() const {
    return balance;
}
