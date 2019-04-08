#include "../include/header.h"




  int main(int argc, char **argv) {

    // loads the configuration files required
    // initialize
    initializeShell();

    // reads the input form the command line
    //run the command loop
    loop();


    return 0;
  }






  void initializeShell() {
    int interractive = 0;
    int terminal = 0;
    pid_t pgid;
    struct termios terminalModes;


    printf(" ===================================\n");
    printf(" STARTING SHELL\n");
    printf(" ===================================\n");

      // purpose is to initialize the shell upon opening
    if(interractive){
      terminal = STDIN_FILENO;
      interractive = isatty(terminal);

      while(tcgetpgrp(terminal) != (pgid = getpgrp())) {
        kill(-pgid, SIGTTIN);
      }


      signal(SIGINT, SIG_IGN);
      signal(SIGQUIT,SIG_IGN);
      signal(SIGTSTP,SIG_IGN);
      signal(SIGTTIN,SIG_IGN);
      signal(SIGTTOU,SIG_IGN);
      signal(SIGCHLD,SIG_IGN);

      pgid = getpid();
      if(setpgid(pgid, pgid) < 0) {
        perror("ERROR: shell couldnt be placed in its own process group");
        exit(1);
      }

      tcsetpgrp(terminal, pgid);

      tcgetattr(terminal, &terminalModes);
    }
  }






  void loop() {

    char **arguments;
    int stats;
    uid_t uid;
    struct passwd *pw;
    char hostName[1024] = "";


    do {
      uid = getuid();
      pw = getpwuid(uid);
      gethostname(hostName, 1023);
      if(getuid() == 0) {
        if(pw) {
          printf("[%s@%s]#",pw->pw_name, hostName);
        }
      }
      else {
        if(pw) {
          printf("[%s@%s]$",pw->pw_name, hostName);
        }
      }
      arguments = getln();
      stats = executeCommand(arguments);

    }while(stats);
  }






  int executeCommand(char **parsedInput) {
    int i;
    int blk = 0;
    int in;
    int out;
    char *inputFile;
    char *outputFile;

    if(parsedInput[0] == NULL) {
      // The command that was entered was empty
      printf("Please enter a command!! Command Line Empty");
      return 1;
    }
    for(i = 0; i < numOfBuiltIns(); i++) {
      if(strcmp(parsedInput[0], builtIns[i]) == 0) {
        return (*builtInFunc[i])(parsedInput);
      }
    }

    // assigns 1 or 0 to block if & is present
    blk = (checkAmpersand(parsedInput) == 0);

    // Check for redirected input
    in = redirectedInput(parsedInput, &inputFile);
    switch(in) {

      case -1:
        printf("Error!!! Redirecting input.\n");
        return 1;

      case 0:
        break;

      case 1:
        printf("Redirecting input to %s\n", inputFile);
    }


    // Check for redirected outputFile
    out = redirectedOutput(parsedInput, &outputFile);
    switch(out) {

      case -1:
        printf("Error!!! Redirection output.\n");
        return 1;

      case 0:
        break;

      case 1:
        printf("Redirecting output to %s\n", outputFile);
        break;
    }




    return finalExe(parsedInput, blk, in, out, inputFile, outputFile);
  }





  int finalExe(char **parsedInput, int blk, int in, int out, char *inputFile, char *outputFile) {
    pid_t pid, pid1;
    int state;
    int fd[2];


    pid = fork();
    if(pid < 0) {
      //forking failed
      perror("Could not fork!!\n");
    }
    else if(pid == 0){
      //Child process
      if(in) {
        readFromFile(fd[0], inputFile, parsedInput);
        return 1;
      }

      if(out) {
        close(fd[0]);
        writeToFile(fd[1], outputFile, parsedInput);
        return 1;
      }
      pid1 = execvp(parsedInput[0], parsedInput);
      exit(1);
    }

    else{
      // Parent Process
      do{
        if(blk) {
          pid1 = waitpid(pid, &state, WUNTRACED);
        }
      }while(!WIFEXITED(state) && !WIFSIGNALED(state));
    }

    return 1;
  }







  int cmd_exit(char **arguments) {
    printf("Exiting the shell.....Bye!!\n");
    exit(EXIT_SUCCESS);
  }



  int cmd_gcd(char **arguments) {
    int num1;
    int num2;

    num1 = atoi(arguments[1]);
    num2 = atoi(arguments[2]);

    if(num1 == 0 && num2 == 0) {
      num1 = toDec(arguments[1]);
      num2 = toDec(arguments[2]);
    }
    else if(num1 != 0 && num2 == 0) {
      num2 = toDec(arguments[2]);
    }
    else if(num1 == 0 && num2 != 0) {
      num1 = toDec(arguments[1]);
    }


    printf("GCD(%s,%s) = %d\n", arguments[1], arguments[2], gcd(num1, num2));
    return 1;
  }



  int gcd(int num1, int num2) {
    while (num1 != num2) {
      if(num1 > num2) {
        num1 -= num2;
      }
      else {
        num2 -= num1;
      }
    }
    return num1;
  }




  int toDec(char *value) {
    int decimalNum = 0;
    int base = 1;
    int length;
    int i;

    if(value[0] == 0 && value[2] == '\0') {
      return 0;
    }
    else {
      length = strlen(value);
      for(i = length - 1; i >= 0; i--) {
        if(value[i] >= '0' && value[i] <= '9') {
          decimalNum += (value[i] - 48) * base;
          base = base * 16;
        }
        else if(value[i] >= 'a' && value[i] <= 'f') {
          decimalNum += (value[i] - 87) * base;
          base = base * 16;
        }
        else if(value[i] >= 'A' && value[i] <= 'F') {
          decimalNum += (value[i] - 55) * base;
          base = base * 16;
        }
      }
    }
    return decimalNum;
  }




  int cmd_args(char **arguments) {
    int i = 0;
    int j;
    char str[50] = "";
    int k = 1;
    int s = 0;

    while(arguments[k] != NULL) {

      if(arguments[k][0] == '\"' && s == 0) {
        s = 1;
        j = k;
      }
      else if(arguments[k][0] == '\"' && s == 1) {
        strcat(str, arguments[k]);
        free(arguments[k]);
        strcpy(arguments[j], str);
        s = 0;
        arguments[j + 1] = NULL;
      }
      if(s == 1) {
        strcat(str, arguments[k]);
        if(j != k){
        free(arguments[k]);
        }
      }
      k++;
    }
    while(arguments[i + 1] != NULL) {
      i++;
    }
    printf("argc = %d,args =", i);
    for(j = 0; j < i; j++) {
      if(j == i - 1) {
        printf(" %s\n", arguments[j + 1]);
      }
      else {
        printf(" %s,", arguments[j + 1]);
      }
    }


    return 1;
  }






  int redirectedInput(char **argument, char **file) {
    int i = 0;
    int j;


    while(argument[i] != NULL) {
      if(strcmp(argument[i], "<") == 0){
        free(argument[i]);

        if(argument[i + 1] != NULL) {
          *file = argument[i + 1];
        }
        else {
          return -1;
        }

        for(j = i; argument[j - 1] != NULL; j++) {
          argument[j] = argument[j + 2];
        }
        return 1;
      }
      i++;
    }
    return 0;
  }






  int redirectedOutput(char **argument, char **file) {
    int i = 0;
    int j;


    while(argument[i] != NULL){
      if(strcmp(argument[i], ">") == 0) {
        free(argument[i]);

        if(argument[i + 1] != NULL) {
          *file = argument[i + 1];
        }
        else {
          return -1;
        }

        for(j = i ; argument[j - 1] != NULL; j++) {
          argument[j] = argument[j + 2];
        }
        return 1;
      }
      i++;
    }
    return 0;
  }





