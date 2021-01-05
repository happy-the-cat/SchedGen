/***************************************************************

Filename: SchedGen.cpp

Author: J. Cai & K. Juan

Description:
 SchedGen is an application that generates a schedule for the users according 
 to a set of tasks with specified priority inputted by the user. It features 
 automatic scheduling of tasks starting at the user's prime time. Higher priority 
 tasks are scheduled during the specified prime time and before the lower ones.

 User's can log-in or sign-up to start and input their tasks for the program 
 to generate a schedule. SchedGen's database can hold a maximum of 50 users, 
 with each user having its own directory to store all the schedules they made. 
 Each schedule has a maximum capacity of 30 tasks and can extend beyond 1-day 
 schedules, depending on the input. 

 The program attempts to apply all the topics learned in LBYEC2B.
	
Acknowledgements:
 This program is based off the previous programs in the Modules 1-7 created by Sir J. Catalan, 
 the previous machine problems made by J. Cai, and some programs found in GeeksforGeeks.

****************************************************************/

#include "Support.h"
#include "Schedule.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/stat.h>

/*** Class prototypes ***/
class Account;
class Login;
class Signup;
class Menu;

/*** Class definitions ***/

// This class holds the functions for signup and login operations.
class Account {
  /*** attributes or data members ***/
	typedef struct {
		// Create structure for a pair of username and password.
		char username[20];
		char password[15];
	} userpass;
		
  public:
  	char filename[20];
    userpass usersdb[100];
    int count;
    int max;
    char *inputuser;
    char *inputpass;
    
  /*** behaviors or function members ***/
    Account() {};			// Default Constructor that doesn't change anything
    Account(int n, const char *fn);	// Overloaded constructor
	// Deconstructor
    ~Account() {
      delete inputuser;
      delete inputpass;
    }
    
    // Function prototypes
	void displayMenu();
    void entry(Account *db);
	void pwdMasking(char *password);
	bool verify(Account *db);
	bool createFolder(const char *username);
	void writeUserToFile();
    void readUsersFromFile();
};

// This class is a subclass of Account; it features the login operations.
class Login: public Account {
  public:
	/*** behaviors or function members ***/
	Login() {};		// Default Constructor that doesn't change anything
	bool verify(Account *db);
};

// This class is a subclass of Account; it features the signup operations.
class Signup: public Account {
  public:
	/*** behaviors or function members ***/
	Signup() {};	// Default Constructor that doesn't change anything
	bool verify(Account *db);	// Overloaded function from parent class
};


// TODO: Schedule


// This class contains the different menus in the program.
class Menu {
  public:
	/*** behaviors or function members ***/
	Menu() {};	// Default Constructor that does nothing
	void entryMenu(Account *acct);
	void mainMenu(char* username);
	
};

/*********************************************************************************************
The following are the implementations of the functions and constructors in the classes above. 
**********************************************************************************************/

// This is an overloaded implementation of the createFolder() function in Account class
// that creates a new folder with the username as a name for new users.
bool Account::createFolder(const char *username) {
	// mkdir(filename) function creates a new empty directory/folder with the name filename.
	// It returns 0 when a folder is successfully created the folder, and -1 for failure to do so.
	// Source: https://www.geeksforgeeks.org/create-directoryfolder-cc-program/
	if (!(mkdir(username))) return true;
		// condition is !(mkdir(...)) because 0 means false so, !false is true
	else return false;
}

// This is an implementation of the writeUserToFile() function in FileIO class. 
// It adds another user to the general textfile containing the details of all the users by overwriting it.
void Account::writeUserToFile() {
	FILE *fp;
	int i;
  
	fp = fopen(this->filename, "w");	// Open file for writing
	if (fp==NULL)
		printf("\nAn error occured. Cannot Open database.\n");
  	else {
	  	// Overwrite the text file with the updated database
	  	fprintf(fp,"%d",this->count);	// Write total count first
	  	// Loop through each user in the database and store the username and password pair 
		// in the textfile with the character '|' as the delimiter.
		for (i=0; i<this->count; i++)
  			fprintf(fp,"\n%s|%s", this->usersdb[i].username, this->usersdb[i].password);
		fclose(fp);	// Close the file
	}
}

