#include "UserData.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>

UserData::UserData()
{
    sessionUser="none"; //initilized upon successful login
    userText="";
    root=new User;
    root->username="null"; //when adding users, always check if name is null before adding and if so, refuse it
    //the above could be avoided by ALWAYS checking if x->parent=NULL, but that's tedious and boring and I don't care
    root->parent=NULL;
    root->leftChild=NULL;
    root->rightChild=NULL;
}

UserData::~UserData(){}

void UserData::interface(){ //this won't count as one of the 10 methods, too cheaty
    //if(occasion=="list"){
        listUsers(root);
    /*}
    else if(occasion=="export"){
        User *x=root;
        ifstream outFile;
        outFile.open("logins.txt");
        exporter(x);
    }*/
}

void UserData::importer()
{
    ifstream inFile;
    inFile.open("logins.txt");
    string importUser,importPass;
    while(inFile){
        getline(inFile,importUser,',');
        getline(inFile,importPass,'\n');
        if(importUser != ""){ //last line is there, but empty - program processes it as user
            addUser(importUser, importPass);
        }
    }
}

void UserData::exporter(User* x)
{
    string line;
    x=root;
    if(x->leftChild != NULL){
        exporter(x->leftChild);
    }
    if(x->username != "null"){ //skips over 'null' root
        //line=x->username+","+x->password+"\n";
        //outFile<<line;
    }
    if(x->rightChild != NULL){
        exporter(x->rightChild);
    }
}

string UserData::login(string username, string password)
{
    User* checker;
    checker=searcher(username); //searcher just returns closest match
    if(checker->username==username && checker->password==password){
        sessionUser=username; //sets active user for later use
        sessionPassword=password;
        return "true";
    }
    else if(checker->username==username && checker->password != password){
        return "badPass";
    }
    else{
        return "badUser";
    }
}

bool UserData::addUser(string username, string password)
{
    User *test;
    test=searcher(username);
    if(test->username==username){ //user already exists
        return false;
    }

    User *newUser=new User; //only gets this far if user doesn't already exist
    newUser->username=username;
    newUser->password=password;
    newUser->leftChild=NULL;
    newUser->rightChild=NULL;
    User *x=root;
    User *parent=NULL;
    while(x != NULL){
        parent=x;
        if(newUser->username.compare(x->username)<0){
            x=x->leftChild;
        }
        else{
            x=x->rightChild;
        }
    }
    newUser->parent=parent;
    if(newUser->username.compare(parent->username)<0){
        parent->leftChild=newUser;
    }
    else{
        parent->rightChild=newUser;
    }
    return true;
}

void UserData::listUsers(User* x)
{
    if(x->leftChild != NULL){
        listUsers(x->leftChild);
    }
    if(x->username != "null"){ //skips over 'null' root
        cout<<x->username<<endl;
    }
    if(x->rightChild != NULL){
        listUsers(x->rightChild);
    }
}

bool UserData::changePass(string oldPass, string newPass)
{
    if(oldPass != sessionPassword){
        return false;
    }
    else{
        User *findUser;
        findUser=searcher(sessionUser); //user already logged in, so findUser should ALWAYS get the right user, not just the closest
        findUser->password=newPass;
        return true;
    }
}

void UserData::deleteAccount()
{
    //
}

void UserData::loadText(string text)
{
    userText=text;
    istringstream is(userText);
    string word;
    while(is>>word){
        textVector.push_back(word);
    }
}

void UserData::commonWords()
{
    if(userText==""){
        cout<<"No text detected."<<endl;
        return;
    }
}

void UserData::avgWordLength()
{
    if(userText==""){
        cout<<"No text detected."<<endl;
        return;
    }
    int letterSum=0;
    for(int i=0; i<textVector.size(); i++){
        cout<<textVector[i]<<" ";
    }
    for(int i=0; i<textVector.size(); i++){
        if(textVector[i] != " "){
            letterSum=letterSum+textVector[i].size();
        }
    }
    cout<<endl<<letterSum;
}

User* UserData::searcher(string item)
{
    User *x=root;
    while(x != NULL && item != x->username){
        if(item.compare(x->username)<0){
            x=x->leftChild;
        }
        else{
            x=x->rightChild;
        }
    }
    if(x==NULL){
        return root;
    }
    else{
        return x;
    }
}
