# Leighton_CSCI2270_FinalProject: Text analyzer with login system
Christopher Leighton's final project for CSCI 2270

Summary:
--------------
This program features an authentication system through which users can create accounts and log into them (implemented with a binary tree).  It features features like checking if a username a user is trying to create already exists and refusing them if that's the case.  In a demonstration of my prowess in cryptography, the usernames and passwords will be stored in a plain text file.  Each time the program is started, it reads the text file and rebuilds the database (and likewise, upon the program being closed, it exports any updates to the userdata to the text file).  Upon login, the user has several options, one of which is to enter a length of text which the program can analyze (calculating most used words, calculating average word length, sort their text alphabetically).  At this point, the user also has the option to change their password and delete their account.

How to Run:
--------------
Place main.cpp, UserData.cpp, UserData.h, and logins.txt in the same directory and compile with:
	
	g++ -o leighton_finalproject main.cpp UserData.cpp

Dependencies:
--------------
All used libraries are included in the C++ standard library.

System Requirements:
--------------
Designed for Linux.  Should run on Mac with minimal modification or on Windows with slightly more extensive modification.

Contributors:
--------------
Christopher Leighton

Open issues/bugs:
--------------

