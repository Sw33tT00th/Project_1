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
int read_header(FILE* input_file, Header *file_header);
int write_header(FILE* output_file, char *format, Header file_header);
int read_p3(FILE* input_file, Header file_header, Pixel *body_content);
int write_p3(FILE* output_file, Header file_header, Pixel *body_content);
int read_p6(char *file_name, FILE* input_file, Header file_header, Pixel *body_content);
int write_p6(FILE* output_file, Header file_header, Pixel *body_content);
int check_for_valid_arguments(int argc, char *argv[]);
int file_exists(char *file_name);
int peek_next_char(FILE* input_file);
void handle_comment(FILE* input_file);


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
* READ HEADER
* Purpose: Reads the header of the input file and store it in a global variable
* Return value:
*				0 if file read has a properly formatted .ppm header
*				1 if the file is not ppm format
*				2 if the file is neither P3 nor P6
*				3 if the file has a width less than 1px
*				4 if the file has a height less than 1px
*				5 if the maximum color value is outside of the range [1, 65535]
******************************************************************************/
int read_header(FILE* input_file, Header *file_header) {
	// read in the magic number
	file_header->magic_number[0] = fgetc(input_file);
	// check if first character is P
	if(file_header->magic_number[0] != 'P') {
		fprintf(stderr, "ERROR: Invalid File:\n\tFile is not in the PPM format\n\nClosing Program\n");
		return 1;
	}
	file_header->magic_number[1] = fgetc(input_file);
	// check if second character is 3 or 6
	if(file_header->magic_number[1] != '3' && file_header->magic_number[1] != '6') {
		fprintf(stderr, "ERROR: Invalid File:\n\tFile is neither P3 or P6\n\nClosing Program\n");
		return 2;
	}
	int i;
	for(i = 0; i < 3; i++) {
		// walk through whitespace and comments and store none of it
		while(isspace(peek_next_char(input_file))) {
			fgetc(input_file);
			if(peek_next_char(input_file) == '#') { handle_comment(input_file); }
		}
		// get the width data
		if(i == 0) {
			fscanf(input_file, "%d", &(file_header->width));
			// check if Width value is valid
			if(file_header->width < 1) {
				fprintf(stderr, "ERROR: Invalid image width, must be at least 1px wide\n\nClosing Program\n");
				return 3;
			}
		}
		// get the height data
		if(i == 1) {
			fscanf(input_file, "%d", &(file_header->height));
			// check if height value is valid
			if(file_header->height < 1) {
				fprintf(stderr, "ERROR: Invalid image height, must be at least 1px high\n\nClosing Program\n");
				return 4;
			}
		}
		// get the maximum value data
		if(i == 2) {
			fscanf(input_file, "%d", &(file_header->max_val));
			// check if the maximum color value is valid
			if(file_header->max_val < 1 || file_header->max_val > 65535) {
				fprintf(stderr, "ERROR: Invalid maximum color value, must be between 1 and 65535\n\nClosing Program\n");
				return 5;
			}
		}
		
	}
	// walk through whitespace and comments and store none of it
	while(isspace(peek_next_char(input_file))) {
		fgetc(input_file);
		if(peek_next_char(input_file) == '#') { handle_comment(input_file); }
	}
	//printf("Magic Number: %s\nWidth: %d\nHeight: %d\nMaximum Color Value: %d\n", file_header.magic_number, file_header.width, file_header.height, file_header.max_val);
	return 0;
}

/******************************************************************************
* WRITE HEADER
* Purpose: Writes the header information into the specified file
* Return Value:
*				0 if all went well and all data was accounted for
*				1 if the output file failed to open
******************************************************************************/
int write_header(FILE* output_file, char *format, Header file_header) {
	if (output_file == NULL) {
		fprintf(stderr, "ERROR: Output File failed to open\n\nClosing Program\n");
		return 1;
	}
	char temp[80];
	// write magic number
	sprintf(temp, "P%s\n", format);
	fputs(temp, output_file);
	// write dimensions
	sprintf(temp, "%d %d\n", file_header.width, file_header.height);
	fputs(temp, output_file);
	// write max color value
	sprintf(temp, "%d\n", file_header.max_val);
	fputs(temp, output_file);
	return 0;
}

/******************************************************************************
* READ P3
* Purpose: Reads the file as if it were a P3 formatted ppm file
* Return Value:
*				0 if the file was read completely without any errors
*				1 if the file is shorter than expected based on width x height
*				2 if the body contains invalid characters
*				3 if the value found is less than 0 or greater than max_val
******************************************************************************/
int read_p3(FILE* input_file, Header file_header, Pixel *body_content) {
	int i;
	int j;
	double temp;
	for (i = 0; i < file_header.width * file_header.height; i++) {

		for (j = 0; j < 3; j++) {
			// find the next non whitespace character
			while(isspace(peek_next_char(input_file))) {
				fgetc(input_file);
			}
			// check if it's the end of the file
			if(peek_next_char(input_file) == EOF) {
				fprintf(stderr, "ERROR: File ended earlier than expected - Missing Data\n\nClosing Program\n");
				return 1;
			}
			// check if the next value is a number
			if(!isdigit(peek_next_char(input_file))) {
				fprintf(stderr, "ERROR: Invalid data in image content\n\nClosing Program\n");
				return 2;
			}
			fscanf(input_file, "%lf", &temp);
			if(temp < 0 || temp > file_header.max_val) {
				fprintf(stderr, "ERROR: Invalid data. Data value out of range\n\nClosing Program\n");
				return 3;
			}
			temp = temp / file_header.max_val;
			if (j == 0) {
				body_content[i].r = temp;
			}
			else if (j == 1) {
				body_content[i].g = temp;
			}
			else {
				body_content[i].b = temp;
			}
		}
	}
	return 0;
}

