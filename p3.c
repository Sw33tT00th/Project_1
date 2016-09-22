#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "helpers.h"
#include "header.h"
#include "p3.h"


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