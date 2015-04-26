#ifndef USERDATA_H
#define USERDATA_H
#include <iostream>
#include <vector>

using namespace std; //bad practice, I know

struct textVector
{
    string word;
    int usages;
};

struct User
{
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
        void interface(string); //for printing
        string login(string,string); //user and pass
        bool addUser(string,string); //user and pass
        void listUsers(User*);
        bool changePass(string,string); //old pass and new pass
        void deleteAccount(); //call it with what they logged in with, don't ask for it again
        void loadText(string);
        void commonWords();
        void avgWordLength();
        void sortText(); //NEW FEATURE: sort entered text alphabetically
        User* searcher(string);
    protected:
    private:
        User *root;
        string sessionUser;
        string sessionPassword;
        string userText;
        vector<textVector> text; //user text stored here for analysis
        vector<textVector> textTemp; //identical to 'text', but for editing in commonWords
        int vectorSize; //size of textVector
        string exportLine;
};

#endif // USERDATA_H
