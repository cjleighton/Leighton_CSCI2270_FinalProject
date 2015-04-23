#include <iostream>
#include <cstdlib>
#include "UserData.h"
#include <unistd.h>
#include <fstream>

using namespace std;

void mainMenuPrint(string occasion)
{
    system("clear");
    cout<<"1. Log in"<<endl<<"2. Create account"<<endl<<"3. Quit"<<endl;
    if(occasion=="fail"){
        cout<<"That is unacceptable."<<endl;
        sleep(1);
        mainMenuPrint("null");
    }
}

void secondaryMenuPrint(string occasion)
{
    system("clear");
    cout<<"1. Load text"<<endl<<"   2. Most common words"<<endl<<"   3. Average word length"<<endl<<"4. List all users"<<endl<<"5. Change password"<<endl<<"6. Delete account"<<endl<<"7. Log out"<<endl;
    if(occasion=="fail"){
        cout<<"That is unacceptable."<<endl;
        sleep(1);
        secondaryMenuPrint("null");
    }
}

UserData submenu(UserData database){
    string input;
    bool running=true;
    bool passChanged;
    string text, oldPass, newPass;
    secondaryMenuPrint("null");
    while(running==true){
        cin>>input;
        cin.ignore();
        if(input=="1"){ ///LOAD TEXT
            secondaryMenuPrint("null");
            cout<<"Enter text: "<<endl;
            getline(cin,text);
            database.loadText(text);
            secondaryMenuPrint("null");
            cout<<"Loaded."<<endl;
            sleep(1);
            secondaryMenuPrint("null");
        }
        else if(input=="2"){ ///MOST COMMON WORDS
            secondaryMenuPrint("null");
            database.commonWords();
        }
        else if(input=="3"){ ///AVERAGE WORD LENGTH
            secondaryMenuPrint("null");
            database.avgWordLength();
        }
        else if(input=="4"){ ///LIST USERS
            secondaryMenuPrint("null");
            database.interface("list");
        }
        else if(input=="5"){ ///CHANGE PASSWORD
            secondaryMenuPrint("null");
            cout<<"Current password: "<<endl;
            cin>>oldPass;
            cout<<"New password: "<<endl;
            cin>>newPass;
            passChanged=database.changePass(oldPass, newPass);
            secondaryMenuPrint("null");
            if(passChanged==false){
                cout<<"Current password incorrect."<<endl;
                sleep(1);
                secondaryMenuPrint("null");
            }
            else{
                cout<<"Password changed."<<endl;
                sleep(1);
                secondaryMenuPrint("null");
            }
        }
        else if(input=="6"){ ///DELETE ACCOUNT
            secondaryMenuPrint("null");
            database.deleteAccount();
            cout<<"Account deleted."<<endl;
            sleep(1);
            return database;
        }
        else if(input=="7"){ ///LOG OUT
            return database;
        }
    }
}

int main()
{
    UserData database;
    database.importer();

    string loggedIn="false"; //set to badLogin or badPass if bad login upon call of databse.login, submenu is never called without login function returning true
    string input, username, password;
    bool addedUser;
    mainMenuPrint("null");
    while(true){
        cin>>input;

        if(input=="1"){ ///LOG IN
            mainMenuPrint("null");
            cout<<"Username: ";
            cin>>username;
            cout<<"Password: ";
            cin>>password;
            loggedIn=database.login(username, password); //true if successful login, false if not
            if(loggedIn=="true"){ //second login and
                mainMenuPrint("null");
                cout<<"Success!"<<endl;
                sleep(1);
                database=submenu(database); //but... does submenu need to return database? I think so, to update things done to database from submenu like account deletions
                //loggedIn=false; //sort of clarifying to code readers, but completely unnecessary
                mainMenuPrint("null"); ///returns here if 6 or 7 are pressed from submenu
            }
            else if(loggedIn=="badUser"){
                mainMenuPrint("null");
                cout<<"Account does not exist."<<endl;
                sleep(1);
                mainMenuPrint("null");
            }
            else if(loggedIn=="badPass"){
                cout<<"Wrong password."<<endl;
            }
        }

        else if(input=="2"){ ///CREATE ACCOUNT
            mainMenuPrint("null");
            cout<<"Desired username: ";
            cin>>username;
            cout<<"Password: ";
            cin>>password;
            addedUser=database.addUser(username, password);
            if(addedUser==true){
                mainMenuPrint("null");
                cout<<"Account created."<<endl;
                sleep(1);
                mainMenuPrint("null");
            }
            else if(addedUser==false){
                mainMenuPrint("null");
                cout<<"That username is already taken. Try something else."<<endl;
                sleep(2);
                mainMenuPrint("null");
            }
        }

        else if(input=="3"){ ///QUIT
            mainMenuPrint("null");
            cout<<"Goodbye!"<<endl;
            database.interface("export"); //tree easier to access from inside class
            return 0;
        }
        else{
            mainMenuPrint("fail");
        }
    }
    return 0;
}
