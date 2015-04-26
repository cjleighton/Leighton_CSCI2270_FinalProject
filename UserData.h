#ifndef USERDATA_H
#define USERDATA_H
#include <iostream>
#include <vector>

using namespace std;

struct textVector //vector of this type stores userText
{
    string word;
    int usages;
};

struct User
{
    string username;
    string password;
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
        void interface(string); //for listUsers and exporter
        string login(string,string); //user and pass
        bool addUser(string,string); //user and pass
        void listUsers(User*);
        bool changePass(string,string); //old pass and new pass
        void deleteAccount(); //doesn't need argument, uses sessionUser
        void loadText(string);
        void commonWords();
        void avgWordLength();
        void sortText();
        User* searcher(string);
    protected:
    private:
        User *root;
        string sessionUser;
        string sessionPassword;
        string userText;
        vector<textVector> text; //user text stored here for analysis
        vector<textVector> textTemp; //copy of 'text', but for editing in commonWords
        vector<string> textAlpha; //copy of 'text', but for alphabetization
        int vectorSize; //size of textVector
        string exportLine;
};

#endif // USERDATA_H
