#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc > 9)
        printf("ERROR: You added too many arguments\n");
    else if (argc < 9)
        printf("ERROR: You need to add more arguments\n");
    
    int i = 0;
    printf("%d\n", argv[1][2]);
}