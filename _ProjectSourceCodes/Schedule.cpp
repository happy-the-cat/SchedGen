#include "Schedule.h"
#include "Support.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// This is an overloaded implementation of the constructor for Schedule class.
Schedule::Schedule(const char *username) { 
	strcpy(this->username, username); 
	taskCount = 0;
	strcpy(schedName, "");
}

// This is an implementation of the generateSched() function in Schedule class.
// It generates a new schedule by asking user for their tasks' details and 
// generating a schedule out of it.
void Schedule::generateSched() {
	// Check if task details is empty or not
	if (this->taskDetails != NULL) {
		// free up task details for new sched if it's not empty
		freeDynamicTaskArrays();
	}
	createDynamicTaskArrays();  // dynamically allocate memory for task details
	strcpy(schedName, "");  // Empty sched name to signify no sched name
	taskCount = 0;  // Initialize # of tasks for new scehdule
	inputDetails();
	sortTasksByPriority();
	checkTasksOrder();
	createSched();
	printSched();
}

// This is an implementation of the addTask() function in Schedule class.
// Adds another task/s and generates a new schedule.
void Schedule::addTask() {
	// Check if there's an existent schedule, if not, abort operation
	if(taskCount == 0 && strlen(schedName)==0) {
		printf("\nNo schedule created/opened yet. Please generate or open one first.\n");
		return;
	}
	// Add task to existing schedule
	inputTaskDetails();
	sortTasksByPriority();
	checkTasksOrder();
	createSched();
	printSched();
}

// This is an implementation of the removeTask() function in Schedule class.
// Removes a task from the list of tasks.
void Schedule::removeTask() {
	int i, idx;
	// Check if there's an existent schedule, if not, abort operation
	if(taskCount == 0 && strlen(schedName)==0) {
		printf("\nNo schedule created/opened yet. Please generate or open one first.\n");
		return;
	}
	// Remove task to existing schedule
	printTaskDetails();  // Print tasks and their details
	printf("\nEnter the number (No.) of the task you wish to remove: ");
	scanf("%d", &idx);
	getchar();
	// For index out of bounds, abort operation
	if(idx >= taskCount || idx < 0) {
		printf("Task number not found. No task removed.\n");
		return;
	}
	// Move the elements after the given index one step closer to the index (i-1) 
	// starting at the nearest element to "delete" the element at the given index.
	for(i=idx+1; i<taskCount; i++) {
		strcpy(taskDetails[i-1][0], taskDetails[i][0]);  // task priority
		strcpy(taskDetails[i-1][1], taskDetails[i][1]);  // task duration
		taskNumbers[i-1][0] = taskNumbers[i][0];  // task name
		taskNumbers[i-1][1] = taskNumbers[i][1];  // task time interval		
	}
	taskCount--;  // update task count
	// generate task and print it; no need reorganize tasks as no new task is added
	createSched();
	printSched();
}

// This is an implementation of the openSchedFile() function in Schedule class.
// It opens an existing schedule file and stores the tasks details in it.
// Skips operation if inputted filename doesn't exists.
void Schedule::openSchedFile() {
	char filename[50];  
	bool flag;
	printf("\nEnter the filename of the schedule you wish to open (ex. mySched1): ");
	gets(filename);
	setSchedName(filename);
	flag = readSchedFromFile();
	if (flag) printf("Schedule file successfully opened and read!\n");
	else strcpy(schedName, "");
}

// This is an implementation of the saveSchedFile() function in Schedule class.
// It saves an schedule to a textfile by storing the tasks details into it. 
// If specified schedule file exists, overwrite it with the updated schedule.
// If there is an opened schedule file, update it by overwriting it.
void Schedule::saveSchedFile() {
	char filename[20], choice;
	bool flag = false;
	FILE *fp;
	if (taskCount == 0) {  // Empty schedule
		printf("No schedule created/opened yet. Please generate or open one first.");
		return;
	}
	do {
		// Check if sched name is empty, if yes ask user for sched name.
		if (strlen(schedName) == 0) {
			printf("\nEnter new schedule's name: ");
			gets(filename);
			setSchedName(filename);
			// Check if filename exists by attempting to open it for reading
			fp = fopen(schedName, "r");
			if (fp!=NULL) {  // when file with inputted filename exists
				printf("%s schedule exists!\n", filename);
				printf("Do you wish to overwrite it (enter 'y' for yes and any character for no)? ");
				choice = getchar();
				getchar();
				if (choice != 'y') continue;  // Skip current loop and continue to next iteration
				fclose(fp);  // close file
			}
		}
		// Write schedule to file to save it as a textfile.
		flag = writeSchedToFile();
	} while (!flag);  // Loop as long as writeSchedToFile() operation is unsuccessful.
	printf("Schedule file successfully saved!\n");
}

