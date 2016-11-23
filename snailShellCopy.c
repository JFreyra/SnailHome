#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {

  //int exit = 0;
  
  while(1){
    char input[256];
    char path[666];
    getcwd(path, 666);
    printf("%s >>", path);
    fgets(input,sizeof(input),stdin);
    
    if (!strcmp(input,"\n")) {
      printf("Please enter a valid argument!\n");
      return 0;
    }

    /*
    char * newLine=strchr(input, '\n');
    if (newLine)
      *newLine = NULL; */
    strtok(input,"\n");
  

    char *s = input;
    char *commands[10];
  
    int i = 0;
    while (s) {
      commands[i] = strsep(&s," ");
      //printf("%s \n", commands[i]);
      i++;
    }
    commands[i]=NULL;

    if(!strcmp(commands[0],"exit")){
      exit(0);
    }

    if (!strcmp(commands[0],"cd")){
      printf("cd ing");
      return 0;
    }
      
    int pid=fork();

    if (!pid){
      execvp(commands[0],commands);
      printf("%s \n", strerror(errno));
      //printf("hi\n");
    }
    if (pid)
      waitpid(pid);

  }
  
}
