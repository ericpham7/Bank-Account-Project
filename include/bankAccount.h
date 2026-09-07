#pragma once // to prevent the preprocessor from
#include <string>
using namespace std;
class bankAccount {
public:
    int accountNum;
    std::string firstName, lastName, pin, CVC, expirationDate;

    // Constructor declarations only
    bankAccount();
    bankAccount(int accNum, std::string fName, std::string lName, double startBalance);

    void withdraw(double withdrawAmount);
    void deposit(double depositAmount);
    double getBalance() const;

private:
    double balance;
};
