#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

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

//Takes in the arguments to execvp as well as file descriptors for the input and output. 
void command(char* path, char* args [], int in, int out){
  //printf("still alive?\n");

  /*
  //  int std_in =dup(STDIN_FILENO);
  //int std_out =dup(STDOUT_FILENO);
  if (in != STDIN_FILENO){
    dup2(in, STDIN_FILENO);
  }  
    
  if (out != STDOUT_FILENO){
    dup2(out, STDOUT_FILENO);
  }
  */
  
  //printf("how bout now?\n");

  int pid=fork();

  if (!pid){
    if (in != STDIN_FILENO){
      dup2(in, STDIN_FILENO);
      close(in);
    }  
    
    if (out != STDOUT_FILENO){
      dup2(out, STDOUT_FILENO);
      close(out);
    }
    
    execvp(path, args);
    printf("%s \n", strerror(errno));
    exit(1);
  }
    
  if (pid){
    int s;
    wait(&s);
  }

}

void main() {
  umask(0000);

  while(1){
    //char input[256];
    //char path[666];
    
    char *path=malloc(sizeof(char*));
    char *input=malloc(sizeof(char*));
    getcwd(path, 666);
    printf("%s >> ", path);
    fgets(input,256,stdin);
    //printf("%s\n", input);
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
  
//Searches for <
    int source, dest;
    
    if (strchr(input, '<')){
        char * path=strsep(&input, "'<'");
        //Clear leading whitespace of files
        while(path[0] != ' ')
	  path++;

	source=open(path, O_RDONLY);
    }
    else source = STDIN_FILENO;
    
    if (strchr(input, '>')){
      //printf("%s\n",input);
      char * path2 = strsep(&input, "'>'");

      printf("ouput file: %s\n", path2);
      printf("???: %s\n", input);
      //File came first --> swap source and file.

      char *temp=path2;
      path2=input;
      input=temp;
      
      printf("ouput file: %s\n", path2);
      printf("???: %s\n", input);

      while(path2[0] == ' ')
            path2++;
            
      dest = open(path2, O_CREAT, 0644);
      close(dest);
      open(path2, O_WRONLY);
      //printf("sadface \n");
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
      exit(1);
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
