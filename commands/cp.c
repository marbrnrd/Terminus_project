#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>


/* 2 - Main function. If no input or output file if grented, or if one of the doesnt exit an error is re-
       ported. Copies contents of input file to output file one character at a time..
**********************************************************************************************************/
int main(int argc , char* argv[]){
   
   FILE *f1;
   FILE *f2;
   int c;
   char err[80] = "cp: the following error ocurred: ";
 
   //check if arguments are correct
   if(argc < 3){
      printf("cp: missing operands, correct format: cp inputfile outputfile\n");
      exit(1);
   }

   if(argc > 3){
      printf("cp: too many operands, correct format: cp inputfile outputfile\n");
      exit(1);
   }
  
   //try to open the given files
   if((f1 = fopen(argv[1],"r")) == NULL){
      strcat(err,argv[1]);
      perror(err);
      exit(1);
   }
   if((f2 = fopen(argv[2],"w")) == NULL){
       strcat(err,argv[2]);
       perror(err);
       exit(1); 
   }

   //copy contents of f1 to f2
   while((c = getc(f1)) != EOF){
      putc(c, f2);
   }

   fclose(f1);
   fclose(f2);
  
   return 0;
}
