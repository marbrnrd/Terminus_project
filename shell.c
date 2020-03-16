#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20

#define LS 5863588
#define CD 5863276
#define MKDIR 210720772860
#define PWD 193502992
#define CAT 193488125     
#define RM 5863780
#define PWD 193502992


char* root;
char locations[256];
char home[256];

int cat(char* file);
int pwd();
int rm (int argc,char **argv);

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
   cmdp= cmd;
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

///////////////////////////////////////
unsigned long hashcode(const char *str){
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

//////////////////////////////////////
void location_desc(){
  char buf[256];
  char locpath[256];
  char* prev;
  char* cwd = getcwd(buf,sizeof(buf));
 
  //retrieve the last element of the current directory path 
  char* token = strtok(cwd, "/");          
  while(token != NULL){
    prev = token; 
    token =  strtok(NULL, "/");
  }
          
  //form the " .../Terminus_project/files/locations/location.txt" string and execute cat
  strcat(prev,".txt");
  strcpy(locpath,locations);

  char* argv[MAXARGS] = {};
  argv[0]="cat";
  argv[1]=strcat(locpath,prev);  
  execute(2,argv);

}
//////////////////////////////////////
int execute(int argc, char *argv[])
{
  pid_t pid;
  int status;

  
  if(strcmp(argv[0],"cd")==0){           //cd command case 
    if(argc < 2){
      chdir(home);
      location_desc(); 
    }
    else if(chdir(argv[1]) != 0){  
      fprintf(stderr, "No location with that name, try again\n");
    }
    else{
      location_desc();
    }
  }
  else if((pid = fork()) < 0){                  /*try to fork a child process*/
     printf("Error: fork failed\n");
     exit(1);
  }
  else if(pid == 0){                            /*Child process*/	
     
     char cmdpath[256];
     strcpy(cmdpath,root);
          
     //determine path of the command executable
     switch(hashcode(argv[0]))
     {
       case CAT:   
         strcat(cmdpath,"/commands/cat");
         break;
	
       case LS:    
         if(execvp(argv[0], argv) < 0){
           perror("The following error ocurred:");
           exit(1);
         }
         break;

       case RM:                 
         strcat(cmdpath,"/commands/rm");
         break;

       case PWD:
         strcat(cmdpath,"/commands/pwd");
         break;
     } 
     
     //exec the command
     if(execv(cmdpath, argv) < 0){
       perror("The following error ocurred:");
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
int main ()
{
   char * Prompt = "myShell0> ";
   int eof= 0;
   int argc;
   char *args[MAXARGS];
   char buf[256];

   //Create directory tree used in the game
   system("bash creation_script.sh");
   
   //set current directory to the root of the game directory tree
   system("cat files/intro.txt");
   root = getcwd(buf,sizeof(buf));  
  
   //store path to home dir. 
   strcpy(home,root); 
   strcat(home,"/Home");
 
   //store path to the dir. with the location texts   
   strcpy(locations,root); 
   strcat(locations,"/files/locations/");

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
