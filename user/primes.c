#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

typedef struct _Pipe
{
    int out;
    int in;
} Pipe;

int main(int argc, char* argv)
{
    Pipe p;
    pipe((int*)&p);
    int pid = fork();
    if (pid > 0)
    {
        close(p.out);
        for(int i = 2; i <=35; i++)
        {
            write(p.in, &i, 1);
        }
        close(p.in);
        wait(0);
    }
    else
    {
        for(;;)
        {
            // p = get a number from left neighbor
            // print p
            // loop:
            //     n = get a number from left neighbor
            //     if (p does not divide n)
            //         send n to right neighbor
            close(p.in);
            int prime;
            if (read(p.out, &prime, 1) <= 0) break;

            fprintf(1, "prime %d\n", prime);

            Pipe p_new;
            pipe((int*)&p_new);
            int pid = fork();
            if(pid > 0)
            {
                int n;
                close(p_new.out);
                while(read(p.out, &n, 1))
                    if (n % prime !=0)
                        write(p_new.in, &n, 1);
                close(p_new.in);
                close(p.out);
                wait(0);
            }
            else
            {
                p.in = p_new.in;
                p.out = p_new.out;
            }
        }
    }
    exit(0);
}

