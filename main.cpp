#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <array>
#include <iomanip>

using namespace std;

struct User{
    string name="", password="";
    int privilege=4;
    float balance=0.0;
};
struct Item{
    string disc = "";
    float price = NULL;
    bool isAvail = false;
    bool isFood = true;
};
void reload();
void addUser();
void extendUser();
void addToMenu();
void order();
void listOrders();
void updateItems();
void login();
void welcome();
bool is_match(string, string);
void clearLines(int);
int userIndex(string);

array<User, 10> users;
array<Item, 10> items;
int nUsers = 0;
int nItems = 0;
User currentUser;
string menuFileName = "data/menu.dat";
string userFileName = "data/users.dat";
string orderFileName = "data/order.dat";

int main()
{
    cout << setprecision(2) << fixed;
    order();
    reload();
    login();
    return 0;
}


void reload(){
    ofstream uFile(userFileName.c_str(), ios::app); // to assert the file exists.
    uFile.close();

    ifstream userFile(userFileName.c_str());
    if(!userFile.fail()){
        string username, password;
        int privilege;
        float balance;
        userFile >> username >> password >> privilege >> balance;
        if(!userFile.good()){
        //incase the file is empty create admin user
        User admin = {"admin", "admin", 0, 0};
        users[0] = admin;
        ofstream uFile(userFileName.c_str(), ios::app); // to assert the file exists.
        uFile << admin.name << " " << admin.password << " "<< admin.privilege << " " << admin.balance <<endl;
        uFile.close();
        }
        else{
        do{
            if(nUsers>=size(users)) { extendUser(); }
            User user = {username, password, privilege, balance};
            users[nUsers++] = user;
            userFile >> username >> password >> privilege >> balance;
        }while (userFile.good());
        userFile.close();
        }
    }
    else{
        cout << "Cannot open the file: "<<userFileName.c_str()<<endl;
    }

    ofstream mFile(menuFileName.c_str(), ios::app); // to assert the file exists.
    mFile.close();
    ifstream menuFile(menuFileName.c_str());
    if(!menuFile.fail()){
        string disc;
        float price;
        bool isFood;
        menuFile >> disc >> price >> isFood;
        if(!menuFile.good()){
        // incase the file is empty
        cout << "Nothing on the menu! Consider adding after login.\n";
        }
        do{
            Item item = {disc, price, true, isFood};
            items[nItems++] = item;
            menuFile >> disc >> price >> isFood;
        }while (menuFile.good());
        menuFile.close();
    }
    else{
        cout << "Cannot open the file: "<<menuFileName.c_str()<<endl;
    }
}

void clearLines(int li){
    cout << "\x1b["<<li<<"A";
    for(int i=0; i<li; i++){
    for(int j=0; j<80; j++){
        cout << " ";
    }
    cout << endl;
    }
    cout << "\x1b["<<li<<"A";

}

void logout(){
    clearLines(20);
    cout << "you have logged out, login again!\n";
    currentUser={"","",4,0};
    login();
}
void login(){
    string username, password;
    cout << "CAFETERIA MANAGEMENT SYSTEM, LOGIN PAGE." << endl;
    cout << "Enter username : ";
    cin >> username;
    cout << "Enter password : ";
    cin >> password;
    if(is_match(username, password)){
        currentUser = users[userIndex(username)];
        welcome();
    }
    else{
        clearLines(4);
        cout << "\x1b[31mIncorrect USERNAME or PIN, Please! try again\n.\033[0m" <<endl;
        login();
    }
}

int userIndex(string username){
    for(int i=0; i<nUsers; i++){
        if(users[i].name==username){return i;}
    }
}
bool is_match(string username, string password){
    return users[userIndex(username)].password == password;
}

