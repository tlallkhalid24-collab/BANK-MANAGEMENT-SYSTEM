// ======================================================
//        BANK MANAGEMENT SYSTEM IN C++
//        Object-Oriented Programming Project
// ======================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// ======================================================
//                 BASE ACCOUNT CLASS
// ======================================================

class BankAccount
{
protected:
    int accountNumber;
    string holderName;
    double balance;

public:
    // Constructor
    BankAccount()
    {
        accountNumber = 0;
        holderName = "";
        balance = 0;
    }

    // Virtual function for polymorphism
    virtual void createAccount()
    {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cin.ignore();

        cout << "Enter Account Holder Name: ";
        getline(cin, holderName);

        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    // Deposit Function
    void deposit()
    {
        double amount;

        cout << "\nEnter Deposit Amount: ";
        cin >> amount;

        if (amount > 0)
        {
            balance += amount;
            cout << "Amount Deposited Successfully!\n";
        }
        else
        {
            cout << "Invalid Amount!\n";
        }
    }

    // Withdraw Function
    void withdraw()
    {
        double amount;

        cout << "\nEnter Withdrawal Amount: ";
        cin >> amount;

        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            cout << "Withdrawal Successful!\n";
        }
        else
        {
            cout << "Insufficient Balance or Invalid Amount!\n";
        }
    }

    // Display Account Details
    virtual void displayAccount()
    {
        cout << "\n===================================";
        cout << "\nAccount Number : " << accountNumber;
        cout << "\nHolder Name    : " << holderName;
        cout << "\nBalance        : " << balance;
        cout << "\n===================================\n";
    }

    // Get Account Number
    int getAccountNumber()
    {
        return accountNumber;
    }

    // Save Data to File
    virtual void saveToFile(ofstream &file)
    {
        file << accountNumber << endl;
        file << holderName << endl;
        file << balance << endl;
    }

    // Load Data from File
    virtual void loadFromFile(ifstream &file)
    {
        file >> accountNumber;
        file.ignore();

        getline(file, holderName);

        file >> balance;
    }
};

// ======================================================
//               SAVING ACCOUNT CLASS
// ======================================================

class SavingAccount : public BankAccount
{
private:
    double interestRate;

public:
    SavingAccount()
    {
        interestRate = 5.0;
    }

    void createAccount() override
    {
        BankAccount::createAccount();

        cout << "Enter Interest Rate (%): ";
        cin >> interestRate;
    }

    void displayAccount() override
    {
        cout << "\n------ Saving Account ------";
        BankAccount::displayAccount();
        cout << "Interest Rate : " << interestRate << "%\n";
    }

    void saveToFile(ofstream &file) override
    {
        file << "Saving" << endl;

        BankAccount::saveToFile(file);

        file << interestRate << endl;
    }

    void loadFromFile(ifstream &file) override
    {
        BankAccount::loadFromFile(file);

        file >> interestRate;
    }
};

// ======================================================
//               CURRENT ACCOUNT CLASS
// ======================================================

class CurrentAccount : public BankAccount
{
private:
    double overdraftLimit;

public:
    CurrentAccount()
    {
        overdraftLimit = 1000;
    }

    void createAccount() override
    {
        BankAccount::createAccount();

        cout << "Enter Overdraft Limit: ";
        cin >> overdraftLimit;
    }

    void displayAccount() override
    {
        cout << "\n------ Current Account ------";
        BankAccount::displayAccount();
        cout << "Overdraft Limit : " << overdraftLimit << endl;
    }

    void saveToFile(ofstream &file) override
    {
        file << "Current" << endl;

        BankAccount::saveToFile(file);

        file << overdraftLimit << endl;
    }

    void loadFromFile(ifstream &file) override
    {
        BankAccount::loadFromFile(file);

        file >> overdraftLimit;
    }
};

// ======================================================
//                 GLOBAL VECTOR
// ======================================================

vector<BankAccount*> accounts;

// ======================================================
//              SEARCH ACCOUNT FUNCTION
// ======================================================

BankAccount* findAccount(int accNo)
{
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i]->getAccountNumber() == accNo)
        {
            return accounts[i];
        }
    }

    return NULL;
}

// ======================================================
//             CREATE NEW ACCOUNT FUNCTION
// ======================================================

void createNewAccount()
{
    int choice;

    cout << "\n1. Saving Account";
    cout << "\n2. Current Account";
    cout << "\nChoose Account Type: ";
    cin >> choice;

    BankAccount* acc;

    if (choice == 1)
    {
        acc = new SavingAccount();
    }
    else
    {
        acc = new CurrentAccount();
    }

    acc->createAccount();

    accounts.push_back(acc);

    cout << "\nAccount Created Successfully!\n";
}

// ======================================================
//               DEPOSIT FUNCTION
// ======================================================

void depositMoney()
{
    int accNo;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount* acc = findAccount(accNo);

    if (acc != NULL)
    {
        acc->deposit();
    }
    else
    {
        cout << "Account Not Found!\n";
    }
}

// ======================================================
//              WITHDRAW FUNCTION
// ======================================================

void withdrawMoney()
{
    int accNo;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount* acc = findAccount(accNo);

    if (acc != NULL)
    {
        acc->withdraw();
    }
    else
    {
        cout << "Account Not Found!\n";
    }
}

// ======================================================
//             DISPLAY ACCOUNT FUNCTION
// ======================================================

void displayAccountInfo()
{
    int accNo;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    BankAccount* acc = findAccount(accNo);

    if (acc != NULL)
    {
        acc->displayAccount();
    }
    else
    {
        cout << "Account Not Found!\n";
    }
}

// ======================================================
//                SAVE ALL DATA
// ======================================================

void saveData()
{
    ofstream file("bankdata.txt");

    for (int i = 0; i < accounts.size(); i++)
    {
        accounts[i]->saveToFile(file);
    }

    file.close();

    cout << "\nData Saved Successfully!\n";
}

// ======================================================
//                 LOAD ALL DATA
// ======================================================

void loadData()
{
    ifstream file("bankdata.txt");

    if (!file)
    {
        return;
    }

    while (!file.eof())
    {
        string type;

        getline(file, type);

        if (type == "Saving")
        {
            SavingAccount* s = new SavingAccount();
            s->loadFromFile(file);

            accounts.push_back(s);
        }
        else if (type == "Current")
        {
            CurrentAccount* c = new CurrentAccount();
            c->loadFromFile(file);

            accounts.push_back(c);
        }

        file.ignore();
    }

    file.close();
}

// ======================================================
//                     MAIN FUNCTION
// ======================================================

int main()
{
    loadData();

    int choice;

    do
    {
        cout << "\n====================================";
        cout << "\n      BANK MANAGEMENT SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Create New Account";
        cout << "\n2. Deposit Money";
        cout << "\n3. Withdraw Money";
        cout << "\n4. Check Account Details";
        cout << "\n5. Save Data";
        cout << "\n6. Exit";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            createNewAccount();
            break;

        case 2:
            depositMoney();
            break;

        case 3:
            withdrawMoney();
            break;

        case 4:
            displayAccountInfo();
            break;

        case 5:
            saveData();
            break;

        case 6:
            saveData();
            cout << "\nThank You For Using Bank Management System!\n";
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}
