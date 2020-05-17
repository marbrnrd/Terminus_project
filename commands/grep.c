#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 1 - Function to check if there are execution errors, if the int returned by the function to check
       is negative the error is reported and execution of grep stops.

       Input:  int returned by the function to check correct completion of.
       Output: Error information defined by errno is printed and program exited if input value < 0.
**********************************************************************************************************/
void check(int err)
{
   if(err < 0)
   {
      perror("grep: the following error ocurred");
      exit(1);
   }
}

/* 2 - Function to print lines with the first ocurrence of a given pattern highlited in red. The line
       is traversed one step at a time, and the characters that form the first ocurrence of the pattern
       are ovewritten in red. Only lines that contain the pattern are printed.

       Input:  line to inspect (char []) and pattern to look for (char*).
       Output: line with the pattern highlighted is printed to standard output.
**********************************************************************************************************/
void print_colored(char line[], char* pattern)
{
   char* head = line;
   char* color_start = strstr(line, pattern);
   

   if(color_start != NULL){
      char* color_end = color_start + strlen(pattern);

      while(head != color_start)
      {
         check(write(1,head,sizeof(char)));
         head++;
      }
      system("echo -en \x1b[31m");

      while(head !=color_end)
      {
         check(write(1,head,sizeof(char)));
         head++;
      }
      system("echo -en \x1b[0m");

      while(*head != '\0')
      {
        check(write(1,head,sizeof(char)));
        head++;
      }
      check(write(1,"\n",1));
   }
}

/* 3 - Function to read the contents of a given file while printing the lines that contain
       a user inputed pattern to standard output with the pattern location highlighted. 
       While reading the file, lines are stored in an array, reading one character at a time.
       When a newline character is read, the line is inspected. The process reapeats until rea-
       ching the end of the file.  

       Input:   file descriptor of the file to read(int), and pattern to look for(char*).
       Output:  lines containing the pattern are are printed to standard output with the first 
                occurrence of the pattern highlighted.
**************************************************************************************************/
void match_pattern(int fd, char* pattern)
{
   int i=0,r;
   char buf;
   char line[4096];

   while((r = read(fd, &buf, sizeof(char))) > 0)
   {
      if(buf == '\n')
      {
         line[i]='\0';
         print_colored(line,pattern);
         memset(line,0,sizeof(line));
         i=0;
      }
      else
      {
         line[i++]=buf;
      }
   }
   check(r);
}

/* 4 - Main function. If no file is granted, input is read from standard input.
**********************************************************************************************/
int main(int argc, char* argv[]){

   int fd;
   struct stat stt;

   if(argc == 2)
   {      
         match_pattern(0,argv[1]);
   }
   else if(argc==3)
   {   
       check(stat(argv[2],&stt)); //check if the file exists

       check(fd = open(argv[2],O_RDONLY));

       match_pattern(fd,argv[1]);
   }
   return 0;
}