/******************************************************************************
* WRITE P3
* Purpose: Writes the data passed in to a file in the P3 format
* Return Value:
*				0 if all went well and all data was accounted for.
******************************************************************************/
int write_p3(FILE* output_file, Header file_header, Pixel *body_content) {
	char temp[80];
	int i;
	
	for(i = 0; i < file_header.width * file_header.height; i++) {
		sprintf(temp, "%d\n%d\n%d\n", (int)(body_content[i].r * file_header.max_val), (int)(body_content[i].g * file_header.max_val), (int)(body_content[i].b * file_header.max_val));
		fputs(temp, output_file);
	}
	return 0;
}

/******************************************************************************
* READ P6
* Purpose: Reads the file as if it were a P6 formatted ppm file
* Return Value:
*				0 if the file was read completely without any errors
*				1 if the end of the file was found unexpectedly
*				2 if the value found is less than 0 or greater than max_val
******************************************************************************/
int read_p6(char *file_name, FILE* input_file, Header file_header, Pixel *body_content) {
	int size;
	int first_byte;
	int second_byte;

	// check how many bytes need to be read
	if (file_header.max_val < 256) {
		size = 1;
	}
	else if (file_header.max_val >= 256) {
		size = 2;
	}
	// close the file and reopen it as binary in the same place
	long int location = ftell(input_file);
	fclose(input_file);
	input_file = fopen(file_name, "rb");
	fseek(input_file, location, SEEK_SET);
	// main read loop
	int i;
	int j;
	for(i = 0; i < (file_header.width * file_header.height); i++) {
		for(j = 0; j < 3; j++) {
			// check if it's the end of the file
			if(peek_next_char(input_file) == EOF) {
				fprintf(stderr, "ERROR: File ended earlier than expected - Missing Data\n\nClosing Program\n");
				printf("%d\n", i);
				return 1;
			}
			first_byte = fgetc(input_file);
			if(size == 1) {
				if(first_byte < 0 || first_byte > file_header.max_val) {
					fprintf(stderr, "ERROR: Invalid data. Data value out of range\n\nClosing Program\n");
					return 2;
				}
			}
			if(size == 2) {
				first_byte = first_byte << 8; // left shift the first byte
				// check if it's the end of the file
				if(peek_next_char(input_file) == EOF) {
					fprintf(stderr, "ERROR: File ended earlier than expected - Missing Data\n\nClosing Program\n");
					return 1;
				}
				second_byte = fgetc(input_file);
				first_byte = first_byte + second_byte;
				if(first_byte < 0 || first_byte > file_header.max_val) {
					fprintf(stderr, "ERROR: Invalid data. Data value out of range\n\nClosing Program\n");
					return 2;
				}
			}
			if(j == 0) {
				body_content[i].r = (double)first_byte / file_header.max_val;
			}
			else if(j == 1) {
				body_content[i].g = (double)first_byte / file_header.max_val;
			}
			else {
				body_content[i].b = (double)first_byte / file_header.max_val;
			}
		}
	}
	fclose(input_file);
	input_file = fopen(file_name, "r");
	return 0;
}

/******************************************************************************
* WRITE P6
* Purpose: Writes the data passed in to a file in the P6 format
* Return Value:
*				0 if all went well and all data was accounted for.
******************************************************************************/
int write_p6(FILE* output_file, Header file_header, Pixel *body_content) {
	int i;
	int j;
	int val;
	char temp1;
	char temp2;
	
	for(i = 0; i < file_header.width * file_header.height; i++) {

		for(j = 0; j < 3; j++) {
			if(j == 0) {
				val = (int)(body_content[i].r * file_header.max_val);
			}
			else if (j == 1) {
				val = (int)(body_content[i].g * file_header.max_val);	
			}
			else {
				val = (int)(body_content[i].b * file_header.max_val);
			}

			if(file_header.max_val > 255) {
				temp1 = (char)(val >> 8);
				fwrite(&temp1, 1, sizeof(temp1), output_file);
				// clear bits to the left of the least significant byte
				val = val << 24;
				val = val >> 24;
			}
			temp2 = (char)val;
			fwrite(&temp2, 1, sizeof(temp2), output_file);	
		}
	}
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

/******************************************************************************
* PEEK NEXT CHARACTER
* Purpose: Check what the next character in a stream is
* Return Value: Returns the next character in the input_file stream.
******************************************************************************/
int peek_next_char(FILE* input_file) {
	int c;
	c = fgetc(input_file);
	ungetc(c, input_file);
	return c;
}

/******************************************************************************
* HANDLE COMMENT
* Purpose: move the file pointer past the end of a comment
******************************************************************************/
void handle_comment(FILE* input_file) {
	while(peek_next_char(input_file) != '\n') {
		fgetc(input_file);
	}
}