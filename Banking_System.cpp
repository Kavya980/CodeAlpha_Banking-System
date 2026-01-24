#include<fstream>
#include<iostream>
#include<windows.h>
#include <ctime>
using namespace std;

class Account{          //Account class
private:
string AccountNo,Password;
int Balance;
public:
Account():AccountNo(""),Password(""),Balance(0){}

void setAccountNo(string id){
AccountNo=id;
};

void setPassword(string pw){
Password=pw;
};

void setBalance(int bal){
Balance=bal;
};

void deposit(int amount) {
        Balance += amount;
    }

bool withdraw(int amount) {
    if (amount <= Balance) {
      Balance -= amount;
        return true;
        }
        return false;
    }  

string getAccountNo(){
    return AccountNo;
}

string getPassword(){
    return Password;
}

int getBalance(){
    return Balance;
}
};

class Customer {        //Customer Class
private:
    string name;
    Account account;

public:
    void setName(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    Account& getAccount() {
        return account;
    }
};
class Transaction {
private:
    string accountNo,type;
    int amount,balance;

public:
    Transaction(string acc, string t, int amt,int bal) {
        accountNo = acc;
        type = t;
        amount = amt;
        balance=bal;
    }

    void save() {
        ofstream file("Transaction.txt", ios::app);
        time_t now = time(0);
        file<<accountNo<< " | "<<type
            <<" | "<<amount<<" | Balance: "
            <<balance<<" | "<<ctime(&now);
        file.close();
    }
};

void saveAccount(Account &account) {
    ofstream file("Account.txt", ios::app);
    file << account.getAccountNo() << " "
         << account.getPassword() << " "
         << account.getBalance() << endl;
    file.close();
}

bool loadAccount(string, string, Account&);
bool loadAccountByNumber(string accNo, Account &account) {
    ifstream file("Account.txt");
    string id, pw;
    int bal;

    while (file >> id >> pw >> bal) {
        if (id == accNo) {
            account.setAccountNo(id);
            account.setPassword(pw);
            account.setBalance(bal);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void updateAccount(Account&);

bool isValidAccountNo(const string& accNo) {
    if (accNo.length() < 8)
        return false;

    for (char c : accNo) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

void openAccount(Customer &customer){    //Functions
system("cls");
string name,id,pw;
cout << "\tEnter Customer Name: ";
cin.ignore();
getline(cin, name);
customer.setName(name);

cout<<"\tEnter Account No.: ";
cin>>id;

if (!isValidAccountNo(id)) {
    cout << "Account number must be atleast 8 digits!";
    Sleep(2000);
    return;
}

customer.getAccount().setAccountNo(id);

cout<<"\tEnter a Strong Password: ";
cin.ignore();
getline(cin, pw);

Account temp;
if (loadAccountByNumber(id, temp)) {
    cout << "Account already exists!";
    Sleep(2000);
    return;
}
customer.getAccount().setAccountNo(id);
customer.getAccount().setPassword(pw);
customer.getAccount().setBalance(0);

saveAccount(customer.getAccount());
ofstream custFile("Customer.txt", ios::app);
custFile << customer.getName() << " "
         << customer.getAccount().getAccountNo() << endl;
custFile.close();

cout<<"\tAccount Created succesfully!";
Sleep(4000);
}
void depositCash() {
    string accNo, pw;
    int amount;
    Account account;

    cout << "Enter Account No: ";
    cin >> accNo;
    cout << "Enter Password: ";
    cin >> pw;

    if (!loadAccount(accNo, pw, account)) {
        cout << "Invalid Account or Password!";
        Sleep(2000);
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> amount;

    if (amount <= 0) {
    cout << "Invalid amount!";
    Sleep(2000);
    return;
    }
    account.deposit(amount);
    updateAccount(account);
    Transaction t(accNo, "DEPOSIT", amount, account.getBalance());
    t.save();
    cout << "Deposit Successful!";
    Sleep(2000);
}

void withdrawCash() {
    string accNo, pw;
    int amount;
    Account account;

    cout << "Enter Account No: ";
    cin >> accNo;
    cout << "Enter Password: ";
    cin >> pw;

    if (!loadAccount(accNo, pw, account)) {
        cout << "Invalid Account or Password!";
        Sleep(2000);
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if (!account.withdraw(amount)) {
        cout << "Insufficient Balance!";
        Sleep(2000);
        return;
    }

    updateAccount(account);
    Transaction t(accNo, "WITHDRAW", amount, account.getBalance());
    t.save();
    cout << "Withdrawal Successful!";
    Sleep(2000);
}

void fundTransfer(){
    string fromAcc,toAcc,pw;
    int amount;

    Account sender,receiver;
    cout<<"Enter your Account no.: ";
    cin>>fromAcc;
    cout<<"Enter Password: ";
    cin>>pw;

    if (!loadAccount(fromAcc, pw, sender)) {
        cout << "Invalid sender account or password!";
        Sleep(2000);
        return;
    }

    cout << "Enter receiver Account No: ";
    cin >> toAcc;
    
    if (!loadAccountByNumber(toAcc, receiver)) {
        cout << "Receiver account not found!";
        Sleep(2000);
        return;
    }
    if (fromAcc == toAcc) {
    cout << "Cannot transfer to same account!";
    Sleep(2000);
    return;
    }

    cout << "Enter Amount to Transfer: ";
    cin >> amount;

    if (amount <= 0 || sender.getBalance() < amount) {
        cout << "Insufficient Balance!";
        Sleep(2000);
        return;
    }

    sender.withdraw(amount);
    receiver.deposit(amount);

    updateAccount(sender);
    updateAccount(receiver);

    Transaction t1(fromAcc, "TRANSFER-OUT", amount, sender.getBalance());
    t1.save();

    Transaction t2(toAcc, "TRANSFER-IN", amount, receiver.getBalance());
    t2.save();

    cout << "Fund Transfer Successful!";
    Sleep(2000);
}

void viewTransactions(string accNo) {
    ifstream file("Transaction.txt");
    string line;

    cout << "\n--- Transaction History ---\n";
    while (getline(file, line)) {
        if (line.substr(0, accNo.length()) == accNo){
            cout << line << endl;
        }
    }
    file.close();
    Sleep(4000);
}

bool loadAccount(string accNo, string pw, Account &account) {
    ifstream file("Account.txt");
    string id, password;
    int balance;

    while (file >> id >> password >> balance) {
        if (id == accNo && password == pw) {
            account.setAccountNo(id);
            account.setPassword(password);
            account.setBalance(balance);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
void showAccountInfo() {
    string accNo, pw;
    Account account;

    cout << "Enter Account No: ";
    cin >> accNo;
    cout << "Enter Password: ";
    cin >> pw;

    if (!loadAccount(accNo, pw, account)) {
        cout << "Invalid Account!";
        Sleep(2000);
        return;
    }

    cout << "\n--- ACCOUNT DETAILS ---\n";
    cout << "Account No : " << account.getAccountNo() << endl;
    cout << "Balance    : " << account.getBalance() << endl;
    Sleep(4000);
}


void updateAccount(Account &account) {
    ifstream infile("Account.txt");
    ofstream outfile("AccountTemp.txt");

    string id, pw;
    int bal;

    while (infile >> id >> pw >> bal) {
        if (id == account.getAccountNo()) {
            outfile << id << " "
                    << pw << " "
                    << account.getBalance() << endl;
        } else {
            outfile << id << " "
                    << pw << " "
                    << bal << endl;
        }
    }

    infile.close();
    outfile.close();

    remove("Account.txt");
    rename("AccountTemp.txt", "Account.txt");
}

void checkBalance() {
    string accNo, pw;
    Account account;

    cout << "Enter Account No: ";
    cin >> accNo;
    cout << "Enter Password: ";
    cin.ignore();
    getline(cin, pw);

    if (!loadAccount(accNo, pw, account)) {
        cout << "Invalid Account or Password!";
        Sleep(2000);
        return;
    }

    cout << "Current Balance: " << account.getBalance();
    Sleep(3000);
}

int main(){
Customer customer;
bool exit=false;

while(!exit){
    system("cls");
    int val;
   
    cout<<"\tWelcome To Bank Account Management System"<<endl;
    cout<<"\t********************************************"<<endl;
    cout<<"\t1.Open New Account"<<endl;
    cout<<"\t2.Deposit Cash"<<endl;
    cout<<"\t3.Withdraw Cash"<<endl;
    cout<<"\t4.Check Balance"<<endl;
    cout<<"\t5.View Transactions"<<endl;
    cout<<"\t6.Fund Transfer"<<endl;
    cout<<"\t7.View Account Details"<<endl;
    cout<<"\t8.Exit\n";
    cout<<"\tEnter Your Choice: ";
    cin>>val;

 switch (val) {
        case 1:
            openAccount(customer);
            break;
        case 2:
            depositCash();
            break;
        case 3:
            withdrawCash();
            break;
        case 4:
            checkBalance();
            break;
        case 5:{
             string accNo;
            cout << "Enter Account No: ";
            cin >> accNo;
            viewTransactions(accNo);
            break;
        }
        case 6:   
          fundTransfer();
          break;

        case 7:
          showAccountInfo();
          break;

        case 8:
          exit = true;
          break;

        default:
        cout << "Invalid choice!";
        Sleep(3000);
}
if (cin.fail()) {
    cin.clear();
    cin.ignore(1000, '\n');
}
}
return 0;
}