// This is an implementation of the writeUserToFile() function in FileIO class. 
// It reads all the user details inside the textfile and 
// stores the corresponding details to the users database.
void Account::readUsersFromFile() {
  	// TO call username variable: this->usersdb[i].username
  	// TO call password variable: this->usersdb[i].password
  	// where i is the index or the number of userpass pair, 
	// starting from 0 (so index of the first pair is i=0)
  
	FILE *fp;
  	char s[100];
  	int i;
  
  	fp = fopen(this->filename, "r");	// Open file for reading
  	if (fp==NULL)
  		printf("\nAn error occured. Cannot Open database.\n");
 	else {
		getDelimitedItem(s,'|',fp);
		this->count = atoi(s);	// get the total count of userpass pairs in the database
		// atoi() function converts a given string to integer
		// Read the username and password pairs from the file and store them in the database.
		for (i=0; i<this->count; i++) {
			getDelimitedItem(this->usersdb[i].username,'|',fp);
			getDelimitedItem(this->usersdb[i].password,'|',fp);
		}
		fclose(fp);	// Close the file
	}
}

// This is an overloaded implementation of the constructor for Account class
// from the overloaded constructor in Account.
Account::Account(int n, const char *fn) {
	max = n;	// maximum capacity of the database
	strcpy(filename, fn);
	count = 0;  // Iniitalize count in case there's nothing in textfile
	// extract the users' details from the textfile and store them to the database
	readUsersFromFile();
	inputuser = new char[20];
	inputpass = new char[15];
}

// This is an implementation of the pwdMasking() function in Account class.
// This function performs password masking while getting said password from user.
void Account::pwdMasking(char *password){
	char c;
	int i=0;
	// Loops over each character in the input stream and masks it with an 
	// asterisk while storing the actual character to password string.	
	while(i>=0) { 
		c = getch();			// Get the inputted character and store it to c
		if (c==8) { 			// To not read BACKSPACE character (ANSI value = 8)
			// "\b" simply moves the cursor to the left; thus, 
			// "\b \b" moves the cursor to the left once, replaces the character with a space, 
			// and moves the cursor back to its place again, 
			// "removing" the last character in the output stream.
	     	printf("\b \b");
	    	i--;				// Decrement the password index/letter count
    	}
 		else if (c==13) {		// To not read ENTER character (ANSI value = 13)
	      	password[i]='\0';	// Print null character "\0" to signify end of string
	      	break;         		// Break out of loop for getting password and masking it
	    }
 		else {					// Perform password masking
     		printf("*");		// Print asterisk to the output stream
 			password[i] = c;	// Store the inputted character to password stirng
			i++;				// Increment the password index/letter count
    	}
	} //End while  (password masking)
	printf("\n");
}

// This is an implementation of the verify() function in Account class.
// This function verifies the inputted username and password.
bool Account::verify(Account *db) {
	// Loops over each pair of username and password in the database 
	// and compares it with the inputted username and password using strcmp() function.
	// strcmp() function compares two strings and returns 0 if both are equal.
	for (int i=0; i<db->count; i++) {
		// Return true if inputted userpass pair found a match in the database.
		// strcmp(str1, str2) compares str1 to str2, case sensitive, 
		// 		and returns 0 if both strings being compared are the same.
		if(strcmp(db->usersdb[i].username, db->inputuser) == 0 && 
			strcmp(db->usersdb[i].password, db->inputpass) == 0) return true;
	} // End For 
	return false;	// Return false if inputted userpass is not found in the database.
}

// This is an implementation of the entry() function in Account class.
// It prompts user to input their username and password and 
// store them in the inputuser and inputpass, respectively.
void Account::entry(Account *db){
	printf("\nEnter Username: ");
	// Gets username from user with a maximum length of the size of inputuser string.
	gets(db->inputuser);
	printf("Enter Password: ");
	// Call to password masking function to get password and mask it (for encryption).
	pwdMasking(db->inputpass);
}

// This is the implementation of verify() function in Login class
// that overrides the verify() function in Account class to 
// perform a complete login operation (getting userpass from user and verifying them).
bool Login::verify(Account *db) {
	db->entry(db);		// Get username and password from user as input
	// Verifies the inputted username and password for its validity (in the database or not).
	// verify(db) returns true if inputted username and password are in the database, else, false.
	if(db->verify(db)) {
		printf("\nLogin Successful! Welcome %s!\n", db->inputuser);
		printf("Moving to main menu...\n");
		return true;
	}
	else {
		printf("\nLogin Failed! Please try again.\n");
		return false;
	}
}

