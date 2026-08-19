#include <iostream>
#include <vector>
#include "bankAccount.h"
using namespace std;

void menu();

void showAllAccounts(const vector<bankAccount>& accounts) {
    if (accounts.empty()) {
        cout << "No accounts available.\n";
        return;
    }

    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << "Account " << i + 1 << ": "
             << accounts[i].firstName << " " << accounts[i].lastName
             << " | Account #" << accounts[i].accountNum
             << " | Balance: $" << accounts[i].getBalance() << "\n";
    }
}

int main() {
    int choice;
    vector<bankAccount> accounts;
    int selectedAccountIndex = -1;

    while (true) {
        menu();
        cout << "Enter your choice:";
        cin >> choice;

        if (choice == 1) {
            if (accounts.empty()) {
                cout << "No accounts created yet.\n";
                continue;
            }

            int accountNum;
            cout << "Enter account number: ";
            cin >> accountNum;

            bool found = false;
            for (size_t i = 0; i < accounts.size(); ++i) {
                if (accounts[i].accountNum == accountNum) {
                    selectedAccountIndex = static_cast<int>(i);
                    found = true;
                    cout << "Account selected: " << accounts[selectedAccountIndex].firstName
                         << " " << accounts[selectedAccountIndex].lastName << "\n";
                    break;
                }
            }

            if (!found) {
                cout << "Account not found.\n";
            }
        }
        else if (choice == 2) {
            int newAccountNum;
            string newFirstName, newLastName;
            double startingBalance;

            cout << "Enter account number: ";
            cin >> newAccountNum;
            cout << "Enter first name: ";
            cin >> newFirstName;
            cout << "Enter last name: ";
            cin >> newLastName;
            cout << "Enter starting balance: $";
            cin >> startingBalance;

            accounts.push_back(bankAccount(newAccountNum, newFirstName, newLastName, startingBalance));
            selectedAccountIndex = static_cast<int>(accounts.size()) - 1;

            cout << "New account created successfully!\n";
            cout << "Account Holder: " << accounts[selectedAccountIndex].firstName << " "
                 << accounts[selectedAccountIndex].lastName << "\n";
            cout << "Account Number: " << accounts[selectedAccountIndex].accountNum << "\n";
            cout << "Starting Balance: $" << accounts[selectedAccountIndex].getBalance() << "\n";
        }
        else if (choice == 3) {
            if (accounts.empty() || selectedAccountIndex == -1) {
                cout << "Please select or create an account first.\n";
                continue;
            }

            double depositAmount;
            cout << "Enter a deposit Amount: ";
            cin >> depositAmount;
            accounts[selectedAccountIndex].deposit(depositAmount);
        }
        else if (choice == 4) {
            if (accounts.empty() || selectedAccountIndex == -1) {
                cout << "Please select or create an account first.\n";
                continue;
            }

            double withdrawAmount;
            cout << "Enter a withdraw Amount: ";
            cin >> withdrawAmount;
            accounts[selectedAccountIndex].withdraw(withdrawAmount);
        }
        else if (choice == 5) {
            if (accounts.empty() || selectedAccountIndex == -1) {
                cout << "Please select or create an account first.\n";
                continue;
            }

            cout << "Current Balance: $" << accounts[selectedAccountIndex].getBalance() << endl;
        }
        else if (choice == 6) {
            showAllAccounts(accounts);
        }
        else if (choice == 7) {
            cout << "Thanks for using Chase Bank!\n";
            break;
        }
        else {
            cout << "Invalid Input! please try again!\n";
        }
    }

    return 0;
}

void menu() {
    cout << "\n1. Select Account\n";
    cout << "2. Create New Account\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Check Balance\n";
    cout << "6. View All Accounts\n";
    cout << "7. Exit\n";
}
