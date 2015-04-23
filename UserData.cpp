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

void UserData::interface(string occasion){ //this won't count as one of the 10 methods, too cheaty
    if(occasion=="list"){
        listUsers(root);
    }
    else if(occasion=="export"){
        ofstream outFile("logins.txt");
        exporter(root);
        outFile<<exportLine;
        outFile.close();
    }
}

void UserData::importer()
{
    ifstream inFile("logins.txt");
    string importUser,importPass;
    while(inFile){
        getline(inFile,importUser,',');
        getline(inFile,importPass,'\n');
        if(importUser != ""){ //last line is there, but empty - program mistakenly processes it as user. this catches that
            addUser(importUser, importPass);
        }
    }
}

void UserData::exporter(User* x)
{
    if(x->leftChild != NULL){
        exporter(x->leftChild);
    }
    if(x->username != "null"){ //skips over 'null' root
        exportLine=exportLine+x->username+","+x->password; ///seems like this will get astoundingly inefficient with large trees
        exportLine=exportLine+"\n";
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
        return "badUser"; //returns string so that we can have these three different returns; 'true' and 'false' aren't enough
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
    User* x=searcher(sessionUser);
    //x->username now definitely equals sessionUser
    if(x->leftChild==NULL && x->rightChild==NULL){ ///NO CHILDREN
        if(x->parent->leftChild==x){
            x->parent->leftChild=NULL;
        }
        else{
            x->parent->rightChild=NULL;
        }
        delete x;
    }
    else if((x->leftChild==NULL && x->rightChild != NULL) || (x->leftChild != NULL && x->rightChild==NULL)){ ///ONE CHILD
        if(x->leftChild==NULL && x->rightChild != NULL){
            if(x->parent->leftChild==x){
                x->parent->leftChild=x->rightChild;
                x->rightChild->parent=x->parent;
                delete x;
            }
            else{
                x->parent->rightChild=x->rightChild;
                x->rightChild->parent=x->parent;
                delete x;
            }

        }
        else{
            if(x->parent->leftChild==x){
                x->parent->leftChild=x->leftChild;
                x->leftChild->parent=x->parent;
                delete x;
            }
            else{
                x->parent->rightChild=x->leftChild;
                x->leftChild->parent=x->parent;
                delete x;
            }
        }
    }
    else if(x->leftChild != NULL && x->rightChild != NULL){ ///TWO CHILDREN
        User* xRight=x->rightChild;
        if(xRight->leftChild==NULL && xRight->rightChild==NULL){
            x->username=xRight->username;
            x->password=xRight->password;
            delete xRight;
            x->rightChild=NULL;
        }
        else{
            if((x->rightChild)->leftChild != NULL){
                User* xLeft=(x->rightChild)->leftChild;
                User* xLeftP=(x->rightChild);
                while(xLeft->leftChild != NULL){
                    xLeftP=xLeft;
                    xLeft=xLeft->leftChild;
                }
                x->username=xLeft->username;
                x->password=xLeft->password;
                delete xLeft;
                xLeftP->leftChild=NULL;
            }
            else{ //(x->right)->right != NULL
                User* temp=x->rightChild;
                x->username=temp->username;
                x->password=temp->password;
                x->rightChild=temp->rightChild;
                delete temp;
            }
        }
    }
}

void UserData::loadText(string text)
{
    textVector.clear(); //clear if user is doing this for a second time (ergo textVector will already have stuff in it)
    userText=text; //no need to reset userText to "", though
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
    int wordCount=0;
    float avgWordLength;
    for(int i=0; i<textVector.size(); i++){
        letterSum=letterSum+textVector[i].size();
        wordCount=wordCount+1;
    }
    float letterSumD=float(letterSum); //just conversion from int to double for better averaging
    float wordCountD=float(wordCount);
    avgWordLength=letterSumD/wordCountD;
    cout<<"Average word length: "<<avgWordLength<<" characters."<<endl;
}

void UserData::sortText()
{
    //
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
