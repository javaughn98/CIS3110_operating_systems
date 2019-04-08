Name: Javaughn Burke
Course: CIS 3110
Date: February 7, 2019


Assignment 1


  For my assignment I created a shell that is supposed to mimic the bash
shell. To start the program you have to run the ish.c file using ./ish.
the program should first initialize the shell so that it could function properly,
once the shell has initialized you can type the command exit to end the program at anytime.
The program should execute any of the internal commands entered, it also checks for
any created commands such as gcd, lcm, args and exit.
    The function that I created lcm should calculate the lowest multiple between two numbers
and returns the lcm between any two numbers. This uses both hexadecimal and decimal numbers,
to use command simply type: lcm 20 30. This function makes use of the gcd function that
was required in the outline which can be run by typing the command: gcd 28 4.
The ls -l >foo.txt command should create a text file and store the output of the command
in the named text file. the sort < foo.txt will not work unless the file is already
created. this command should sort the lines in the text file and output the sorted
info, however the code stored in the file will not be overwritten with the sorted lines.
