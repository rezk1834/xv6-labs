#include "kernel/types.h"
#include "user.h"

int main() {
    int pipe1[2], pipe2[2];
    char message[1];
    int child_pid;

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        printf("Failed to create pipe\n");
        exit(1);
    }

    if ((child_pid = fork()) < 0) {
        printf("Failed to fork\n");
        exit(1);
    }

    if (child_pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        if (read(pipe1[0], message, 1) != 1) {
            printf("Child process: read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        if (write(pipe2[1], message, 1) != 1) {
            printf("Child process: write error\n");
            exit(1);
        }

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        message[0] = 'A';
        if (write(pipe1[1], message, 1) != 1) {
            printf("Parent process: write error\n");
            exit(1);
        }

        if (read(pipe2[0], message, 1) != 1) {
            printf("Parent process: read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());

        close(pipe1[1]);
        close(pipe2[0]);
        wait(0);
        exit(0);
    }
}

