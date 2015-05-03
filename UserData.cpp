#include "UserData.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>

/*UserData() constructor: builds root with "null" username and initializes
variables for later use.
Pre-conditions:
-userText, the users' custom text, is set to a blank string
-memory for the tree's root node is unallocated
Post-conditions:
-userText is set to "" for testing purposes in commonText, avgWordLength, and sortText
-root exists with username "null" and NULL children and parents*/
UserData::UserData()
{
    userText="";
    root=new User;
    root->username="null"; //when adding users, always check if name is null before adding and if so, refuse it
    root->parent=NULL;
    root->leftChild=NULL;
    root->rightChild=NULL;
}

UserData::~UserData(){} //destructor does nothing

/*void interface(string); is called from main to access listUsers and
exporter methods. These functions are recursive and require that the tree's
root be fed in, thus making them uncallable directly from main.  Always called
with string arguments "list" or "export", each corresponding to the
function it accesses (listUsers or exporter).
How to call: <UserData object>.interace(string) where string MUST be either "list" (to list user) or "export" (to export userdata to file)
Pre-conditions:
-user should be logged in if argument is "list"
Post-conditions:
-logins.txt will have an updated version of the user tree*/
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

/*void importer(); is immediately run in main when the program starts.
It reads a text file of the form "user,pass\n" and passes in those usernames
and passwords to addUser, which actually builds the tree
How to call: <UserData object>.importer(). Should only be called at the beginning of the program,
immediately after the root node is built. The method expects no arguments.
Pre-conditions:
-'root' is the only active node in the binary tree
-logins.txt is unopened
Post-conditions:
-logins.txt has been read
-the binary tree has been built (via the addUser function)*/
void UserData::importer()
{
    ifstream inFile("logins.txt");
    string importUser,importPass;
    while(inFile){
        getline(inFile,importUser,',');
        getline(inFile,importPass,'\n');
        if(importUser != ""){ //last line is there, but empty - program mistakenly processes it as user. this catches that.
            addUser(importUser, importPass);
        }
    }
}

/*void exporter(User*); is indirectly called through the 'interface' method and
begins a recursive inorder traversal of the tree. It writes userdata to string
exportLine, which the interface function then writes to logins.txt.
How to call: calling <UserData object>.interface("export") will execute this function (where "export" is a string)
Pre-conditions:
-globally accessible string exportLine is blank
-the tree should be built (it will be if the program is running, clearly)
Post-conditions:
-all userdata from the tree is written to string exportLine
-method 'interface(string)' then accesses exportLine and writes it to logins.txt*/
void UserData::exporter(User* x)
{
    if(x->leftChild != NULL){
        exporter(x->leftChild);
    }
    if(x->username != "null"){ //skips over 'null' root
        exportLine=exportLine+x->username+","+x->password+"\n"; ///seems like this will get astoundingly inefficient with large trees
    }
    if(x->rightChild != NULL){
        exporter(x->rightChild);
    }
}

/*string login(string,string); is called when the user tries to login and
takes a string username and password. It returns strings that indicate a
good login, a bad login because of a wrong password, or a bad login because
of a missing account (username not found).
How to call: <UserData object>.login(string,string) should be called whenever the user
tries to login.  Its first argument is the username the user tries to login with and the
latter is the password they enter.
Pre-conditions:
-the tree is built (and can be searched by the 'searcher' method)
-sessionUser and sessionPassword are either blank or contain the credentials of the
previously logged in user
-string "loggedIn" in main.cpp is "false"
Post-conditions:
-the sessionUser and sessionPassword are written with the credentials of the current user
-string "loggedIn" in main.cpp is "true"*/
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
        return "badUser"; //returns string so that we can have these three different returns; booleans'true' and 'false' aren't enough
    }
}

/*bool addUser(string,string); is called from: a) the main menu when the user wants to add a new
account and b) from importer, repeatedly, as data from logins.txt is read.
If the username they enter is not already taken, a node with their credentials is added
to the binary tree.
How to call: <UserData object>.addUser(string,string) where the first string is the desired username
and the second is the desired password.
Pre-conditions:
-tree must be built
-for a successfully created account: node with desired username can't already exist
Post-conditions:
-if account successfully created: tree will have a new node with new user's data
-if account successfully created: boolean 'addedUser' in main.cpp is set to true*/
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

