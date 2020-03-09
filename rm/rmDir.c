#include <stdio.h>
#include <unistd.h>

void main (int argc,char **argv)
{
   if(argc!=2){
   printf("Please, introduce the correct parameters\n");
}
else{
int exist;
   exist=rmdir(argv[1]);
if(exist==0){
printf("Directory deleted");
}
else{
printf("Error");
}
}
}
