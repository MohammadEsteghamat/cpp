#include <iostream>
#include <unordered_map>
#include <iomanip>
using namespace std;

class Bank {
private:
    unordered_map<short int, double> accounts;

public:
    Bank() {
        accounts[0] = 0;
    }

    void createAccount();
    void deleteAccount();
    void increaseBalance();
    void withdraw();
    void transfer();
    void showBalance();
    ~Bank();
};

void Bank::createAccount() {
    short int id;
    cout << "Enter ID to create account: ";
    cin >> id;

    if (id == 0) {
        cout << "ID00 is reserved for the bank president.\n";
        return;
    }

    if (accounts.find(id) == accounts.end()) {
        accounts[id] = 10;
        cout << "Account with ID" << setw(2) << setfill('0') << id << " created successfully with initial balance: 10\n";
    } else {
        cout << "Account with ID" << setw(2) << setfill('0') << id << " already exists! Please try again with a unique ID.\n";
    }
}

void Bank::deleteAccount() {
    short int id;
    cout << "Enter ID to delete account: ";
    cin >> id;

    if (id == 0) {
        cout << "Account with ID00 (Bank President) cannot be deleted.\n";
        return;
    }

    if (accounts.find(id) != accounts.end()) {
        double balance = accounts[id];
        accounts.erase(id);
        accounts[0] += balance;
        cout << "Account with ID" << setw(2) << setfill('0') << id << " has been deleted successfully. Transferred " << balance << " to ID00 (Bank President).\n";
    } else {
        cout << "Account with ID" << setw(2) << setfill('0') << id << " does not exist.\n";
    }
}

void Bank::increaseBalance() {
    short int id;
    double amount;
    cout << "Enter ID to increase balance: ";
    cin >> id;
    cout << "Enter amount to add: ";
    cin >> amount;

    if (accounts.find(id) != accounts.end()) {
        double addedAmount = amount * 0.99;
        double fee = amount * 0.01;
        accounts[id] += addedAmount;
        accounts[0] += fee;
        cout << "Balance for ID" << setw(2) << setfill('0') << id << " increased by " << addedAmount << ". New balance: " << accounts[id] << ". Fee of " << fee << " added to ID00.\n";
    } else {
        cout << "Account with ID" << setw(2) << setfill('0') << id << " does not exist.\n";
    }
}

void Bank::withdraw() {
    short int id;
    double amount;
    cout << "Enter ID to withdraw from: ";
    cin >> id;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if (accounts.find(id) != accounts.end()) {
        if (accounts[id] >= amount) {
            accounts[id] -= amount;
            cout << "Withdrawal successful. New balance for ID" << setw(2) << setfill('0') << id << " is " << accounts[id] << "\n";
        } else {
            cout << "Insufficient balance for ID" << setw(2) << setfill('0') << id << ". Current balance is " << accounts[id] << "\n";
        }
    } else {
        cout << "Account with ID" << setw(2) << setfill('0') << id << " does not exist.\n";
    }
}

void Bank::transfer() {
    short int fromID, toID;
    double amount;
    cout << "Enter sender ID :";
    cin >> fromID;
    cout << "Enter receiver ID : ";
    cin >> toID;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (accounts.find(fromID) != accounts.end() && accounts.find(toID) != accounts.end()) {
        double fee = amount * 0.01;
        if (accounts[fromID] >= amount + fee) {
            accounts[fromID] -= (amount + fee);
            accounts[toID] += amount;
            accounts[0] += fee;
            cout << "Transfer successful. " << amount << " transferred from ID" << setw(2) << setfill('0') << fromID
                 << " to ID" << setw(2) << setfill('0') << toID << ". Fee of " << fee << " added to ID00 (Bank President).\n";
        } else {
            cout << "Insufficient balance in sender account (ID" << setw(2) << setfill('0') << fromID << ") for transfer and fee.\n";
        }
    } else {
        cout << "Invalid sender or receiver ID.\n";
    }
}

void Bank::showBalance() {
    short int id;
    cout << "Enter ID to view balance: ";
    cin >> id;

    if (accounts.find(id) != accounts.end()) {
        cout << "Balance for ID" << setw(2) << setfill('0') << id << " is " << accounts[id] << "\n";
    } else {
        cout << "Account with ID" << setw(2) << setfill('0') << id << " does not exist.\n";
    }
}
Bank::~Bank() {
    for (auto it = accounts.begin(); it != accounts.end(); ) {
        if (it->first != 0) {
            accounts[0] += it->second;
            cout << "Account with ID" << setw(2) << setfill('0') << it->first
                 << " deleted in destructor. Transferred " << it->second
                 << " to ID00 (Bank President).\n";
            it = accounts.erase(it);
        } else {
            ++it;
        }
    }
    cout << "Final balance for ID00 (Bank President) is " << accounts[0] << "\n";
}


int main() {
    Bank ob;
    int choice;

    do {
        cout << "\nBank System Menu:\n";
        cout << "1. Create Account\n";
        cout << "2. Delete Account\n";
        cout << "3. Increase Balance\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Show Balance\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                ob.createAccount();
                break;
            case 2:
                ob.deleteAccount();
                break;
            case 3:
                ob.increaseBalance();
                break;
            case 4:
                ob.withdraw();
                break;
            case 5:
                ob.transfer();
                break;
            case 6:
                ob.showBalance();
                break;
            case 7:
                cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}