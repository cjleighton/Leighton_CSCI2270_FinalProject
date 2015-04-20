#ifndef USERDATA_H
#define USERDATA_H
#include <iostream>
#include <vector>

using namespace std; //bad practice, I know

struct User{
    string username;
    string password;
    //more stuff, text? analysis? we'll see.
    User *parent;
    User *leftChild;
    User *rightChild;
};

class UserData
{
    public:
        UserData();
        ~UserData();
        void importer();
        void exporter(User *x);
        void interface(); //for printing
        string login(string,string); //user and pass
        bool addUser(string,string); //user and pass
        void listUsers(User*);
        bool changePass(string,string); //old pass and new pass
        void deleteAccount(); //call it with what they logged in with, don't ask for it again
        void loadText(string);
        void commonWords();
        void avgWordLength();
        User* searcher(string);
    protected:
    private:
        User *root;
        string sessionUser;
        string sessionPassword;
        string userText;
        vector<string> textVector;
};

#endif // USERDATA_H
