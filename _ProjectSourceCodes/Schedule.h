/***************************************************************
Filename: Schedule.h
Description:
 This header file contains the member declarations for Schedule class.
 Its function definitions are declared in Schedule.cpp.
 The Schedule class holds the functions relating to schedule operations 
 such as generate, open, edit, and save schedule. 
 Each schedule can have a maximum of 30 tasks.
***************************************************************/

// Header guards (to prevent double inclusion)
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <stdlib.h>

class Schedule {
  /*** attributes or data members ***/
  private:
	char username[20];  // Username of user; 
		// also the folder name where user's schedules will be stored
	char schedName[50];	// File path of schedule (file path + filename)
	float workHrs;		// Soft cap; total hours user will work for the day
	int breakMins;		// Minutes of break between each task
	int taskCount;		// Total number of tasks for the day
	
	// User's prime time to study/work (the best time they work at).
	// Prime Time Preset:
	//   "Morning" 05:00 AM — 11:59 AM; "Afternoon" 12:00 PM — 4:59 PM
	//   "Evening" 05:00 PM — 7:59 PM; "Night" 08:00 PM — 4:59 AM
	// parsePrimeTime() function will get the actual time interval, 
	// prime time stores the word equivalent only (ex. morning).
	char primeTime[15];
	
	// Task details is a pointer to a 2D string array to be 
	// dynamically allocated/freed for each new schedule. 
	// Tasks' details stores the name and time interval of each task.
	// Calling: 
	//    taskDetails[x][y] where x=rowIndex or taskIndex and y=columnIndex; 
	// 	  y=0 for task name; y=1 for task's time interval in the actual schedule.
	char ***taskDetails = NULL;
	// Task numbers, parallel array of taskDetails, points to a 2D array 
	// holding the priority and duration of each task in task details.
	// Task priority scales from 1-3 with 1 as the most prioritized and 3 as the least.
	// Calling: taskNumbers[x][y] where x=taskIndex and y=columnIndex; 
	// 			y=0 for task priority; y=1 for task duration.
	int **taskNumbers = NULL;
	// Parallel arrays taskDetails & taskNumbers are collaboratively 
	// called as tasks parallel arrays here. 

  /*** behaviors or function members ***/
  public:
	Schedule() {}  // Default empty constructor
	Schedule(const char *username); // Overloaded Constructor
	void generateSched();
	void addTask();
	void removeTask();
	void openSchedFile();
	void saveSchedFile();
	
	bool writeSchedToFile();
	bool readSchedFromFile();
	void setSchedName(const char *filename);
	void inputDetails();
	void inputTaskDetails();
	void checkTasksOrder();
	void createSched();
	void printSched();	
	void printTaskDetails();
	void createDynamicTaskArrays();
	void freeDynamicTaskArrays();
	void sortTasksByPriority();	
	void parsePrimeTime(char *T);
};

#endif  // end header guard system
