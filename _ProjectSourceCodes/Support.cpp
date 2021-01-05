/***************************************************************
Filename: Support.cpp
Description:
 This .cpp file implements the functions declared in Support.h header file.
***************************************************************/

#include "Support.h"

// This function reads input from a text file and puts into s any character read between
// delimiters (character passed to d, end-of-line character or line-feed character).
// Parameters:
//    s - buffer to hold item read
//    d - delimiter character
// Reference: sample-structured-type-02.cpp by Sir J. Catalan 
void getDelimitedItem(char s[], char d, FILE*fp) {
	int c = 0, i = 0;
	s[i] = 0;
	while (!feof(fp)) { // while not end of file
	    c=fgetc(fp);	// get ASCII value of first character in file
	    // Check if c is a delimiter
		// ASCII character code 10 is new line (\n)
	    if (c==d||c=='\n'||c==10||c==EOF) return;
	    else {
	        s[i] = c;
	        i++;
	        s[i] = 0;  // set next slot in string to null 
    	}
  	}
}

// It finds the time after M minutes have passed from the given time T in 24-hour format 
// and replaces the original time T with the new time.
// This function is based from NaimishSingh's program found in a GeeksforGeeks article, link as follows:
// https://www.geeksforgeeks.org/program-to-find-the-time-after-k-minutes-from-given-time/
void getTime(char* T, int M) {	
	// Convert the given time (T) in minutes and stores it to the var minutes.
	int minutes = ((T[0] - '0') * 10 + T[1] - '0') * 60 + ((T[3] - '0') * 10 + T[4] - '0'); 
  	// The string T gives the time in 24-hr format, hh:mm, where hh is the hours and mm is the minutes.            
	// Strings are an array of characters; as such, T[0] calls the first character in T,                           
	// T[1] calls the second char and so on so forth.                                                              
	// The expression char - '0' results in a "proper integer digit" by                                            
	// subtracting the ASCII code for 0 from the ASCII code for a specific digit character.                        
	// Ex. '7' - '0' = 7 because as the ASCII code for 7 and 0 are 55 and 48, respectively,                        
	//			the expression '7' - '0' is equivalent to 55 - 48 , which results to 7.                                
	// Reference: https://www.quora.com/Why-does-char-0-successfully-convert-a-char-to-int-in-C/answer/Greg-Kemnitz
	
	minutes += M; // Add M minutes to the current minutes 
   
	// Obtain the new hour and new minutes from minutes var
	int hour = (minutes / 60) % 24; 
	int min = minutes % 60; 
		  
	// Print the hour in appropriate format to string T
	if (hour < 10) sprintf(T, "0%d:", hour);
	else sprintf(T, "%d:", hour); 
//  printf("%s\n", T);
  
    // Print the minute in appropriate format to string T
    char tempstr[6];
	if (min < 10) sprintf(tempstr, "0%d\0", min);
    else sprintf(tempstr, "%d\0", min);
    strcat(T, tempstr);
}

// It dynamically creates a 2D array of integers and returns a pointer to it.
// Parameters:
//   n - number of rows of 2D array
//   m - number of columns of 2D array
int** createDynamicIntArray2D(int n, int m) {
	int **arr, i;
	arr = (int**) calloc (n, sizeof(int*));
    for (i=0; i<n; i++) arr[i] = (int*) calloc (m, sizeof(int));
    return arr;
}

// It frees up a dynamically created 2D string array.
// Parameters:
//   arr - pointer to 2D string array
//   n   - number of rows
void freeDynamicIntArray2D(int **arr, int n) {
    int i;
    for (i=0; i<n; i++) free(arr[i]);
}

// It dynamically creates a 1D array of string and returns a pointer to it.
// Parameter:  n - size of 1D array
char** createDynamicStrArray(int n) {
    int i;
    char **arr;
    arr = (char **) calloc(n, sizeof(char *));
    for (i=0; i<n; i++) arr[i] = (char *) calloc(50, sizeof(char));
    return arr;
}

// It dynamically creates a 2D array of string and returns a pointer to it.
// Parameters:
//   n - number of rows of 2D array
//   m - number of columns of 2D array
char*** createDynamicStrArray2D(int n, int m) {
    int i;
    char ***arr;
    arr = (char ***) calloc(n, sizeof(char **));
    for (i=0; i<n; i++) arr[i] = createDynamicStrArray(m);
    return arr;
}

// It frees up a dynamically created 2D string array.
// Parameters:
//   arr - pointer to 2D string array
//   n   - number of rows
void freeDynamicStrArray2D(char ***arr, int n) {
    int i;
    for (i=0; i<n; i++) free(arr[i]);
}

// Swaps the values of two given integer pointers.
void swapIntValues(int *x, int *y) {
	int temp = *x;  // Get value of pointer x with * (dereferencing a pointer)
	*x = *y;  // Calling * on a pointer dereferences the pointer, getting its valiue.
	*y = temp;  // Save value of temp as the value of y
}

// Swaps the values of two given string pointers.
void swapStrValues(char *a, char *b) {
	char temp[50];
	strcpy(temp, a);
	strcpy(a, b);
	strcpy(b, temp);
}

// Align a string (s) to the left and pad it with spaces acoording to a specified width (w).
void padStr(int w, const char* s) {
	printf("%-*s", w, s);	// - is use left justify the output; asterisk is for the width
}

// Pad a specified integer (x) with spaces acoording to a specified width (w)
void padInt(int w, int x) {
	printf("%-*d", w, x);  // - is use left justify the output; asterisk is for the width
}
