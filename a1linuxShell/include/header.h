#ifndef HEADER_H
#define HEADER_H

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <termios.h>
#include <pwd.h>
#include <fcntl.h>



  void loop();
  int gcd(int num1, int num2);
  int lcm(int num1, int num2);
  int toDec(char *value);
  int executeCommand(char **parsedInput);
  int redirectedOutput(char **argument, char **file);
  int redirectedInput(char **argument, char **file);
  int checkAmpersand(char **parsedInput);
  int cmd_exit(char **arguments);
  void readFromFile(int fd, char *fileName, char **arguments);
  void writeToFile(int fd, char *fileName, char **arguments);
  int numOfBuiltIns();
  void initializeShell();
  int cmd_lcm(char **parsedInput);
  int cmd_args(char **parsedInput);
  int cmd_gcd(char **parsedInput);
  int finalExe(char **parsedInput, int blk, int in, int out, char *inputFile, char *outputFile);
  char **getln();
  char *builtIns[] = {"exit", "gcd", "args", "lcm"};
  int (*builtInFunc[]) (char **) = { &cmd_exit, &cmd_gcd, &cmd_args, &cmd_lcm };













#endif
