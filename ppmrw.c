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
#include <ctype.h>

// TYPE DEFINITIONS
typedef struct Header {
	char magic_number[2];
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
int peek_next_char();
void handle_comment();

// GLOBAL VARIABLES
FILE* input_file;
Header file_header;


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
	input_file = fopen(argv[2], "r");

	error_check = read_header();
	if(error_check) {
		fclose(input_file);
		return 1;
	}

	fclose(input_file);
	return 0;
}

/******************************************************************************
* Read Header function
* Purpose: Reads the header of the input file and store it in a global variable
* Return value:
*				0 if file read has a properly formatted .ppm header
*				1 if the file is not ppm format
*				2 if the file is neither P3 nor P6
*				3 if the file has a width less than 1px
*				4 if the file has a height less than 1px
*				5 if the maximum color value is outside of the range [1, 65535]
******************************************************************************/
int read_header() {

	// read in the magic number
	file_header.magic_number[0] = fgetc(input_file);
	// check if first character is P
	if(file_header.magic_number[0] != 'P') {
		fprintf(stderr, "Invalid File:\n\tFile is not in the PPM format\n\nClosing Program\n");
		return 1;
	}
	file_header.magic_number[1] = fgetc(input_file);
	// check if second character is 3 or 6
	if(file_header.magic_number[1] != '3' && file_header.magic_number[1] != '6') {
		fprintf(stderr, "Invalid File:\n\tFile is neither P3 or P6\n\nClosing Program\n");
		return 2;
	}
	int i;
	for(i = 0; i < 3; i++) {
		// walk through whitespace and comments and store none of it
		while(isspace(peek_next_char())) {
			fgetc(input_file);
			if(peek_next_char() == '#') { handle_comment(); }
		}
		// get the width data
		if(i == 0) {
			fscanf(input_file, "%d", &file_header.width);
			// check if Width value is valid
			if(file_header.width < 1) {
				fprintf(stderr, "Invalid image width, must be at least 1px wide\n\nClosing Program\n");
				return 3;
			}
		}
		// get the height data
		if(i == 1) {
			fscanf(input_file, "%d", &file_header.height);
			// check if height value is valid
			if(file_header.height < 1) {
				fprintf(stderr, "Invalid image height, must be at least 1px high\n\nClosing Program\n");
				return 4;
			}
		}
		// get the maximum value data
		if(i == 2) {
			fscanf(input_file, "%d", &file_header.max_val);
			// check if the maximum color value is valid
			if(file_header.max_val < 1 || file_header.max_val > 65535) {
				fprintf(stderr, "Invalid maximum color value, must be between 1 and 65535\n\nClosing Program\n");
				return 5;
			}
		}
		
	}
	printf("Magic Number: %s\nWidth: %d\nHeight: %d\nMaximum Color Value: %d\n", file_header.magic_number, file_header.width, file_header.height, file_header.max_val);
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

/******************************************************************************
* Peek Next Character function
* Purpose: Check what the next character in a stream is
* Return Value: Returns the next character in a stream.
******************************************************************************/
int peek_next_char() {
	int c;
	c = fgetc(input_file);
	ungetc(c, input_file);
	return c;
}

/******************************************************************************
* Handle Comment function
* Purpose: move the file pointer past the end of a comment
******************************************************************************/
void handle_comment() {
	while(peek_next_char() != '\n') {
		fgetc(input_file);
	}
}