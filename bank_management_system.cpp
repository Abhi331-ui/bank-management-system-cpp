#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<ctime>


using namespace std;


void logTransaction(int accNo, string type, float amount) {
    ofstream logFile("transactions.txt", ios::app);

    time_t now = time(0);
    char* dt = ctime(&now);

    logFile << "AccNo: " << accNo
            << " | " << type
            << " | Amount: " << amount
            << " | Time: " << dt;

    logFile.close();
}

class account{
    private:
        int account_number;
        char name[50];
        float balance;
        int PIN;

    public:
        void create_account(){
            cout << "Enter Account Number: ";
            cin >> account_number;
            cout << "Enter User Name: ";
            cin >> name;
            cout << "Enter Balance: ";
            cin >> balance;
            cout << "Set PIN: ";
            cin >> PIN;
        }

        void display_account(){
            cout << "\n Account Number: " << account_number;
            cout << "\n Name: " << name;
            cout << "\n Balance: " << balance;
        }

        void deposit(float amount){
            balance = balance + amount;
        }

        void withdraw(float amount){
            if(amount <= balance){
                balance = balance - amount;
            }
            else{
                cout << "\n Insufficient Balance";
            }
        }

        bool check_pin(int input_pin){
            return input_pin == PIN;
        }

        int get_account_number(){
            return account_number;
        }
};


int main(){
    account acc;
    int choice, pin;
    float amount;
    cout << "\n======================";
    cout << "\n BANK MANAGEMENT SYSTEM";
    cout << "\n======================\n";
       

    do{
        
        cout << "\n....Services....";        
        cout << "\n1.Create Account";
        cout << "\n2.Deposit";
        cout << "\n3.Withdraw";
        cout << "\n4.Display";
        cout << "\n5.Delete Account";
        cout << "\n6.View Transaction History";
        cout << "\n7.Delete All Accounts";
        cout << "\n8.Exit\n";
        
        
        cout << "\nEnter your Choice: \n";
        cin >> choice;

        switch (choice)
        {
        case 1:{
            acc.create_account();

            ofstream outFile("accounts.dat", ios::binary|ios::app);
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
            outFile.close();

            break;
        }
        case 2:{
            int accNo;
            cout << "Enter Account Number: ";
            cin >> accNo;

            fstream file("accounts.dat", ios::binary | ios::in | ios::out);
            account acc;

             while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
                if (acc.get_account_number() == accNo) {

                    cout << "Enter Your PIN: ";
                    cin >> pin;

                    if(acc.check_pin(pin)){
                        cout << "Enter Amount: ";
                        cin >> amount;
                        acc.deposit(amount);

                        int pos = -1 * sizeof(acc);
                        file.seekp(pos, ios::cur);
                        file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

                        cout << "Diposit Successful";
                        logTransaction(acc.get_account_number(), "DEPOSIT", amount);
                    }
                    else{
                        cout << "\nWrong PIN";
                    }
                    break;
                }
            }
            file.close();
            break;
        }

        case 3:{

            int accNo;
            cout << "Enter Account Number: ";
            cin >> accNo;

            fstream file("accounts.dat", ios::binary | ios::in | ios::out);
            account acc;

             while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
                if (acc.get_account_number() == accNo) {

                    cout << "Enter Your PIN: ";
                    cin >> pin;

                if(acc.check_pin(pin)){
                    cout << "Enter Amount: ";
                    cin >> amount;
                    acc.withdraw(amount);

                    int pos = -1 * sizeof(acc);
                    file.seekp(pos, ios::cur);
                    file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

                    cout << "Withdraw Successful";
                    logTransaction(acc.get_account_number(), "WITHDRAW", amount);
                }
                else{
                    cout << "\nWrong PIN";
                }
                break;
                }
            }
            file.close();
            break;
        }

        case 4:{
            ifstream inFile("accounts.dat", ios::binary);
            account acc;

            while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
                acc.display_account();
            }
            inFile.close();
            break;

        }

        case 5: {
            int accNo;
            cout << "Enter Account Number to delete: ";
            cin >> accNo;

            ifstream inFile("accounts.dat", ios::binary);
            ofstream outFile("temp.dat", ios::binary);

            account acc;
            bool found = false;

            while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
                if (acc.get_account_number() == accNo) {
                    found = true;
                    cout << "Account Deleted Successfully\n";
                } else {
                    outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
                }
            }

            inFile.close();
            outFile.close();

            remove("accounts.dat");           // delete original file
            rename("temp.dat", "accounts.dat"); // rename temp → original

            if (!found) {
                cout << "Account not found\n";
            }

            break;
        }

        case 6: {
            int accNo;
            cout << "Enter Account Number: ";
            cin >> accNo;

            ifstream logFile("transactions.txt");

            string line;

            bool found = false;

            cout << "\n--- Transaction History ---\n";

            while (getline(logFile, line)) {
                if (line.find("AccNo: " + to_string(accNo)) != string::npos) {
                    cout << line << endl;
                    found=true;
                }
            }

            logFile.close();
            if (!found) {
                cout << "No transactions found for this account.\n";
            }
            break;
        }

        case 7: {
            char confirm;

            cout << "Are you sure you want to delete ALL accounts? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {

                if (remove("accounts.dat") == 0) {
                    cout << "All accounts deleted successfully.\n";
                } else {
                    cout << "No accounts found or error deleting file.\n";
                }

            } else {
                cout << "Operation cancelled.\n";
            }

            break;
        }

        case 8:{
            cout << "Exiting...\n";
            break;
        }


        default:{
            cout << "Invalid choice\n";
        }
        }
    }while (choice != 5);
    
    return 0;
}