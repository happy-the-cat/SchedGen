/***************************************************************
Filename: Support.h
Description:
 This header file contains all the supporting and general functions for SchedGen.
 Specifically, it holds the function declaration for the following:
 getting a delimited item in a file, getting the time minutes after a given time, 
 creating 2D int and 3D char (2D string) arrays, freeing said arrays, 
 swapping int and string, and padding int and string outputs.
***************************************************************/

// Header guards (to prevent double inclusion)
#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*** Supporting functions declaration ***/
void getDelimitedItem(char s[], char d, FILE*fp);
void getTime(char* T, int M);
int **createDynamicIntArray2D(int n, int m);
void freeDynamicIntArray2D(int **arr, int n);
char **createDynamicStrArray(int x, int y);
char ***createDynamicStrArray2D(int x, int y, int z);
void freeDynamicStrArray2D(char ***arr, int n);
void swapIntValues(int *x, int *y);
void swapStrValues(char *a, char *b);
void padStr(int w, const char* s);
void padInt(int w, int x);

#endif  // end header guard system
