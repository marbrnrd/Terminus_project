
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <stdlib.h>

/* 1 - Function to check if there are execution errors, if the int returned by the function to check
       is negative the error is reported and execution of cat stops.

       Input:  int returned by the function to check correct completion of.
       Output: Error information defined by errno is printed and program exited if input value < 0.
**********************************************************************************************************/
int check(int err)
{
   if(err < 0){
      perror("cat: the following error ocurred");
      exit(1);
   }
}

/* 2 - Main function. If no file is granted(argc == 1), input is read from standard input.
**********************************************************************************************************/
int main(int argc , char *argv[])
{    
  ssize_t byteread;
  char* buffer;
  fflush(stdout);
  int pagesize = getpagesize();
  int fd;
  int i;
  
  if(argc == 1)
  {
	while((byteread = read(0, buffer, pagesize)) > 0){
	   check(write(1,buffer,byteread));
	}
        check(byteread);
  }
  else
  {
     for(i=1; i<argc; i++){
        fd = open(argv[i], O_RDONLY);
        check(fd);

        buffer = (char*)malloc(pagesize);
 
        while((byteread = read(fd, buffer, pagesize)) > 0){
           check(write(1,buffer,byteread));
        }
 	check(byteread);        

      close(fd);
      }
   }

  return 0;
}   


