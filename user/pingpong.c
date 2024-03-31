#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, int* argv)
{
    if (argc != 1)
    {
        fprintf(2, "No need to pass arg to pingpong!\n");
        exit(1);
    }

    int p_a[2];
    int p_b[2];

    pipe(p_a);
    pipe(p_b);

    int buf[5];
    int pid = fork();
    if (pid > 0)
    {
        write(p_a[1], "ping", 5);
        wait(0);
        read(p_b[0], buf, 5);
        fprintf(1, "%d: received %s\n", getpid(), buf);
    }
    else
    {
        read(p_a[0], buf, 5);
        fprintf(1, "%d: received %s\n", getpid(), buf);
        write(p_b[1], "pong", 5);
    }

    exit(0);
}