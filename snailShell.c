#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

// Takes a path directly as it is received from the input.
// changes directory to that path.
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

//Takes in the
void command(char* path, char* args [], fd in, fd out){
    if (in != STDIN_FILENO){
        dup2(in, STDIN_FILENO);
    }  
    
    if (out != STDOUT_FILENO){
        dup2(out, STDOUT_FILENO);
    } 
    
    int pid=fork();

    if (!pid){
	  execvp(path, args);
	  printf("%s \n", strerror(errno));
    }
    
    if (pid)
      waitpid(pid);

}

void main() {

  while(1){
    char input[256];
    char path[666];
    getcwd(path, 666);
    printf("%s >> ", path);
    fgets(input,sizeof(input),stdin);
    
    if (!strcmp(input,"\n")) {
      continue;
    }

    /*
    int PR = 0;
    if(strchr(input,'>') || strchr(input,'<') || strchr(input,'|'))
      PR = 1;
    */
    
    /*
    char * newLine=strchr(input, '\n');
    if (newLine)
      *newLine = NULL; */
    strtok(input,"\n");
  
//Searches for < or >
    fd source, dest;
    
    if (strchr(input, '<')){
        char * path=strsep(*input, '<');
        //Clear leading whitespace of files
        while(strcmp(path[0]," ") != 0)
            path++;
        //File came first --> swap source and file.
        char * temp = path;
        path = input;
        input = temp;
        
        source = open(source, O_CREAT, O_WRONLY);
    }
    else source = STDIN_FILENO;
    
    if (strchr(input, '>')){
        char * path2 = strsep(*input, "'>'");
        while(strcmp(path2[0]," ") != 0)
            path2++;
            
        dest = open(dest, O_CREAT, O_WRONLY);
        
    }
    else dest = STDOUT_FILENO;
    
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

    command(commands[0], commands, source, dest);
/*
    int pid=fork();

    if (!pid){
	  execvp(commands[0],commands);
	  printf("%s \n", strerror(errno));
    }
    
    if (pid)
      waitpid(pid);
*/

  }
  
  
}
