
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <stdlib.h>

  
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
	   write(1,buffer,byteread);
	}
	if(byteread < 0){
           perror("cat:");
	   return -1;
	}
  }
  else
  {
     for(i=1; i<argc; i++){
        fd = open(argv[i], O_RDONLY);
        if(fd < 0){
	   perror("The following error ocurred");
           return -1;
        }
        buffer = (char*)malloc(pagesize);
 
        while((byteread = read(fd, buffer, pagesize)) > 0){
           write(1,buffer,byteread);
        }
        if(byteread < 0){
	    perror("cat:");
	   return -1;
        }
      close(fd);
      }
   }

  return 0;
}   


