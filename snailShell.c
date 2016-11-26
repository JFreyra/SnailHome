#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// gets your current path
void cd(char *commands[]){
  
  char path[100];
  getcwd(path, 100);
  printf("This is your current path: %s\n", path);
        
  char *tempPath= path;
  char *dirs[100];//contains directories to the path
  int i = 0;
  strsep(&tempPath,"/");//gets rid of first "" element
        
  //parses through the path
  while (tempPath) {
    dirs[i] = strsep(&tempPath,"/");
    i++;
  }
  dirs[i]=0;
      
  //creates new path according to input
  char newpath[100];
  strcpy(newpath, "/");
  i=0;   
  if (!strcmp(commands[1],"..")){
    while (dirs[i]) {
      //printf("adding: %s\n",dirs[i]);
      strcat(newpath, dirs[i]);
      strcat(newpath, "/");
      //printf("the path: %s\n",newpath );
      i++;
    }    
  }else{
    while (dirs[i]) {
      // printf("adding: %s\n",dirs[i]);
      strcat(newpath, dirs[i]);
      strcat(newpath, "/");
      // printf("the path: %s\n",newpath );
      i++;
    }
  }  
  strcat(newpath, commands[1]);
      
  //changes directory
  if(chdir(newpath))
    printf("Error?: %d Cannot Change to this Directory\n", chdir(newpath));
  getcwd(path, 100);
  printf("This is your new path: %s\n", path);
  
}

void PorR(){

}

int main() {

  while(1){
    char input[256];
    char path[666];
    getcwd(path, 666);
    printf("%s >> ", path);
    fgets(input,sizeof(input),stdin);
    
    if (!strcmp(input,"\n")) {
      continue;
    }

    int PR = 0;
    if(strchr(input,'>') || strchr(input,'<') || strchr(input,'|'))
      PR = 1;

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

    if(!strcmp(commands[0],"cd")){
      cd(commands);
      continue;
    }
      
    int pid=fork();

    if (!pid){
      if(PR)
	PorR();
      else{
	execvp(commands[0],commands);
	printf("%s \n", strerror(errno));
      }

    }
    if (pid)
      waitpid(pid);

  }
  
  
}
