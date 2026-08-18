#include <iostream>

using namespace std;

class bankAccount{
    public:
        int accountNum;
        string firstName, lastName, pin, CVC, expirationDate;

        bankAccount() : balance(0.0), accountNum(0), firstName(""), lastName("") {}

        bankAccount(int accNum, string fName, string lName, double startBalance)
            : accountNum(accNum), firstName(fName), lastName(lName), balance(startBalance) {}

        void withdraw(double withdrawAmount){
            if (withdrawAmount <= 0){
                cout << "Invalid amount.\n";
            }
            else if (withdrawAmount > balance){
                cout << "Insufficient funds. Please try again.\n";
            }
            else{
                balance -= withdrawAmount;
                cout << "Withdraw: $" << withdrawAmount << '\n';
                cout << "Balance Updated: $" << balance << '\n';
            }
        }

        void deposit(double depositAmount){
            if(depositAmount > 0){
                this->balance += depositAmount;
                cout << "Deposited: $" << depositAmount << endl;
                cout << "Balance Updated: $" << this->balance << endl;
            }
            else{
                cout << "Insufficient Amount Entered\n";
            }
        }

        double getBalance(){
            return this->balance;
        }

    private:
        double balance;
};

void menu();

int main(){
    int choice;
    bankAccount obj;

    while (true){
        menu();
        cin >> choice;

        if(choice == 1){
            double withdrawAmount;
            cout << "Enter a withdraw Amount:";
            cin >> withdrawAmount;
            obj.withdraw(withdrawAmount);
        }
        else if(choice == 2){
            double depositAmount;
            cout << "Enter a deposit Amount:";
            cin >> depositAmount;
            obj.deposit(depositAmount);
        }
        else if(choice == 3){
            cout << "Current Balance: $" << obj.getBalance() << endl;
        }
        else if(choice == 4){
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

            obj = bankAccount(newAccountNum, newFirstName, newLastName, startingBalance);

            cout << "New account created successfully!\n";
            cout << "Account Holder: " << obj.firstName << " " << obj.lastName << "\n";
            cout << "Account Number: " << obj.accountNum << "\n";
            cout << "Starting Balance: $" << obj.getBalance() << "\n";
        }
        else if(choice == 5){
            cout << "Thanks for using Chase Bank!\n";
            break;
        }
        else{
            cout << "Invalid Input! please try again!\n";
        }
    }
    return 0;
}

void menu(){
    cout << "\n1. Withdraw\n";
    cout << "2.deposit\n";
    cout << "3.check balance\n";
    cout << "4.Create New Account\n";
    cout << "5.Close Session\n";
}
