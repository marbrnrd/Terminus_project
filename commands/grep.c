#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>



void print_colored(char line[], char* pattern)
{
   char* head = line;
   char* color_start = strstr(line, pattern);
   

   if(color_start != NULL){
      char* color_end = color_start + strlen(pattern);

      while(head != color_start){
         write(0,head,sizeof(char));
         head++;
      }
      system("echo -en  \x1b[31m"); 
      while(head !=color_end){
         write(0,head,sizeof(char));
         head++;
      }
      system("echo -en \x1b[0m");
      while(*head != '\0'){
        write(0,head,sizeof(char));
        head++;
      }
      write(0,"\n",1);
   }
}


void match_pattern(char* argv[])
{
   int fd,i=0,r;
   char buf;
   char line[256];
 
   if((fd = open(argv[2],O_RDONLY)) < 0){
       perror("The following error happened: ");
       exit(1);
   }
   while((r = read(fd, &buf, sizeof(char))) != 0){
      if(buf == '\n'){
         line[i]='\0';
         print_colored(line,argv[1]);
         /*
         if(strstr(line, argv[1]) != NULL){
            write(0,line,sizeof(line));  
            write(0,"\n",1); 
         }
         */
         memset(line,0,sizeof(line));
         i=0;
      }
      else{
         line[i++]=buf;
      }
   }
}


int main(int argc, char* argv[]){

   struct stat stt;
   if(argc==3)
   {   
       if(stat(argv[2],&stt)==0) //check if the file exists
           match_pattern(argv);  
       else
       {
           perror("stat()");
           exit(1);
       }
   }
   return 0;
}
