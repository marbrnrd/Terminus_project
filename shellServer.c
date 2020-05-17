#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cd/cd.h"

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXPIPE 20
#define MAXARGS 20

char* root;
char* originalpath;
char locations[256];
char home[256];
char cmdpath[256];  

struct sigaction old_action;

int cd(int argc,char **argv);

static int execute_piped(int cmdnum,char** cmds,char* rfiles[]);
static int execute(char* line);


/* 1 -  Function to check if there are execution errors, if the int returned by the function to check
        is negative the error is reported.

	Input:  int returned by the function to check correct completion of.
        Output: Error information defined by errno is printed if input value < 0.
************************************************************************************************************/
void check(int err){
   if(err < 0){
      perror("Error");
   }
}

/* 2 - Checks if a line's length not greater than the maximum size allowed(200). Characters are read one at
       a time until a newline character is found or the max size is reached. Errors can be caused due to rea-
       ding failure(read()) or too long lines.

       Input:   Array to store the read line on (char[]) and end of file pointer (int*).
       Output:  Read line ending on NULL, and value end of file pointer value set(0 - not eof, 1 - eof).
************************************************************************************************************/
int read_line(char line[], int* eofp)
{
   int ret,i;

   *eofp = 0;
   i=0;
   while ((ret=read(0,line+i,1)) == 1) {
      if (line[i]=='\n') break;  // correct line
      i++;
      if (i>=MAXLINE) {
         ret=-2;        // line too long
         break;
      }
   }

   switch (ret)
   {
     case 1 : line[i+1]='\0';    // correct reading
              break;
     case 0 : *eofp = 1;        // end of file
              return 0;
              break;
     case -1: fprintf(stderr,"Reading failure \n"); //reading failure
              return 0;
              break;
     case -2: fprintf(stderr,"Line too long -- removed command\n"); //line too long
              return 0;
              break;
   }

   return 1;
}

