#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "cd/cd.h"

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20

char* root;
char* originalpath;
char locations[256];
char home[256];
char cmdpath[256];  

struct sigaction old_action;

int cd(int argc,char **argv);

static int execute(int argc, char *argv[]);

/////////// reading commands:

int read_args(int* argcp, char* args[], int max, int* eofp)
{
   static char cmd[MAXLINE];
   char* cmdp;
   int ret,i;

   *argcp = 0;
   *eofp = 0;
   i=0;
   while ((ret=read(0,cmd+i,1)) == 1) {
      if (cmd[i]=='\n') break;  // correct line
      i++;
      if (i>=MAXLINE) {
         ret=-2;        // line too long
         break;
      }
   }

   switch (ret)
   {
     case 1 : cmd[i+1]='\0';    // correct reading 
              break;
     case 0 : *eofp = 1;        // end of file
              return 0;
              break;
     case -1 : *argcp = -1;     // reading failure
              fprintf(stderr,"Reading failure \n");
              return 0;
              break;
     case -2 : *argcp = -1;     // line too long
              fprintf(stderr,"Line too long -- removed command\n");
              return 0;
              break;
   }
   // Analyzing the line
   cmdp = cmd;
   for (i=0; i<max; i++) {  /* to show every argument */
      if ((args[i]= strtok(cmdp, " \t\n")) == (char*)NULL) break;
      cmdp= NULL;
   }
   if (i >= max) {
      fprintf(stderr,"Too many arguments -- removed command\n");
      return 0;
   }
   *argcp= i;
   return 1;
}
//////////////////////////////////////
void removeDirectoryTree(){
   char rmcommand[256];
   strcpy(rmcommand,"rm -R ");
   strcat(rmcommand,home);
   system(rmcommand);
}

//////////////////////////////////////
void sigint_handler(int sig){
    printf("\nDeleting game world :(\n");
    sigaction(SIGINT, &old_action, NULL);
    setenv("PATH", originalpath, 1);
    removeDirectoryTree();
    kill(0, SIGINT);  
}
//////////////////////////////////////
void location_desc(char* location){
  char buf[4096];
  char locpath[4096];
  char* prev;

  char* dir = getcwd(buf, sizeof(buf));
  char* curr = strtok(dir,"/");
  while(curr != (char*)NULL){
     prev = curr;
     curr = strtok(NULL,"/");
  }
  
  //form the " .../Terminus_project/files/locations/location.txt" string and execute cat
  strcat(prev,".txt");  
  strcpy(locpath,locations);

  char* argv[MAXARGS] = {};
  argv[0] = "cat";
  argv[1] = strcat(locpath, prev);  
  execute(2,argv);

}
//////////////////////////////////////
int execute(int argc, char *argv[])
{
  pid_t pid;
  int status;
  char buf[256]; 

  if(strcmp(argv[0],"cd")==0 || strcmp(argv[1],"~") == 0 ){           //cd command case
     char* dir = getcwd(buf, sizeof(buf));
     char* first = "You are at the first room\n";
  
     if((strcmp(home, dir) == 0) && (argc > 1) && (strcmp(argv[1],"..") == 0)){
        write(0, first, strlen(first));
     }
     else if(cd(argc, argv) == EXIT_SUCCESS){
        location_desc(argv[1]); 
     }
     else{
	     char* error = "No such directory\n";
	     write(0,error,strlen(error));
     }
  }
  else if((pid = fork()) < 0){                /*try to fork a child process*/
     printf("Error: fork failed\n");
     exit(1);
  }
  else if(pid == 0){                          /*Child process*/	
     //exec the command
     if(execvp(*argv, argv) < 0){
        fprintf(stderr, "The following error ocurred: command %s not found\n", argv[0]);
        exit(1);
     }
  }
  else{                                       /*parent process*/  
    if((pid = wait(&status)) < 0){
	fprintf(stderr, "Error: wait failed\n");
    }
  }
  
  return 0;
}
//////////////////////////////////////////////
int main_shell ()
{
   char * Prompt = "myShell0> ";
   int eof= 0;
   int argc;
   char *args[MAXARGS];
   char buf[256];
   struct sigaction action;
   
   originalpath = getenv("PATH");

   //set SIGINT signal handler to our handler(so it deletes the directory tree upon exiting the game)
   memset(&action, 0, sizeof(action));
   action.sa_handler = &sigint_handler;
   sigaction(SIGINT, &action, &old_action);

   //Create directory tree used in the game
   system("bash creation_script.sh");
   
   //set current directory to the root of the game directory tree
   root = getcwd(buf,sizeof(buf));  
   setenv("ROOT",root,1);  

   //set up PATH environment variable 
   strcpy(cmdpath,root);
   strcat(cmdpath,"/commands"); 
   setenv("PATH", cmdpath, 1);  

   //set up HOME environment variable
   strcpy(home,root); 
   strcat(home,"/Home");
   setenv("HOME", home, 1);  

   //store path to the dir. with the location texts   
   strcpy(locations,root); 
   strcat(locations,"/files/locations/");

   //display intro message
   char* intro[MAXARGS]= {};   
   intro[0] = "cat";
   intro[1] = "files/intro.txt";
   execute(2, intro);
  
   //Move to Home(Game starting location)
   chdir("Home");
    
   while (1) {
      write(0,Prompt, strlen(Prompt));

      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         execute(argc, args);
      }
      if (eof) exit(0);
   }
}
