#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

  int exit = 0;
  
  while(!exit){
    char *input[256];
    printf(">>");
    fgets(input,sizeof(input),stdin);
    
    if (!strcmp(input,"\n")) {
      printf("Please enter a valid argument!\n");
      return 0;
    }
    
    strtok(input,"\n");
  

    char *s = input;
    char *commands[10];
  
    int i = 0;
    while (s) {
      commands[i] = strsep(&s," ");
      i++;
    }

    if(!strcmp(command[0],"exit")){
      exit = 1;
      break;
    }


    execvp(commands[0],commands);
  }

  return 0;

  
}