// This is an implementation of the setSchedName() function in Schedule class.
// Sets the value of schedName by forming the file path 
// for a given schedule's filename.
void Schedule::setSchedName(const char *filename) {
	// filepath stored in schedName: username\filename.txt
	// where username is the user's folder under the current working directory
	// and filename is the actual name of the textfile.
	sprintf(schedName, "%s\\%s.txt", username, filename);
	//printf("%s", schedName);
}

// This is an implementation of the writeSchedToFile() function in Schedule class. 
// This function creates a new file, if there is no pre-existing file 
// with the same name, and writes the schedule into it. 
// Returns true for successful operation, and false otherwise.
// Delimiters: ':' for sched details    '|' for task details
bool Schedule::writeSchedToFile() {
	// Tasks parallel arrays Calling: 
	//    taskDetails[x][y] where x=taskNum; y=0 for task name; y=1 for time interval.
	//    taskNumbers[x][y] where x=taskNum; y=0 for task's priority; y=1 for duration.
	FILE *fp;  // file pointer
	int i;
	
	fp = fopen(schedName, "w");	// Create/open file for writing
	if (fp==NULL) {
		printf("\nAn error occur. Couldn't save schedule.\n");
		strcpy(schedName, "");  // Reset sched name
		return false;
	}
  	else {
	  	// Overwrite the text file with the updated database
	  	// Write total task count, work hours, break duration, and prime time first 
	  	fprintf(fp, "%d:%.2f:%d:%s", this->taskCount, this->workHrs, 
		  		this->breakMins, this->primeTime);  // sched details first
	  	// Loop through each task in the parallel arrays (taskDetails & taskNumbers)
		// and store its details in a textfile with the character '|' as the delimiter.
		// Format: task name|time interval|priority|duration
		for (i=0; i<this->taskCount; i++)
  			fprintf(fp,"\n%s|%s|%d|%d", this->taskDetails[i][0], this->taskDetails[i][1], 
			  		this->taskNumbers[i][0], this->taskNumbers[i][1]);
		fclose(fp);	// Close the file
		return true;
	}
}

// This is an implementation of the readSchedFromFile() function in Schedule class. 
// It reads all the tasks' details inside the textfile and 
// stores the corresponding details to the the tasks parallel arrays.
// Returns true for successful operation, and false otherwise.
// Delimiters: ':' for sched details    '|' for task details
bool Schedule::readSchedFromFile() {
	FILE *fp;
  	char temp[100];
  	int i;
  
  	fp = fopen(this->schedName, "r");	// Open file for reading
  	if (fp==NULL) {
  		printf("\nFile not found!\n");
  		return false;
	}
 	else {
 		// Check if tasks parallel arrays are empty or not
		if (this->taskDetails != NULL) {  // if not empty (NULL)
			freeDynamicTaskArrays(); // free up task parallel arrays for new sched
		}
		// Dynamically allocate memory for tasks parallel arrays
		createDynamicTaskArrays();
		// Get the sched details first
		getDelimitedItem(temp, ':', fp);  // get total task count
		this->taskCount = atoi(temp);	  	 // atoi() converts string to int
		getDelimitedItem(temp, ':', fp);  // get total work hours
		this->workHrs = atof(temp);		  	 // atof() converts string to float
		getDelimitedItem(temp, ':', fp);  // get break minutes
		this->breakMins = atoi(temp);
		getDelimitedItem(this->primeTime, ':', fp);  // get prime time
		// atoi() function converts a given string to integer
		// Read the task details from the file and store in the tasks parallel arrays.
		// Textfile format: task name|time interval|priority|duration
		for (i=0; i<this->taskCount; i++) {
			getDelimitedItem(this->taskDetails[i][0], '|', fp);  // retrieve task name
			getDelimitedItem(this->taskDetails[i][1], '|', fp);  // retrieve time iterval
			getDelimitedItem(temp, '|', fp);  // retrieve and parse task priority
			this->taskNumbers[i][0] = atoi(temp);
			getDelimitedItem(temp, '|', fp);  // retrieve and parse task duration
			this->taskNumbers[i][1] = atoi(temp);
		}
		fclose(fp);	// Close the file
		return true;
	}
}

