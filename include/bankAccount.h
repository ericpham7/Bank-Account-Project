#pragma once

#include <string>

class bankAccount {
public:
    int accountNum;
    std::string firstName;
    std::string lastName;
    std::string pin;
    std::string CVC;
    std::string expirationDate;

    bankAccount();
    bankAccount(int accNum, std::string fName, std::string lName, double startBalance);

    bool withdraw(double withdrawAmount);
    bool deposit(double depositAmount);
    double getBalance() const;

private:
    double balance;
};
