/******************************************************************************
* Author:		John Loudon
* Created on:	9/12/2016
* Git Repo:		https://github.com/Sw33tT00th/Project_1
******************************************************************************/

// printf("%c\n", peek_next_char());

// IMPORT STATEMENTS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "helpers.h"
#include "header.h"
#include "p3.h"
#include "p6.h"

// FUNCTION SIGNATURES
int check_for_valid_arguments(int argc, char *argv[]);
int file_exists(char *file_name);


/******************************************************************************
* MAIN
* Usage: Must call this application with 3 arguments:
* 		- the type of output desired (3 or 6 for P3 or P6 respectivly)
* 		- an input file in the .ppm format
*		- an output destination that ends with the .ppm file extention
******************************************************************************/
int main(int argc, char *argv[]) {
	Header file_header;
	int error_check;
	// ERROR CHECKING FOR INPUTS
	error_check = check_for_valid_arguments(argc, argv);
	if(error_check) { return 1; } // close if arguments are invalid.
	// check if input file actually exists
	if(!file_exists(argv[2])) {
		fprintf(stderr, "ERROR: Input file does not exist\n\nClosing Program\n");
		return 1;
	}
	FILE* input_file = fopen(argv[2], "r");

	error_check = read_header(input_file, &file_header);
	if(error_check) {
		fclose(input_file);
		return 1;
	}
	// initilize the body content array
	Pixel body_content[file_header.width * file_header.height];
	// check if the file input was P3 or P6
	if(strcmp(file_header.magic_number, "P3") == 0) {
		error_check = read_p3(input_file, file_header, body_content);
	}
	else {
		error_check = read_p6(argv[2], input_file, file_header, body_content);
	}

	if(error_check) {
		fclose(input_file);
		return 1;
	}
	// close input file, we're done with it at this point
	fclose(input_file);

	// Open new file for output and write header
	FILE* output_file = fopen(argv[3], "w");
	write_header(output_file, argv[1], file_header);
	// check which output format was specified
	if(strcmp(argv[1], "3") == 0) {
		error_check = write_p3(output_file, file_header, body_content);
	}
	else {
		error_check = write_p6(output_file, file_header, body_content);
	}
	// handle error response from write function
	if(error_check) {
		fclose(output_file);
		return 1;
	}

	fclose(output_file);
	return 0;
}

/******************************************************************************
* CHECK FOR VALID ARGUMENTS
* Purpose: Checks the number of arguments, followed by the arguments themselves
* 		   to make sure that the arguments are what we need.
* Return Value:
*				0 if all arguments are accounted for and are theoretically
*				valid
*				1 if there aren't enough arguments
*				2 if the first argument isn't either 3 or 6
******************************************************************************/
int check_for_valid_arguments(int argc, char *argv[]) {
	// Not enough Inputs
	if(argc < 4) {
		fprintf(stderr, "ERROR: Invalid program usage, not enough parameters\n");
		fprintf(stderr, "\tCall should be \"./ppmrw [3|6] <input_file.ppm> <output_file.ppm>\"\n\n");
		fprintf(stderr, "Closing Program\n");
		return 1;
	}
	// Invalid first argument
	if(strcmp(argv[1], "3") && strcmp(argv[1], "6")) {
		fprintf(stderr, "ERROR: Invalid input in first argument, value must be 3 or 6\n\n");
		fprintf(stderr, "Closing Program\n");
		return 2;
	}
	// All Arguments pass
	return 0;
}

/******************************************************************************
* FILE EXISTS
* Purpose: Check if a file exists
* Return Value:
* 				0 if the provided file name does not exists in the file system
*				1 if the provided file name exists in the file system
******************************************************************************/
int file_exists(char *file_name) {
	if( access( file_name, F_OK ) != -1 ) {
	    return 1;
	}
	return 0;
}