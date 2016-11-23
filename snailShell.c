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
    
    *(strchr(input, '\n')) = NULL; //strtok(input,"\n");
  

    char *s = input;
    char *commands[10];
  
    int i = 0;
    while (s) {
      commands[i] = strsep(&s," ");
      i++;
    }

    if(!strcmp(commands[0],"exit")){
      exit = 1;
      break;
    }

     if(!strcmp(commands[0],"cd")){
      //gets your current path
      char path[100];
      getcwd(path, 100);
      printf("This is your current path: %s\n", path);
        
      char *tempPath= path;
      char *dirs[100];//contains directories to the path
      i = 0;
      strsep(&tempPath,"/");//gets rid of first "" element
        
      //parses through the path
      while (tempPath) {
	dirs[i] = strsep(&tempPath,"/");
	i++;
      }
      dirs[i]=0;
        
      //prints out the directories to path
      int i=0;
      /*while(dirs[i]){
	printf("directory: %s\n", dirs[i]);
	i++;
	}*/
      
      /*i=0;
      while(dirs[i]){
	printf("directory: %s\n", dirs[i]);
	i++;
	}
      */
        
      //creates new path according to input
      char newpath[100];
      strcpy(newpath, "/");
      
      /*i=0;
      while(dirs[i]){
	printf("directory: %s\n", dirs[i]);
	i++;
	}*/
      
      //printf("directory: %s\n", dirs[2]);
      
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
      //printf("the new path: %s\n", newpath);
        
      //changes directory
      printf("Error?: %d\n", chdir(newpath));
      getcwd(path, 100);
      printf("This is your new path: %s\n", path);
       
      /*printf("Error?: %d\n", chdir("/home/cg"));
        getcwd(path, 100);
        printf("This is your current path (cg): %s\n", path);
        printf("Error?: %d\n", chdir("/home/cg/root"));
        getcwd(path, 100);
        printf("This is your current path (root): %s\n", path);
      */
    }
    


    //execvp(commands[0],commands);
  }

  return 0;

  
}
