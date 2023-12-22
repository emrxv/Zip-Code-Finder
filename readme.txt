Emre Sunar
esunar

To compile:

1 Open the terminal
2 Type "make" to compile the C files
3 Type "make docs" to ensure that doxygen copies the html files into the proper directory
4 Execute the programs by using the commands indicated below

fed2cs2303.C

Main:

usage: ./fed2cs2303 input_csv_file output_csv_file

The program will parse the federal zip code file and write the parsed data onto the csv file 
specified, ready to use the zipcode method on.

zipcode.c 

Main:

usage: ./zipcode input_csv_file

The program will prompt you to enter in cities/towns in Massachusetts, and it will output 
the corresponding zip codes for the city or town. When you are done finding zip codes,
you may type <Ctrl-d> to exit the program.
