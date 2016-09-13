/******************************************************************************
* Author:		John Loudon
* Created on:	9/12/2016
* Git Repo:		https://github.com/Sw33tT00th/Project_1
******************************************************************************/

// IMPORT STATEMENTS
#import <stdio.h>
#import <stdlib.h>
#import <unistd.h>
#import <string.h>

// TYPE DEFINITIONS
typedef struct Header {
	int width;
	int height;
	int max_val;
} Header;

typedef struct Pixel {
	double r;
	double g;
	double b;
} Pixel;

// FUNCTION SIGNATURES
int file_exists(char *file_name);
int check_for_valid_arguments(int argc, char *argv[]);
int read_header();

// GLOBAL VARIABLES
FILE* input_file;
Header file_header;
Pixel body[];


/******************************************************************************
* Main function
* Usage: Must call this application with 3 arguments:
* 		- the type of output desired (3 or 6 for P3 or P6 respectivly)
* 		- an input file in the .ppm format
*		- an output destination that ends with the .ppm file extention
******************************************************************************/
int main(int argc, char *argv[]) {
	int error_check;
	// ERROR CHECKING FOR INPUTS
	error_check = check_for_valid_arguments(argc, argv);
	if(error_check) { return 1; } // close if arguments are invalid.
	// check if input file actually exists
	if(!file_exists(argv[2])) {
		fprintf(stderr, "Input file does not exist\n\nClosing Program\n");
		return 1;
	}
}

/******************************************************************************
* Read Header function
* Purpose: Reads the header of the input file and store it in a global variable
* Return values:
*		- returns 0 if file read has a properly formatted .ppm header
*		- returns 1 if the magic number is neither P3 or P6
*		- returns 2 if either the dimensions or max val are missing
******************************************************************************/
int read_header() {

	return 0;
}

/******************************************************************************
* Check for Valid Arguemnts function
* Purpose: Checks the number of arguments, followed by the arguments themselves
* 		   to make sure that the arguments are what we need.
* Return Value: 0 if all arguments are accounted for and are theoretically
*				valid
*				1 if there aren't enough arguments
*				2 if the first argument isn't either 3 or 6
******************************************************************************/
int check_for_valid_arguments(int argc, char *argv[]) {
	// Not enough Inputs
	if(argc < 4) {
		fprintf(stderr, "Invalid program usage, not enough parameters\n");
		fprintf(stderr, "\tCall should be \"./ppmrw [3|6] <input_file.ppm> <output_file.ppm>\"\n\n");
		fprintf(stderr, "Closing Program\n");
		return 1;
	}
	// Invalid first argument
	if(strcmp(argv[1], "3") && strcmp(argv[1], "6")) {
		fprintf(stderr, "Invalid input in first argument, value must be 3 or 6\n\n");
		fprintf(stderr, "Closing Program\n");
		return 2;
	}
	// All Arguments pass
	return 0;
}

/******************************************************************************
* File Exists function
* Purpose: Check if a file exists
* Return Value: 1 if the provided file name exists in the file system
* 				0 if the provided file name does not exists in the file system
******************************************************************************/
int file_exists(char *file_name) {
	if( access( file_name, F_OK ) != -1 ) {
	    return 1;
	}
	return 0;
}