// This is an implementation of the inputDetails() function in Schedule class.
// This function is exclusively for entering the details and contents 
// of NEW schedules.
void Schedule::inputDetails() {
	char choice;
	// Get the total work hours and break duration
	printf("\nEnter the number of hours you wish to work for the day: ");
	printf("\n  (Note: This will serve as the SOFT cap for the total duration of tasks only~) \n");
	printf("Enter total work hours: ");
	scanf("%f", &this->workHrs);
	printf("\nEnter the length of your break between tasks (in minutes): ");
	scanf("%d", &this->breakMins);
	getchar();  // Get leading new line ('\n') left in the input stream by scanf()
	// Get prime study time
	printf("\nPrime Study/Work Time preset: ");
	printf("\n   'Morning' 05:00 AM - 11:59 AM\t 'Afternoon' 12:00 PM - 4:59 PM");
	printf("\n   'Evening' 05:00 PM - 7:59 PM\t 'Night' 08:00 PM - 4:59 AM");
	printf("\n  Note: Prime time will serve as the schedule's start time.");
	printf("\nEnter prime study/work time (choose from Morning, Afternoon, Evening, and Night): ");
	gets(this->primeTime);
	// Check prime time input
	// strcmpi(str1, str2) compares strings, case insensitively, and returns 0 when str1==str2. 
	while (!(strcmpi(primeTime, "Morning") == 0 || strcmpi(primeTime, "Afternoon") == 0 || 
			strcmpi(primeTime, "Evening") == 0 || strcmpi(primeTime, "Night") == 0) ) {
		printf("Invalid input! Please choose from Morning, Afternoon, Evening, and Night.");
		printf("\nEnter prime study/work time: ");
		gets(primeTime);
	}
	// Get tasks
	this->inputTaskDetails(); // Function call to input task details function
}

// This is an implementation of the inputTaskDetails() function in Schedule class.
// It asks user for the tasks they plan to do for the day, and 
// the estimated duration and priority level of each task.
// Tasks parallel arrays Calling: 
//    taskDetails[x][y] where x=taskNum; y=0 for task name; y=1 for time interval.
//    taskNumbers[x][y] where x=taskNum; y=0 for task's priority; y=1 for duration.
void Schedule::inputTaskDetails() {
	char choice;
	int wrkMinsLeft = (int) this->workHrs * 60;  // convert workHrs to minutes
		// (int) cast converts workHrs to integer
	printf("\nInput tasks: \n");
	// Loop to input a task and their details
	do {
		// Get task details
		printf(" Enter task name: ");
		gets(this->taskDetails[this->taskCount][0]);
		printf(" Enter task priority from a scale of 1-3 with 1 as the most prioritized): ");
		printf("\n   (Tip: Tasks with 1 as their priority level will be placed at the prime study time period");
		printf("\n         so set the subjects and tasks you have a hard time in to the highest priority~");
		printf("\n         Tasks with the lowest priority will be scheduled last~)");
		printf("\n Enter task priority (1-3): ");
		scanf("%d", &this->taskNumbers[this->taskCount][0]);
		// Check if input is valid (if input is either 1, 2 or 3), 
		// if not, ask for input again.
		while (taskNumbers[taskCount][0] < 1 || taskNumbers[taskCount][0] > 3) {
			printf(" Invalid priority level! Make sure it is either 1 (highest priority), 2, or 3!");
			printf("\n Enter task priority (1-3): ");
			scanf("%d", &this->taskNumbers[this->taskCount][0]);
		}
		printf(" Enter task duration (in minutes): ");
		scanf("%d", &this->taskNumbers[this->taskCount][1]);
		getchar();
		
		// Check if the time has exceeded the total hours user want to work
		if (taskNumbers[taskCount][1] > wrkMinsLeft) {
			printf("   You have exceeded the total work hours you set for this schedule! \n");
			printf("   Please choose the following action to proceed: enter [1] to change task duration, \n");
			printf("     [2] to remove task, and any character to continue. \n");
			printf("   Choice: ");
			choice = getchar();
			getchar();
			if (choice == '1') {
				printf("  Enter new task duration (in minutes): ");
				scanf("%d", &this->taskNumbers[this->taskCount][1]);
				getchar();
			} else if (choice == '2') {
				taskCount--;  // Decrement (since it'll be incremented later)
				strcpy(taskDetails[this->taskCount][0], "0");
				this->taskNumbers[this->taskCount][0] = 0;
				this->taskNumbers[this->taskCount][1] = 0;
			}
		}
		// Update wrkMinsLeft 
		wrkMinsLeft = wrkMinsLeft - this->taskNumbers[this->taskCount][1];
		// Initialize task time interval to 0
		strcpy(taskDetails[taskCount][1], "0");  
		
		//printf("%i %s %s %d %d\n", taskCount, taskDetails[taskCount][0], taskDetails[taskCount][1], taskNumbers[taskCount][0], taskNumbers[taskCount][1]);
		this->taskCount++;  // Update task count (increment)
		printf("Add another task (enter any character for yes and 'n' for no)? ");
		choice = getchar();
		getchar();
		printf("\n");
	} while (choice!='n');
}

