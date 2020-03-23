#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc , char* argv[]){
   
   int fd1, fd2, r;
   struct stat stt;   
   ssize_t psize = getpagesize();
   char buf[psize];


   if((fd1 = open(argv[1], O_RDONLY)) < 0){
	perror("The following error ocurred:");
   }
   if(stat(argv[2],&stt)==0){
       if((fd2 = open(argv[2], O_WRONLY)) < 0){
          while((r = read(fd1,buf,psize)) > 0){
             write(fd2,buf,psize);
          }
       }
   }
   else{
      if((fd2 = open(argv[2], O_CREAT|O_WRONLY))<0){
          while((r = read(fd1,buf,psize)) > 0){
             write(fd2,buf,psize); 
          }
      }
   }

   return 0;
}
