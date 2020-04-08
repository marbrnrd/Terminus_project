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
}