// This is an implementation of the printTaskDetails() function in Schedule class.
// Prints the tasks details with index numbers (task no.) before each task.
void Schedule::printTaskDetails() {
	int i, width=20;
	// Print tasks and their details
	printf("\n----------------------------------YOUR TASKS----------------------------------\n ");
	padStr(5, "No.");
	padStr(35, "Task Name");
	padStr(width, "Task Priority");
	padStr(width, "Task Duration (mins)");
	printf("\n ");
	// Tasks parallel arrays Calling: 
	//    taskNumbers[x][y] where x=taskNum; y=0 for task priority; y=1 for task duration.
	//    taskDetails[x][y] where x=taskNum; y=0 for task name; y=1 for task's time interval.
	for (i=0; i<this->taskCount; i++) {
		padInt(5, i);  // Print index number (also task number)
		padStr(35, taskDetails[i][0]);  // print task name
		padInt(width, taskNumbers[i][0]);  // print task priority
		padInt(width, taskNumbers[i][1]);  // print task duration
		printf("\n ");
	}
}

// This is an implementation of the checkTasksOrder() function in Schedule class.
// Prints the organized tasks, then checks with user if the order is fine, 
// and edit the order if it is not fine.
void Schedule::checkTasksOrder() {
	char choice;
	int idx1, idx2;
	printf("\nBelow are the tasks arranged based on your inputted priority.\n");
	printf("Please check if the order are to your liking.\n");
	do {
		printTaskDetails();  // Print tasks and their details
		// Ask user if they want to change the order
		printf("Do you have any changes you wish to make about the order ");
		printf("(enter 'y' for yes and any character for no)? ");
		choice = getchar();
		getchar();
		if (choice == 'y') {  // Swap the tasks
			printf("Enter the number (No.) of the task you wish to change the order of: ");
			scanf("%d", &idx1);
			printf("Enter the number (No.) of the task you wish swap it with: ");
			scanf("%d", &idx2);
			getchar();
			// Swap the task details of task at idx1 with the ones at task idx2
			swapIntValues(&taskNumbers[idx1][0], &taskNumbers[idx2][0]); // swap task priority
			swapIntValues(&taskNumbers[idx1][1], &taskNumbers[idx2][1]); // swap task duration
			swapStrValues(taskDetails[idx1][0], taskDetails[idx2][0]);   // swap task name
			swapStrValues(taskDetails[idx1][1], taskDetails[idx2][1]);   // swap task time interval
		} 
		else {
			break; // Break out of loop when user has no changes to make 
		}
	} while (true);
}

// This is an implementation of the createSched() function in Schedule class.
// It creates a schedule from the tasks and store their details back in taskDetails 
// and taskNumbers arrays by setting the corresponding time intervals for each task.
void Schedule::createSched() {
	int i;
	char startTime[10], endTime[10];  // time in 24-hr format (hh:mm)
	// Get the schedule's start time from the prime time
	this->parsePrimeTime(startTime);  // Parse prime time to get its time interval (in hh:mm-hh:mm)
	strcpy(startTime, strtok(startTime, "-"));  
			// get the startTime by getting the first token before "-"
			// strtok(str, delim) splits str at delim and returns the first token.
			// It modifies the original string, replacing the delim string in str 
			// to NULL char '\0' to track the tokens.
	strcpy(endTime, startTime);  // Initialize end time
	
	// Set time interval for each task
	// Tasks parallel arrays Calling:
	//    taskNumbers[x][y] where x=taskNum; y=0 for task priority; y=1 for task duration.
	//    taskDetails[x][y] where x=taskNum; y=0 for task name; y=1 for task's time interval.
	for (i=0; i<this->taskCount; i++) {
		// get end time (endTime + task's duration, where endTime has been initialized to startTime)
		getTime(endTime, this->taskNumbers[i][1]);
		// set task's time interval
		sprintf(taskDetails[i][1], "%s-%s", startTime, endTime);
		// update the start time for next task (include break mins)
		strcpy(startTime, endTime);
		getTime(startTime, this->breakMins);  // add break mins
		// initialize end time for next task
		strcpy(endTime, startTime);
	}	
}

