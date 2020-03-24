#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>

int main(int argc, char* argv[])
{
    char cwd[PATH_MAX];
    DIR *dir;
    struct dirent *ent;
    
    if( argc == 1)
    {
        getcwd(cwd, sizeof(cwd));
        if ((dir = opendir (cwd)) != NULL) {
        /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                printf ("%s\n", ent->d_name);
            }
        closedir (dir);
        } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
        } 
    } else if (argc == 2){
        getcwd(cwd, sizeof(argv[1]));
        if ((dir = opendir (cwd)) != NULL) {
        /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                printf ("%s", ent->d_name);
            }
        closedir (dir);
        } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
        }
    } else {
        printf("The data provided is not valid");
    }
}
