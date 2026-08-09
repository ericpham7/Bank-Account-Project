#include <iostream>

using namespace std;

class bankAccount{
    public:
        int accountNum;
        string firstName, lastName, pin, CVC, expirationDate;

        void withdraw(double withdrawAmount){
            if(withdrawAmount > balance){
              cout <<  "Insufficient funds. Please try again";
            }
        };

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
            cout << "Enter a withdraw Amount\n";
            cin >> withdrawAmount;
            obj.withdraw(withdrawAmount);
        }
    }
    return 0;
}

void menu(){
    cout << "1. Withdraw\n";
    cout << "2.deposit\n";
    cout << "3.check balance\n";
}