// This is an implementation of the printSched() function in Schedule class.
// Prints the schedule (task time interval and task name of all tasks).
void Schedule::printSched() {
	int i;
	if (taskCount == 0) {  // if empty schedule
		printf("\nSchedule is empty.\n");
		return;
	}
	// Print tasks and their details
	printf("\n----------------------SCHEDULE----------------------\n ");
	padStr(20, "Time");
	padStr(35, "Task");
	printf("\n ");
	// Tasks parallel arrays Calling:
	//    taskNumbers[x][y] where x=taskNum; y=0 for task priority; y=1 for task duration.
	//    taskDetails[x][y] where x=taskNum; y=0 for task name; y=1 for task's time interval.
	for (i=0; i<this->taskCount; i++) {
		padStr(20, taskDetails[i][1]);  // print task time interval
		padStr(35, taskDetails[i][0]);  // print task name
		if (i != taskCount-1) {
			printf("\n       %d mins BREAK", this->breakMins);
		}
		printf("\n ");
	}
}

// This is an implementation of the sortTasksByPriority() function in Schedule class.
// Sorts the tasks in the tasks parallel arrays by their priority using selection sort.
// Task priority is from 1-3 (sort in acesnding) given by taskNumbers[taskCount][0]. 
// Adapted from https://www.geeksforgeeks.org/selection-sort/
void Schedule::sortTasksByPriority() {
	int i, j, min;
	for (i = 0; i < this->taskCount-1; i++) {  // start of unsorted array
		min = i;  // initialize index of minimum value
		// Loop through each value in the unsorted array (excluding first value) to 
		// compare them to the first value in the unsorted array for finding minimun value.
		for (j = i+1; j < this->taskCount; j++) {
			// If a smaller value is found, update min index
			if (this->taskNumbers[j][0] < taskNumbers[min][0]) min = j;
		}
		// Swap min's task with the first task in the unsorted array
		swapIntValues(&taskNumbers[i][0], &taskNumbers[min][0]);  // swap task priority
		swapIntValues(&taskNumbers[i][1], &taskNumbers[min][1]);  // swap task duration
		swapStrValues(taskDetails[i][0], taskDetails[min][0]);  // swap task name
		swapStrValues(taskDetails[i][1], taskDetails[min][1]);  // swap task time interval
	}
}

// This is an implementation of the parsePrimeTime() function in Schedule class.
// Gets the preset value of prime time according to the following preset and 
// set it to the specified string (T):
//   "Morning" 05:00 AM — 11:59 AM; "Afternoon" 12:00 PM — 04:59 PM
//   "Evening" 05:00 PM — 07:59 PM; "Night" 08:00 PM — 04:59 AM
void Schedule::parsePrimeTime(char *T) {
	if (strcmpi(primeTime, "Morning") == 0) {
		strcpy(T, "05:00-11:59");
	} else if (strcmpi(primeTime, "Afternoon") == 0) {
		strcpy(T, "12:00-16:59");
	} else if (strcmpi(primeTime, "Evening") == 0) {
		strcpy(T, "17:00-19:59");
	} else if (strcmpi(primeTime, "Night") == 0) {
		strcpy(T, "20:00-04:59");
	}
}

// This is an implementation of the createDynamicTaskDetails() function in Schedule class.
// It dynamically creates a 2D string and integer array capable of storing 30 tasks
// and 2 details for each task (30 rows and 2 columns), and 50 characters per string. 
// Said string array will be pointed to by taskDetails and taskNumbers.
void Schedule::createDynamicTaskArrays() {
	this->taskDetails = createDynamicStrArray2D(30, 2, 50);
	this->taskNumbers = createDynamicIntArray2D(30, 2);
}

// This is an implementation of the freeDynamicTaskDetails() function in Schedule class.
// It free up the memory dynamically allocated to the tasks parallel arrays
// (task details & task numbers) and set them back to NULL.
void Schedule::freeDynamicTaskArrays() {
	// free up memory allocated to taskDetails array
	freeDynamicStrArray2D(this->taskDetails, 30);
	this->taskDetails = NULL;
	// delete memory pointed to by taskPriority array
	freeDynamicIntArray2D(this->taskNumbers, 30);
	this->taskNumbers = NULL;
}

