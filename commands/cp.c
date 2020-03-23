#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc , char* argv[]){
   
   FILE *f1;
   FILE *f2;
   int c;
 
   if((f1 = fopen(argv[1],"r")) == NULL){
      perror("The following error ocurred");
      exit(1);
   }
   if((f2 = fopen(argv[2],"w")) == NULL){
       perror("The following error ocurred");
       exit(1); 
   }
   while((c = getc(f1)) != EOF){
      putc(c, f2);
   }
   fclose(f1);
   fclose(f2);
  
   return 0;
}
