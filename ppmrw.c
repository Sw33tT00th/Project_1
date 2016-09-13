/******************************************************************************
* Author:		John Loudon
* Created on:	9/12/2016
* Git Repo:		https://github.com/Sw33tT00th/Project_1
******************************************************************************/

// IMPORT STATEMENTS
#import <stdio.h>
#import <stdlib.h>

// TYPE DEFINITIONS
typedef struct Header {
	int width;
	int height;
	int max_val;
} Header;

// GLOBAL VARIABLES
Header file_header;

/******************************************************************************
* Main function
* Usage: Must call this application with 3 arguments:
* 		- the type of output desired (3 or 6 for P3 or P6 respectivly)
* 		- an input file in the .ppm format
*		- an output destination that ends with the .ppm file extention
******************************************************************************/
int main(int argc, char *argv[]) {
	return 0;
}

/******************************************************************************
* Read Header function
* Purpose: Reads the header of the input file and store it in a global variable
* Return values:
*		- returns 0 if file read has a properly formatted .ppm header
*		- returns 1 if the magic number is neither P3 or P6
*		- returns 2 if either the dimensions or max val are missing
******************************************************************************/
int read_header(FILE *fp) {
	return 0;
}