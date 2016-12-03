#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

// Takes a path directly as it is received from the input.
// changes directory to that path.

 /*  Inputs: char *commands[] */
 /* Returns: Nothing */

void cd(char *commands[]){
  
  char path[100];
  getcwd(path, 100);
        
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
      strcat(newpath, dirs[i]);
      strcat(newpath, "/");
      i++;
    }    
  }else{
    while (dirs[i]) {
      strcat(newpath, dirs[i]);
      strcat(newpath, "/");
      i++;
    }
  }  
  strcat(newpath, commands[1]);
      
  //changes directory
  if(chdir(newpath))
    printf("Error?: %d Cannot Change to this Directory\n", chdir(newpath));
  getcwd(path, 100);  
}

 /*  Inputs: char *args[] */
 /* 	     char *c */
 /* Returns: Index of c in args */

 /* 	     Finds and returns the index of char* c in args */
 /* 	     Returns -1 if not found */
 /* 	     	     (useless, needed an int to return if fail, */
 /* 		      but < > and | are all guaranteed to be in */
 /* 		      input before calling redirect_index()) */
int redirect_index(char* args[], char *c){
  int i = 1;
  while(args){
    if(strcmp(*args,c)) return i;
    i++;
    args++;
  }
  return -1;
}

/*  Inputs: char *path */
 /*   	     char *args[] */
 /* 	     int  *ind */
 /* Returns: Nothing */

 /* 	     Redirects inputs and outputs from stdin and stdout using dup() */
 /* 	     Execvps commands */
 /* 	     If arguments for input and output are invalid, prints error */
 /* 	     	message */
//Takes in the arguments to execvp
void redirect(char* path, char* args [], int* ind){
  
  // indicator
  // 100 == <
  // 010 == >
  // 001 == |

  int indicator = *ind;
  int stdin = dup(0);
  int stdout = dup(1);
  
  int in, out, pipe, prog2IND;
  int inIND = 0;
  int outIND = 0;
  int pipeIND = 0;

  char *prog2[10];
  
  if(indicator%10 && indicator/10){
    printf("Only redirects or one pipe, not both\n");
    return;
  }

  if(indicator==100){
    inIND = redirect_index(args,"<") + 1; //location of new stdin
    in = open(args[inIND], O_RDONLY);
    if(in == -1){
      printf("In File does not exist\n");
      return;
    }
    dup2(in,0);
  }
  
  if(indicator==10){
    outIND = redirect_index(args,">") + 1; //location of new stdout
    out = open(args[outIND], O_WRONLY | O_CREAT, 0644);
    if(out == -1){
      printf("Out File does not exist: %s\n",strerror(errno));
      return;
    }
    dup2(out,1);
  }

  if(inIND) args[inIND-1] = NULL;
  if(outIND) args[outIND-1] = NULL;
  
  if(indicator==1){
    pipeIND = redirect_index(args,"|"); //location of pipe
    prog1IND = 0; // first arg is always program
    prog2IND = pipeIND + 1; // arg after | is always second program
  
    int j=0;
    while(args[j+prog2IND]){
      prog2[j]= args[j+prog2IND];
      j++;
    }
    prog2[j]= NULL;
    pipe= open(args[prog1IND], O_WRONLY | O_CREAT, 0644);
    if (pipe== -1){
      printf("Piping failed\n");
      return;
    }

    dup2(pipe, 1);
    args[pipeIND] = NULL;
  }
  
  int pid=fork();
  if (!pid){
    execvp(args[0],args);
    exit(1);
  }
  if (pid){
    wait(&pid);
    if (indicator==1){
      close(pipe);
      pipe = open(args[prog1IND], O_RDONLY);
      dup2(pipe, 0);
      dup2(stdout,1);
    }
  }
  
  if (indicator==1){
    pid=fork();
    if (!pid){
      execvp(args[prog2IND],prog2);
      exit(1);
    }
    if (pid){
      wait(&pid);
      close(pipe);
    }
  }   

  dup2(stdin,0);
  close(in);
  dup2(stdout,1);
  close(out);
}

 /*  Inputs: Nothing */
 /* Returns: Nothing */

 /* 	     Handles everything else */
 /* 	     Parses lines of commands and splits them into command arrays */
 /* 	     Execvps non-redirected commands */
 /* 	     Handles exiting */
void main() {
  umask(0000);

  while(1){    
    char *path=malloc(sizeof(char*));
    char *input=malloc(sizeof(char*));
    getcwd(path, 666);
    printf("%s >> ", path);
    fgets(input,256,stdin);
    
    int red = 0;
    if(strchr(input,'<')) red = 100;
    if(strchr(input,'>')) red = 10;
    if(strchr(input,'|')) red = 1;

    if (!strcmp(input,"\n")) {
      continue;
    }

    if (strstr(input,"exit")) exit(1);

    strtok(input,"\n");

    char *s = input;
    char *mult[10];
    int multcom = 1;
    if(strstr(s,";")){
      int i = 0;
      while (s) {
	mult[i] = strsep(&s,";");
	i++;
	multcom++;
      }
      mult[i]=NULL;
    }
    else{
      mult[0] = input;
      mult[1] = NULL;
    }

    int multIND = 0;
    while(multcom){

      char *com = mult[multIND];
      char *commands[10];
  
      int i = 0;
      while (com) {
	commands[i] = strsep(&com," ");
	i++;
      }
      commands[i]=NULL;

      if(!strcmp(commands[0],"cd")){
	cd(commands);
	continue;
      }

      int pid=fork();

      if (!pid){
	if(red){
	  redirect(commands[0],commands,&red);
	}
	else{
	  execvp(commands[0],commands);
	  printf("%s \n", strerror(errno));
	}
	exit(1);
      }
    
      if (pid)
	wait(&pid);
      
      multcom--;
      multIND++;
    }
  }  
}
