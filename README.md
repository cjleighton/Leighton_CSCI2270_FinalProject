# Leighton_CSCI2270_FinalProject: Text analyzer with login system
Christopher Leighton's final project for CSCI 2270

Summary:
--------------
This program will feature an authentication system through which users can create accounts and log into them.  This will be implemented with a binary search tree.  It will feature features like checking if a username a user is trying to create already exists and refusing them if that is the case.  In a demonstration of my prowess in cryptography, the usernames and passwords will be stored in a plain text file.  Each time the program is started, it will read the text file and rebuild the database.  Upon login, the user will have several options, one of which will be to enter a length of text which the program will analyze in various ways (calculating most used words, calculating average word length, sort their text alphabetically, etc).  The user will at this point also have the option to export the analysis of their text into a text file, change their password, or delete their account.  Other features may be added as the program is developed.

How to Run:
--------------
Place main.cpp, UserData.cpp, UserData.h, and logins.txt in the same directory and compile with:
	g++ -o leighton_finalproject main.cpp Userdata.cpp

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