/* 3 - Function used to parse a given command(char*). The command is analized and split on arguments. It is
       checked that the argument count is not greater than the maximum allowed 20. The value of the arg count
       is stored in the location pointed by argcp(int*).
 
       Input:   command string(char*), pointer for the argument count(int*), array to store the arguments on 
                (char* []) and value representing the max valid argument count(int).
       Output:  arg count value is stored on *argcp and argument strings are stored in the args array.
                (note: cmpd is modified by strtok)
************************************************************************************************************/
int parse_command(char* cmdp,int* argcp, char* args[], int max){

   int i;

   // Analyzing the line
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

/* 4 - Function to parse a given line, storing in the process each detected command (separated by " | ") in an
       array. It is checked that the command count is not greater than the maximum allowed(20).

       Input:   line to parse(char[]) and pointer array to store the command strings on(char** pipedcmd).
       Output:  pipedcmd is filled. Found command cound is returned.
***************************************************************************************************************/
int parse_pipe(char line[], char** pipedcmd) 
{ 
    int i;
    char* pline = line;

    for (i = 0; i < MAXPIPE; i++) { 
        pipedcmd[i] = strsep(&pline, "|"); 
        if (pipedcmd[i] == NULL) 
            break; 
    } 
  
    return i;
} 

/* 5 - Function to parse the commands that may have redirections ('<','>'). The first and last of the piped co-
       mmands are analized (if not piped the only input command is analized). The command and input/output file
       are split and stored separate arrays.

       Input:   command number(int), string array containing the previously parsed commands and array to store 
                the input/output file names on.
       Output:  file array is filled(NULL value stored if no file) and commmand string is updated for the parsed
                commands.("pwd > file" --> cmd[i] = "pwd", files[1] = "file")   
*****************************************************************************************************************/
int parse_redirection(int cmdnum, char* cmd[],char* files[] )
{
    files[0] = NULL;
    files[1] = NULL; 

    //look for an output file
    char* out = malloc(strlen(cmd[cmdnum-1])); 

    strcpy(out,cmd[cmdnum-1]);
    cmd[cmdnum-1] = strsep(&out, ">"); 
    
    if(out != NULL) 
       out = strsep(&out, "\n");

    if(out != NULL)
       files[1] = strtok(out, " ");

    //look for an input file
    char* in = malloc(strlen(cmd[0]));

    strcpy(in,cmd[0]);
    cmd[0] = strsep(&in, "<");

    if(out != NULL) 
       in = strsep(&in, "\n");

    if(in != NULL)
       files[0] = strtok(in, " ");

    free(out);
    free(in);

    return 0;
}

/* 6 - Deletes the entire directory tree used in the game(to be called when exiting the game)
****************************************************************************************************************/
void removeDirectoryTree(){
   char rmcommand[256];
   strcpy(rmcommand,"rm -R ");
   strcat(rmcommand,home);
   system(rmcommand);
}

/* 7 - Handler function for the SIGINT signal. Restores the original values of: the assigned handler for SIGINT, 
       and the PATH environment variable, removes the directory tree used in the game and finally terminates the
       process.
****************************************************************************************************************/
void sigint_handler(int sig){
    printf("\nDeleting game world :(\n");
    sigaction(SIGINT, &old_action, NULL);
    setenv("PATH", originalpath, 1);
    removeDirectoryTree();
    kill(0, SIGINT);  
}

/* 8 - Function used to print the descriptions of the game locations when moving with "cd".

       Input:  file name of the location(char*).
       Output: Contents of file are printed to standard output.
****************************************************************************************************************/
void location_desc(char* location){
  char buf[256];
  char locpath[256];
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
  strcat(locpath,prev);

  char desc[256] = "cat ";
  execute(strcat(desc,locpath));
}

/* 9 - Function to check if a command corresponds to one of the "built in" commands. If so the command is executed
       by calling to the appropiate external function.

       Input:  pointer array storing user inputed commands(char**).
       Output: If the command is a built-it, it is executed. Returned value: 1.
               Else nothing is executed. Returned value: 0.  
*****************************************************************************************************************/
int builtin_handler(char** cmds)
{
   int argc;
   char buf[256];
   char* argv[MAXARGS]; 
   char* cmd = malloc(strlen(cmds[0]));
   
   strcpy(cmd,cmds[0]);
   if(parse_command(cmd, &argc, argv, MAXARGS) < 0){
      perror("Error: ");
   }

   if(strcmp(argv[0],"cd")==0){           //cd command case
      char* dir = getcwd(buf, sizeof(buf));
      char* first = "You are at the first room\n";
      if((strcmp(home, dir) == 0) && (argc > 1) && (strcmp(argv[1],"..") == 0)){
         write(1, first, strlen(first));
      } 
      else if(cd(argc, argv) == EXIT_SUCCESS){
         location_desc(argv[1]);
      }
      return 1;
   }           

   return 0;     
}

/* 10 -

********************************************************************************************************************/
int execute(char* line)
{
   int cmdnum;
   char* pipedcmd[MAXPIPE];
   char* rfiles[2]; 

   if((cmdnum =  parse_pipe(line, pipedcmd)) > 0){
      parse_redirection(cmdnum, pipedcmd,rfiles); 
      execute_piped(cmdnum,pipedcmd,rfiles); 
   }     
   check(cmdnum);

   return 0;
}

/* 10 - Executes the user inputed commands, with piping and redirection. Steps:
          -If input file is provided, it is opened.
          -It is checked if the input command is a builtin. If so execute and stop(return 0).
          -Piped commands are executed. First command stdin is set to regular stdin or redirected input file
           (if provided), rest of commands stdin is replaced by the reading end of the pipe connected to the
           previous command. All commands stdout are redirected to the writing end of the pipe connected
           to the next command(last command may redirect to output file if provided).
          -The main parent process waits for the last of children.

        Input:   command number(int), pointer array storing each of the commands, string array storing 
                 input/output files.
        Output:  Return 0 on clean execution.
******************************************************************************************************************/
int execute_piped(int cmdnum,char** cmds,char* rfiles[])
{
   int i,status;

   //save in/out
   int tmpin=dup(0);
   int tmpout=dup(1);

   //set the initial input
   int fdin;
   
   if (rfiles[0] != NULL) {
      check(fdin = open(rfiles[0],O_RDONLY));
   }
   else {  
      fdin=dup(tmpin);             // Use default input 
   }

   //check built in command case
   if(builtin_handler(cmds)){return 0;}

   int ret;
   int fdout;
   for(i=0;i<cmdnum; i++) {
       //redirect input
       dup2(fdin, 0);
       close(fdin);

       //setup output
       if (i+1 == cmdnum){         // Last simple command   
          if(rfiles[1] != NULL){
             printf("%s\n",rfiles[1]);
             check((fdout=open(rfiles[1],O_WRONLY | O_TRUNC)));
          }
          else {
             fdout=dup(tmpout);   // Use default output  
          } 
         
       }
       else {                     // Not last simple command create pipe  
          int fdpipe[2];
          check(pipe(fdpipe));
          fdout=fdpipe[1];
          fdin=fdpipe[0];
       }// if/else

       //Redirect output
       dup2(fdout,1);
       close(fdout);

       // Create child process
       check(ret=fork());
       if(ret==0) {
	  int argc; 
          char* argv[MAXARGS];

          if(parse_command(cmds[i], &argc, argv, MAXARGS) > 0){
	     execvp(*argv, argv);
	  }
          char msg[25] = "Error: Command not found\n";
          write(2,msg,sizeof(msg));
          exit(1);
       }
   } // for

   //restore in/out defaults
   dup2(tmpin,0);
   dup2(tmpout,1);
   close(tmpin);
   close(tmpout);

   // Wait for last command
   check(waitpid(ret, &status, 0));
 
   return 0;
}

/* 11 - Main function of the shell. Before starting the command reading loop some configurations are 
        done to set up the game. SIGINT signal handler is swapped by a custom one to delete the directory
        tree created for the game upon closing the program. ROOT, PATH and HOME environment variables are
        changed to simulate the game being run on its own mini computer(changing the PATH also helps with
        making sure the commands executed are the ones programmed by us instead of the bash commands).
        Finally the games introductory message is printed, the prompt is shown and the normal loop starts.
********************************************************************************************************/
int main_shell()
{
   char * Prompt = "myShell0> ";
   int cmdnum, eof= 0;
   char line[MAXLINE];
   char* pipedcmd[MAXPIPE];
   char* rfiles[2];
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
   execute("cat files/intro.txt");

   //Move to Home(Game starting location)
   chdir("Home");
    
   while (1) {
      write(1,Prompt, strlen(Prompt));

      if (read_line(line, &eof) > 0) {
       
        if((cmdnum =  parse_pipe(line, pipedcmd)) > 0){
           parse_redirection(cmdnum, pipedcmd,rfiles); 
	   execute_piped(cmdnum,pipedcmd,rfiles);
	}
      }
      if (eof) exit(0);
   }
}
