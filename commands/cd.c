#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
void cd_home(void)
{
    const char *const home = getenv("HOME");
    if (home)
        printf("chdir(%s) = %d\n", home, chdir(home));
}
 
int cd(int argc, char **argv)
{
    switch (argc) {
    case 1:
        cd_home();
        break;
    case 2:
        if (!strcmp(argv[1], "~"))
            cd_home();
        else
            printf("chdir(%s) = %d\n", argv[1], chdir(argv[1]));
        break;
    default:
        fprintf(stderr, "usage: cd <directory>\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