// This is the implementation of verify() function in Signup class that overrides 
// the verify() function in Account class to perform a complete signup operation. 
// This function returns true if account is successfully created, otherwise, false.
bool Signup::verify(Account *db) {
	// Checks if maximum users capacity (max) reached. 
	// Return false if max capacity reached to loop login menu and login again.
	if (db->count == db->max) {
		printf("\nMaximum users capacity reached!\n");
		printf("Please login to an existing account instead.");
		return false;
	}
	// Continue on creating a new account if there's a slot (max capacity not reached).
	printf("\nWelcome new user!");
	db->entry(db);	// Get username and password from user
	// Check if the inputted username is an existing one by checking it in the database.
	for (int i=0; i<=db->count; i++) {
		// If username is an existing one, return false and exit verify function to login again.
		// strcmpi(str1, str2) compares str1 to str2, case insensitive
		if (strcmpi(db->usersdb[i].username, db->inputuser)==0) {
			printf("\nUsername is taken! Please choose another username.\n");
			return false;
		}
	}	// End For (username verification)
	printf("\nAccount created successfully! Welcome abroad %s!\n", db->inputuser);
	
	// Create folder for new user with username as name.
	// createFolder() function returns true if folder is successfully created.
	if (db->createFolder(db->inputuser)) 
		printf("A folder has been created with username as the name to store your schedules.\n");
	else printf("An error occured in creating a folder with your username as the name.\n");
	
	// Add new user to database
	db->count++;	// Update users count
	// Add new user's details to the database
	strcpy(db->usersdb[db->count-1].username, db->inputuser);
	strcpy(db->usersdb[db->count-1].password, db->inputpass);
	db->writeUserToFile();	// Update general file with the new user's details
	
	printf("Moving to main menu...\n");
	return true;	// return true if username is not in the database to proceed to main menu
}


// This is an implementation of the entryMenu() function in Menu class.
// It contains the initial menu of the application for sign-up and log-in.
void Menu::entryMenu(Account *acct) {
	int choice;
	bool flag=false;	// Flag variable to signify successful login/signup
	Login *login = new Login();
	Signup *signup = new Signup();
	do {
		printf("\n-----------------------ENTRY MENU-----------------------\n");
		printf("Please choose an option below to proceed~\n");
		printf("1. Create an account\n");
		printf("2. Login\n");
		printf("3. Exit app\n");
		printf("Choice: ");
	    scanf("%i", &choice);
		getchar();
		
	    switch (choice) {
		    case 1: // Create an account
			    flag = signup->verify(acct);
			    break;
		    case 2: // Login
		        flag = login->verify(acct);
		    	break;
		  	case 3:	// Exit app
		  		exit(0);
		    default: // Default action when input is invalid
		    	printf("Please enter a valid choice.\n");
		    	break;  
	  	}
	} while (!flag);	// Loop entry menu while flag is false
	delete login;
	delete signup;
	printf("\nPress any key to continue...\n");
	getch();
}

// This is an implementation of the mainMenu() function in Menu class.
// It is the main menu that pops up after user successfully logins or signups.
void Menu::mainMenu(char* username) {
  	char choice;
  	Schedule sched = Schedule(username);
  	do {
		printf("\n------------------------MAIN MENU------------------------\n");
		printf("1. Generate a Schedule\n");
		printf("2. Edit my Schedule (add/delete task) \n");
		printf("3. Save my Schedule to text file / Update file\n");
		printf("4. Open an existing Schedule\n");
		printf("5. Display my Schedule\n");
		printf("6. Logout\n");
		printf("7. Quit Program\n");
		printf("Choice: ");
		choice = getchar();
		getchar();
	
		switch (choice) {
	    	case '1': // Generate a Schedule
	      		sched.generateSched();
				break;
	    	case '2': // Edit my Schedule (add/delete task)
	    		do {
	    			printf("\nEnter [1] to add a task and [2] to delete a task: ");
		    		choice = getchar();
					getchar();
					if (choice=='1') sched.addTask();
					else if (choice=='2') sched.removeTask();
					else printf("Invalid input. Please enter a valid choice.\n");
				} while (choice != '1' && choice != '2');
				break;
	    	case '3': // Save my Schedule to text file
	    		sched.saveSchedFile();
	    		break;
	    	case '4': // Open/Read an existing Schedule
	    		sched.openSchedFile();
	    		break;
	    	case '5': // Display my Schedule
	    		sched.printSched();
	    		break;
	    	case '6': // Logout
	    		return;
	      	case '7': // Quit the Program
	      		exit(0);
	    	default: // Default action when input is invalid 
	    		printf("Please enter a valid choice.\n");
	    		break;	
	  	}
	  	printf("\nPress any key to continue...\n");
		getch();
	} while (true);
}

// This is an implementation of the displayMenu() function in Menu class.
// It passes the current class instance of Account class and 
// calls the entry menu function in menu class to display it.
// Scope operator (::) is use to define members of a class outside the class itself.
void Account::displayMenu() {
	Menu menu;
	printf("_________________________________________________________\n\n");
    printf("  Welcome to SchedGen! Your very own schedule generator!\n");
    printf("_________________________________________________________\n");
	do {
		menu.entryMenu(this);	
			// this keyword passes the a pointer to the current class instance
		menu.mainMenu(this->inputuser);
	} while (true);
}


/***** Main function *****/
int main() {
	Account acct = Account(50, "_general.dat");
	acct.displayMenu();
}