// this function checks if the last string is &
  int checkAmpersand(char **parsedInput) {
    int i;

    for(i = 0; parsedInput[i + 1] != NULL; i++);

    if(strcmp(parsedInput[i], "&") == 0) {
      free(parsedInput[i]);
      parsedInput[i] = NULL;
      return 1;
    }
    else {
      return 0;
    }
    return 0;
  }







  int numOfBuiltIns() {
    return sizeof(builtIns) / sizeof(char *);
  }





  void writeToFile (int fd, char *fileName, char **arguments) {
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  FILE *file;

  if((fd = creat(fileName, mode)) < 0) {
    printf("error creat()\n");
    return;
  }
  else {
    if((file = fdopen(fd, "w")) == NULL) {
      printf("error fdopen()\n");
      close(fd);
    }
    else {
      freopen(fileName, "w", stdout);
      execvp(arguments[0], arguments);
      fclose(file);
    }
  }


  }




  void readFromFile(int fd, char *fileName, char **arguments) {
  FILE *file;
  int i;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  if((fd = open(fileName, mode)) < 0) {
    printf("fopen() error\n");
    return;
  }
  else {
    if((file = fdopen(fd, "r")) == NULL) {
      printf("error fdopen()\n");
      close(fd);
    }
    else {
      freopen(fileName, "r", stdin);
      printf("%s\n", arguments[0]);
      execvp(arguments[0], arguments);
      while ((i = fgetc (file)) != EOF)
        putchar(i);
      fclose(file);

    }
  }

  }

int cmd_lcm(char **parsedInput) {
  int num1;
  int num2;

  num1 = atoi(parsedInput[1]);
  num2 = atoi(parsedInput[2]);

  if(num1 == 0 && num2 == 0) {
    num1 = toDec(parsedInput[1]);
    num2 = toDec(parsedInput[2]);
  }
  else if(num1 != 0 && num2 == 0) {
    num2 = toDec(parsedInput[2]);
  }
  else if(num1 == 0 && num2 != 0) {
    num1 = toDec(parsedInput[1]);
  }


  printf("LCM(%s,%s) = %d\n", parsedInput[1], parsedInput[2], lcm(num1, num2));

  return 1;
}



int lcm(int num1, int num2) {

  if(num1 == 0|| num2 == 0) {
    printf("zero value detected. please enter a different value\n");
    return 0;
  }

  return (num1 * num2) / gcd(num1, num2);
}

//const char *mypath[] = { "./",
//"/usr/bin/", "/bin/", NULL
//};

//while (...)
//{
/* Wait for input */

//printf ("prompt> ");
//fgets (...);

/* Parse input */

//while (( ... = strsep (...)) != NULL)
//{
//...
//}

/* If necessary locate executable using mypath array */

/* Launch executable */

  //if (fork () == 0) {
  // Child process

  //}
  //else {

  //} // end of if

//}
