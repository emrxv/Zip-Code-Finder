/** Program demonstrating parsing and linked lists. This program 
 * supports zip code lookup for any town in Massachusetts
 *
 * @author Blake Nelson Emre Sunar
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <forward_list>
#include <iostream>
#include <ctype.h>
#include "zipfed.hpp"
#include "zipfilter.h"

// size of fully qualified path/file name with null terminator
#define SZ_FILENAME (129)

/** Function to read a line from the output of fed2cs2303 and return pointer to the 
 * line of data represented as a C-String (i.e. NULL terminated string)
 *
 * @lineptr is a pointer to the dynamically allocated buffer to fill.
 *   If the pointer is NULL, a buffer will be allocated to hold the line.
 *   If the buffer is too small to hold the line, realloc will be called
 *   to allocate a larger buffer. If the buffer is reallocated, n will
 *   be set to new size.
 * @n is a pointer to the size of the buffer pointer to be lineptr. If
 *   memory is reallocated for lineptr, the size of n will be incremented
 *   accordingly. The parameter n will always specify the size of buffer
 *   pointed at by lineptr.
 * @fptr is the FILE pointer to the open file to read.
 * @return number bytes (chars) read or -1 on error
 */
ssize_t readln_cs2303 (char **lineptr, size_t *n, FILE *stream) {
  ssize_t bytes_read = -1;  // -1 will signify error reading line
  const int delim = '\n';   // each line terminated with \r\n

  // Verify the file is open
  if (stream == NULL) {
    return (ssize_t) -1;
  }

  bytes_read = getdelim (lineptr, n, delim, stream);


  for (int i = bytes_read - 1; i >= 0; i--) {
    if ((*lineptr)[i] == '\n') {
      (*lineptr)[i] = '\0';
    }
  }
    
  return bytes_read;
}


/** program to find the zip code for any town in Massachusetts
 *
 * @param argc is the number of field on command line - should be 1
 * @param argv is array of strings entered on command line - prog name
 * @return 0 for success
 */

int main (int argc, char *argv[]) {
   if (argc != 2) {
    fprintf(stderr, "usage: %s input_csv_file\n", argv[0]);
    return -1;
  }

  char infile[SZ_FILENAME];  // Path/name of input file
  FILE *fdIn;

  ssize_t chars_read;        // number of chars read for line of input
  char *inbuf = NULL;        // input file has 1 record per line to buffer
  size_t sz_inbuf = 0;       // current size of the input record

  std::forward_list<Zipfed *> llist; // singly linked list of pointers to Zipfed instances

   if (argc != 2) {
    fprintf(stderr, "usage: %s input_csv_file\n", argv[0]);
    return -1;
  }

  strncpy(infile, argv[1], SZ_FILENAME-1);

  /* Open input and output files - return error on failure
   * input for reading. output for writing
   */
  fdIn = fopen(infile, "r");
  if (fdIn == NULL) {
    fprintf(stderr, "cannot open %s for input - exiting\n", infile);
    return -2;
  }

  /* Now a loop to read each line of the input file formatted and structured
   * Then, each line is loaded into our zip code data structure
   * 
   *
   * Notes: 
   *  - each line of the input is a zip code record.
   *  - read until EOF on input or error
   */

  // Loop to process each line of zip code data in CSV format
  while ((chars_read = readln_cs2303 (&inbuf, &sz_inbuf, fdIn)) != EOF) {
    if (chars_read == 0) {  // not EOF, but nothing to process
      continue;
    }

    // write the buffer to stdout for debugging
#ifdef DEBUG
    printf("inbuf: %s\n", inbuf);
#endif
    
    // for each input line we create a new object instance of Zipfed, have the object
    // parse the input record and initialize itself with the parse result
    // Nonzero return indicates error parsing
    Zipfed *pZipfed = new Zipfed();
    if(pZipfed->parse_zip_cs2303(inbuf) != 0) {
      fprintf (stderr, "failed to process input record - exiting\n");
      fclose (fdIn);
      return -3;
    }

    // Add the zip code record into the linked list
    llist.push_front(pZipfed);
  }

  // Sort the list by city name
  llist.sort(&Zipfed::comparator_function);

    std::cout << "Enter a city to look up zip codes (Ctrl-D to exit):\n";
    std::string city;
    std::string zipcode;

    while(std::getline(std::cin, city)) {
    	std::cout << "Zip codes for " << city << ":";

    	bool found = false;

    	for (auto& zipcode : llist) {
    		if (zipcode->getCity() == city) {
    			found = true;
    			std::cout << " " << zipcode->getZip();
    		} 
    	}
    	
    	std::cout << "\n";

        if (std::cin.eof()) {
            break;
        }
         if (!found) {
    		std::cout << "no zip codes found \n";
        }
    }


/*
  // Now iterate through the list to make sure it's all there
  // zipprint will print the struct to output stream
  for(std::forward_list<Zipfed *>::iterator it = llist.begin(); it != llist.end(); it++) {
    Zipfed * pTmpZipfed = *it;
      pTmpZipfed->print();
    
  }
*/

  /* Free memory before exiting
   * keep deleting the head until list is empty.
   * this code is responsible for freeing the zip code data struct
   * the linked list will free the node form the linked list
   */
  while (!llist.empty()) {
    llist.pop_front();
  }
    
  return 0;


}