void welcome(){
    clearLines(20);
    cout << "WELCOME TO PRO CAFE, CAFETERIA MANAGEMENT SYSTEM!\n";
    cout <<left<< "\x1b[32mlogged in as: " << currentUser.name <<endl
         <<"your balance: " << currentUser.balance<< "\033[0m\n";

    cout << "\t\t\tOPTIONS\n";
    cout<<"\t============================================\n";

    int privilege = currentUser.privilege;
    int cmd=0;
    switch(privilege){
    case 0: goto admin;
    case 1: goto cashier;
    case 4: goto customer;
    default: goto cheifOrBarista;
    }
    admin:
    cout << "\t||\tpress 1 - to see statistics       ||\n";
    cout << "\t||\tpress 2 - to add new user         ||\n";
    cashier:
    cout << "\t||\tpress 3 - to add new item to menu ||\n";
    cheifOrBarista:
    cout << "\t||\tpress 4 - to see active orders    ||\n";
    cout << "\t||\tpress 5 - to set availability     ||\n";
    customer:
    cout << "\t||\tpress 6 - to order food or drink  ||\n";
    cout << "\t||\tpress 7 - to logout               ||\n";
    cout << "\t--------------------------------------------\n";


    prompt:
    cout << "Enter you choice: ";
    cin >>cmd;
    switch(cmd){
    case 1:
        cout << "Statistics\n";
        //statistics();
        break;
    case 2:
        addUser();
        break;
    case 3:
        addToMenu();
        break;
    case 4:
        listOrders();
        break;
    case 5:
        updateItems();
        break;
    case 6:
        order();
        break;
    case 7:
        logout();
        break;
    default:
        clearLines(7);
        cout << "\x1b[31mInvalid Input.\n\033[0m";
        goto prompt;
        break;
    }
}

void addUser(){
    if(currentUser.privilege == 0){
        string username, password;
        int privilege;
        float balance;
        cout << "What type of user you want to add?" <<"\n0 for admin\n1 for cachier\n2 for chief\n3 for barista\n4 for customer\n";
        cout << "Enter user privilege : ";
        cin >> privilege;
        cout << "Enter username : ";
        cin >> username;
        cout << "Enter password : ";
        cin >> password;
        cout << "Enter amount of initial balance: ";
        cin >> balance;
        User newUser = {username, password, privilege, balance};
        if(nUsers>=size(users)) { extendUser(); }
        users[nUsers++] = newUser;
        cout << "\x1b[34msuccessfully created new user\n";
        cout <<left<<setw(10)<< "username: " << newUser.name<<endl;
        cout <<setw(10)<< "password: " << newUser.password<<endl;
        cout <<setw(10)<< "Balance: " << newUser.balance<< "\033[0m\n";

        ofstream userFile(userFileName.c_str(), ios::app);
        if(!userFile.fail()){
            userFile<< setiosflags(ios::fixed);
            userFile << username << " " << password << " "<< privilege << " " << balance <<endl;
            userFile.close();
            char cmd;
            cout << "Do you want to add more? y/n ";
            cin >> cmd;
            clearLines(14);
            cmd=='y'?addUser():welcome();
        }
        else{
            cout << "Sorry! Couldn't open file " << userFileName.c_str() << endl;
        }
    }
    else{
        cout << "\x1b[31mSorry! Only only admins can add new user.\n\033[0m";
        welcome();
    }
}

void addToMenu(){
    string disc;
    float price;
    char fOd;
    cout << "Food or Drink? f/d ";
    cin >> fOd;
    cout << "Enter discription: ";
    cin >> disc;
    cout << "Enter price: ";
    cin >> price;
    Item newItem={disc, price, true, fOd=='f'};
    items[nItems++] = newItem;
    ofstream menuFile(menuFileName.c_str(), ios::app);
    if(!menuFile.fail()){
        menuFile << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << setprecision(2);
        menuFile << disc << " " << price << " " << (fOd=='f') <<endl;
        menuFile.close();
        char cmd;
        cout << "Successfully added" << disc << ". Do you want to add more? y/n ";
        cin >> cmd;
        clearLines(3);
        cmd=='y'?addToMenu():welcome();
    }
    else{
        welcome();
    }
}