/*void listUsers(User*); is called from the interface function with the
tree's root as an argument.  It begins an inorder, recursive readout of
the tree.
How to call: <UserData object>.interace("list")
Pre-conditions:
-tree is built
-user should be logged in
Post-conditions:
-none, changes nothing*/
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

/*bool changePass(string,string); finds the currently logged in user's account in the binary
tree, verifies that the oldPass they enter matches sessionPassword, which was set from the 'login'
method, and then replaces it
with a new password;
How to call: <UserData object>.changePass(string,string) where the first string is the user's
current password (they must re-enter is for security) and the latter string is their new password.
Pre-conditions:
-tree is built
-user is logged in
-sessionPassword is set (occurs automatically with a succesful login)
-boolean passChanged in main.cpp is set as false
Post-conditions:
-if successful password change: user's password in tree is set to newPass
-sessionPassword is set to newPass
-boolean passChanged in main.cpp is set to true*/
bool UserData::changePass(string oldPass, string newPass)
{
    if(oldPass != sessionPassword){
        return false;
    }
    else{
        User *findUser;
        findUser=searcher(sessionUser); //user already logged in, so findUser should ALWAYS get the right user, not just the closest
        findUser->password=newPass;
	sessionPassword=newPass;
        return true;
    }
}

/*void deleteAccount() finds the node with node->username that matches sessionUser, which was
set to the current user's username in the 'login' method.  It then deletes that node from the
binary tree.
How to call: <UserData object>.deleteAccount()
Pre-conditions:
-tree is built
-user is logged in (and therefore, account to delete exists and sessionUser is set)
Post-conditions:
-node with current user's credentials are deleted from binary tree*/
void UserData::deleteAccount()
{
    User* x=searcher(sessionUser);
    //x->username definitely equals sessionUser
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

/*void loadText(string); is called when the logged-in user asks to enter a
string of text, which loadText splits into individual words, and pushes into 
'text' and 'textTemp', globally declared vectors of textVector structs and also into
'textAlpha', a vector of strings.
How to call: <UserData object>.loadText(string) where string is the user's custom text.
Pre-conditions:
-user is logged in
-string 'text' in main.cpp is set by user just prior to loadText being called
Post-conditions:
-vector 'text' is filled with the individual words in userText
-vector 'textTemp' is filled with the individual words in userText
-vector 'textAlpha' is filled with the individual words in userText
-the number of occurences of each word is recorded in each item in text and textTemp's integer 'usages' attribute. for example, if the vector has words <test, test, hello, test>, then items 0, 1, and 3
record their usage as 3 while item 3 only records a usage of 1.*/
void UserData::loadText(string uText)
{
    text.clear(); //clear if user is doing this for a second time (ergo text will already have stuff in it)
    textTemp.clear();
    textAlpha.clear();
    userText=uText; //no need to reset userText to "", though
    istringstream is(uText);
    string word;
    textVector *newText=new textVector; //new word to be inserted
    while(is>>word){
        newText->word=word;
        newText->usages=1;
        text.push_back(*newText);
        textTemp.push_back(*newText); //for use in commonWords
        textAlpha.push_back(word); //for use in sortText
    }
    for(int i=0; i<text.size(); i++){ //calculates the usage of each text[i].word and inserts usage into text[i].usages
        for(int j=0; j<text.size(); j++){
            if(j!=i && text[i].word==text[j].word){
                text[i].usages++;
                textTemp[i].usages++; //for use in commonWords
            }
        }
    }
}

/*void commonWords(); takes the now-filled textTemp, removes repeats, 
and then if a) textTemp.size()<10, prints all words in order of decreasing usage,
or if b) textTemp.size()>10, prints the top 10 most common words in order of
decreasing usage.
How to call: <UserData object>.commonWords()
Pre-conditions:
-vector of textVector items 'textTemp' is filled with individual words from userText (done in loadText)
-userText cannot be blank or "No text detected" will be printed
-user must be logged in
Post-conditions:
-textTemp is severely corrupted (cleared upon next calling of loadText anyway, so it doesn't matter)
-userText is reset to ""*/
void UserData::commonWords()
{
    if(userText==""){
        cout<<"No text detected."<<endl;
        return;
    }
    vectorSize=text.size();
    for(int i=0; i<textTemp.size(); i++){ //removes duplicate words
        for(int j=i+1; j<textTemp.size(); j++){
            if(textTemp[j].word==textTemp[i].word){
                textTemp[j].word="";
                textTemp[j].usages=-10000;
            }
        }
    }
    int wordsLeft=10;
    while(wordsLeft>0 && vectorSize>0){ //prints most common word, then second most common, etc, until textTemp is empty OR 10 most common words have been printed
        int maximum=0;
        int indexOfMax;
        for(int i=0; i<textTemp.size(); i++){
            if(textTemp[i].usages>maximum){
                maximum=textTemp[i].usages;
                indexOfMax=i;
            }
        }
        if(textTemp[indexOfMax].usages != -10000){ //excludes deleted items with usages=-10000
            cout<<textTemp[indexOfMax].word<<" ("<<textTemp[indexOfMax].usages<<" usages"<<")"<<endl;
        }
        textTemp[indexOfMax].word=""; //removes word that was just printed
        textTemp[indexOfMax].usages=-10000;
        vectorSize--;
        wordsLeft--;
    }
    userText=""; //resets userText to prevent avgWordLength being called again without new text
}

/*void avgWordLength(); calculates the average length
of the words in vector 'text'. If there are no words in textVector (i.e. the
user hasn't entered anything), avgWordLength will say that no text is detected.
How to call: <UserData object>.avgWordLength();
Pre-conditions:
-userText != "" (should be set in loadText)
-vector 'text' is not empty (should be filled in loadText method)
Post-conditions:
-userText=""*/
void UserData::avgWordLength()
{
    if(userText==""){
        cout<<"No text detected."<<endl;
        return;
    }
    float letterSum=0;
    float wordCount=0;
    float avgWordLength;
    for(int i=0; i<text.size(); i++){
        letterSum=letterSum+text[i].word.size();
        wordCount=wordCount+1;
    }
    avgWordLength=letterSum/wordCount;
    cout<<"Average word length: "<<avgWordLength<<" letters."<<endl;
    userText=""; //resets userText to prevent avgWordLength being called again without new text
}

/*void sortText(); takes no arguments and sorts the words in textVector
alphabetically.  If nothing is found in textVector (i.e. the use hasn't
entered anything), sortText will say that no text is detected.
How to call: <UserData object>.sortText();
Pre-conditions:
-userText != "" (should be set in loadText)
-vector 'textAlpha' is not empty (should be filled in loadText method)
Post-conditions:
-userText=""
-vector 'textAlpha' has been sorted alphabetically*/
void UserData::sortText()
{
    if(userText==""){
        cout<<"No text detected."<<endl;
        return;
    }
    vectorSize=text.size();
    bool flag;
    do{
        flag=0;
        for(int i=0; i<(vectorSize-1); i++){
            if(textAlpha[i]>textAlpha[i+1]){
                textAlpha[i].swap(textAlpha[i+1]);
                flag=1;
            }
        }
    } while(flag==1);

    for(int i=0; i<textAlpha.size(); i++){
        cout<<textAlpha[i]<<endl;
    }

    userText="";
}

/*User* searcher(string); is never called from main(), only from other
methods in the UserData class (login, addUser, changePass, deleteAccount).
It begins a standard search through the tree and returns a) the root with
username "null" if a node with the argument 'username' isn't found, or b)
the node with the same username as argument 'username'.
How to call: <UserData objet>searcher(string) where string is a username
Pre-conditions:
-binary tree is built
Post-conditions:
-wherever this method was equated to a pointer to a node, that pointer is set to
point to the node 'x' that this searcher method returns*/
User* UserData::searcher(string username)
{
    User *x=root;
    while(x != NULL && username != x->username){
        if(username.compare(x->username)<0){
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
