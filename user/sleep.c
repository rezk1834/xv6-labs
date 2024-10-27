#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide exactly one argument: <seconds>\n");
        exit(1);
    }

    int seconds = atoi(argv[1]);
    if (seconds < 0) {
        printf("The number of seconds must be zero or greater\n");
        exit(1);
    }

    sleep(seconds);
    return 0;
}