void order(){
    clearLines(20);
    cout << "Please choose your order.\n";
    cout << setprecision(2) << fixed;
    for(int i=0; i<nItems; i++){
        cout << "\x1b[1;49;34m"
        <<left << i << ". " << items[i].disc
        << right <<setw(6) << items[i].price
        << "\x1b[0m" << ((i+1)%4!=0? '\t' : '\n');
    }
    cout <<endl;

    struct Cart{
        Item item={"", 0.00};
        int qty=0;
        float amount = qty*item.price;
    };
    Cart orders[25];
    int ids[25];
    int countOrder = 0;
    float subtotal = 0;
    cout << "for example\x1b[1;47;34m 2 0 \x1b[0mfor 2 " << items[0].disc << "\n"
         << "Enter negative number to finish the order\n";
    for(int i=0; i<25; i++){
        cout << "Enter your order: ";
        int qty;
        cin >> qty;
        if(qty<0){
        break;
        };
        cin >> ids[i];
        orders[i] = {items[ids[i]], qty};
        subtotal += orders[i].amount;
        countOrder++;
    };
    time_t now = time(0);
    tm dateTime = *localtime(&now);
    cout << setfill('0') << right
        << endl << "Date: " <<setw(2)<< dateTime.tm_mday << "/" <<setw(2)<< 1 + dateTime.tm_mon << "/"
        <<setw(4)<< 1900 + dateTime.tm_year
        << endl << "Time: " <<setw(2) << dateTime.tm_hour << ":" <<setw(2) << dateTime.tm_min
        << endl << left << setw(15) <<"DISCRIPTION" << '\t' << setw(3)
         <<"QTY" << '\t' << setw(6) << "PRICE" << '\t' << setw(9) << "AMOUNT" << endl
         << setfill('-') << setw(44) <<"" << setfill(' ') <<endl;

    for(int i=0; i<10; i++){
      if(orders[i].qty != 0){
      cout << left << setw(15) << orders[i].item.disc << '\t' << setw(3)
           <<orders[i].qty << '\t' << right << setw(5) << orders[i].item.price
           << '\t' << setw(7) << orders[i].amount << endl;
      };
    };
    cout <<left<< setw(5) << "TXBL1" <<right<< setw(34) << subtotal << endl;
    float tax = subtotal * .0825;
    cout <<left<< setw(5) << "TAX" <<right<< setw(34) << tax << endl;
    cout <<left<< setw(5) << "TOTAL" <<right<< setw(34) << subtotal + tax << endl;

    if (currentUser.privilege < 2){
        goto pay;
    }
    cout << "Sorry you have to pay!\n";
    return;

    pay:
    cout << "Paid? y/n ";
    char cmd;
    cin >> cmd;
    if(cmd=='n'){
    order();
    }

    ofstream orderFile(orderFileName.c_str(), ios::app);
    if(!orderFile.fail()){
        orderFile << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << setprecision(2);
        orderFile << countOrder << '\t';
        for(int i=0; i<25; i++){
            if(orders[i].qty != 0){
                orderFile << ids[i] << " " << orders[i].qty << ',';
            }
        }
        orderFile << endl;
        orderFile.close();
        cout << "Successfully ordered. Do you want to order more? y/n ";
        char cmd;
        cin >> cmd;
        cmd=='y'?order():(cout<<"Order Canceled\n", welcome());
    }
}

void listOrders(){
    cout  << "Sorry! this is not implemented yet\n";
}
void updateItems(){
    cout << "Which item you want to update?\n"
         << setprecision(2) << fixed
         << "\x1b[1;49;34m" <<left << "ID " << "Discription "
         << right <<setw(6) << "Price" << '\t' << "Availability\n";

    for(int i=0; i<nItems; i++){
        cout <<left << i << ". " << setw(13) << items[i].disc
             << right <<setw(6) << items[i].price << '\t'
             << (items[i].isAvail? "available\n" : "not available\n");
    }
    cout << "\x1b[0m\nItem number: ";
    int id;
    char cmd;
    cin >> id;
    cout << "is " << items[id].disc << " available? y/n ";
    cin >> cmd;
    items[id].isAvail = cmd=='y';
    welcome();
}


void extendUser(){
    const int s = size(users);
    array<User, s> temp = users;

    array<User, 2*s> users;
    for(int i=0; i<s; i++){
        users[i] = temp[i];
    }
    delete &s;
    delete &temp;
}
/*
void printe(string str){
}
void printw(string str){
}
void prints(string str){
}
void printn(string str){
}
*/

