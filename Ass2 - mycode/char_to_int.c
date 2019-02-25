#include <stdio.h>

char
char_to_int(int c) {
    char n;
    n = c+97;
    return n;
}

int
main(int argc, char* argv[]) {
    char  n;
    int c;
    printf("Please enter exactly one char: \n");
    scanf("%d",&c);
    n = char_to_int(c);
    printf("The integer is %c. \n", n);
    return 0;
}