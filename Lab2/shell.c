#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

#define TRUE 1
#define SUCCESS 1;
#define FAIL -1;
#define clear() printf("\033[H\033[J")
#define LIM 256 // max amount of tokens
#define LINE 1024 // max amount of characters in user input
char* currentDirectory;

void initShell(){

}

void type_prompt(){
  char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
	printf("%s@%s %s > ", getenv("LOGNAME"), hostn, getcwd(currentDirectory, 1024));
}

void print_help(){
  printf("\nThe following commands can be used: \n");
  printf("1. exit\n");
  printf("2. help\n");
  printf("3. clear\n");
  printf("4. programs with pipes and IO redirection.\n\n");
}

void handle_piping(){

}

void handle_IO(){

}

int execute_command(char * usrargs[]){
  char * arg1 = usrargs[0];
  int execbg = 0;
  int i = 0;

  // User entered exit
  if(strcmp(arg1,"exit")==0){
    clear();
    exit(0);
  }
  // User entered help
  else if(strcmp(arg1,"help")==0){
    print_help();
  }
  // User entered clear
  else if(strcmp(arg1,"clear")==0){
    clear();
  }

  // It isn't a on of the single commands
  else{
    // Loop over all input arguments
    while (usrargs[i]!= NULL){
      // When | is detected, user wants to pipe
      if(strcmp(usrargs[i],"|")==0){
        handle_piping();
        return SUCCESS;
      }
      // When & is detected user want to run in background
      else if(strcmp(usrargs[i],"&")==0){
        execbg = 1;
        break;
      }
      // When < is detected it means the user wants input AND ouput redirection.
      else if(strcmp(usrargs[i],"<")==0){
        if(usrargs[i+1]==NULL||usrargs[i+2]==NULL||usrargs[i+3]==NULL){
          printf("Not enough input arguments for <\n");
          return FAIL;
        }
        else{
          if(strcmp(usrargs[3],">")!=0){
            printf("The %s argument was invalid.\n",usrargs[3]);
            return FAIL;
          }
        }
        handle_IO();
        return SUCCESS;
      }
      // When > is detected it means the user wants output redirection.
      else if(strcmp(usrargs[i],">")==0){
        if(usrargs[i+1]==NULL){
          printf("Not enough input arguments for >\n");
          return FAIL;
        }
        handle_IO();
        return SUCCESS;
      }
      i++;
    }
  }
  execute_program(usrargs[0],execbg);
  return 0;
}

int main (int argc, char *argv[]){
  char line[LINE];
  char *tokens[LIM];
  int amountTokens;

  initShell();
  while (TRUE) {
    type_prompt();
    /* READING COMMAND */
    // Empty the line
    memset(line, '\0', LINE);
    // Wait for the user input and put it in the 'line'
    fgets(line, LINE, stdin);
    // Get the first token
    tokens[0] = strtok(line," \n\t");

    // If nothing was given, skip this iteration of the loop
    if(tokens[0]==NULL){
      continue;
    }

    amountTokens = 0;
    // Go over all the tokens until newline or tab
    while(tokens[amountTokens]!=NULL){
      amountTokens++;
      tokens[amountTokens] = strtok(NULL, " \n\t");
    }
    execute_command(tokens);
  }
}
