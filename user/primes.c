#include "kernel/types.h"
#include "user.h"

void find_primes(int read_fd) __attribute__((noreturn));

void produce_numbers(int start, int end, int write_fd) {
    for (int i = start; i <= end; i++) {
        write(write_fd, &i, sizeof(i));
    }
    close(write_fd);
}

void find_primes(int read_fd) {
    int current_prime;
    if (read(read_fd, &current_prime, sizeof(current_prime)) == 0) {
        close(read_fd);
        exit(0);
    }
    printf("prime %d\n", current_prime);

    int new_pipe[2];
    pipe(new_pipe);

    if (fork() == 0) {
        close(new_pipe[1]);
        find_primes(new_pipe[0]);
    } else {
        close(new_pipe[0]);
        int number;
        while (read(read_fd, &number, sizeof(number)) > 0) {
            if (number % current_prime != 0) {
                write(new_pipe[1], &number, sizeof(number));
            }
        }
        close(read_fd);
        close(new_pipe[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int pipe_fds[2];
    pipe(pipe_fds);

    if (fork() == 0) {
        close(pipe_fds[1]);
        find_primes(pipe_fds[0]);
    } else {
        close(pipe_fds[0]);
        produce_numbers(2, 280, pipe_fds[1]);
        close(pipe_fds[1]);
        wait(0);
        exit(0);
    }
}

