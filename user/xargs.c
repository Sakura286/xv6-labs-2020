#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[])
{
    int blank_index = argc;
    char* params[MAXARG];
    for (int i = 0; i < argc; i++)
    {
        params[i] = argv[i];
    }

    char buf[512], *p, *p_orig;
    p_orig = buf;
    p = buf;
    while(read(0, p, 1))
    {
        if (*p++ == '\n')
        {

            int pid = fork();
            if (pid == 0)
            {
                char param_str[512], *pp, *pp_orig;
                pp = param_str;
                pp_orig = param_str;

                strcpy(param_str, p_orig);

                while(*pp != '\n')
                {
                    if (*pp == ' ')
                    {
                        // string trunk with '\0', rather than ' '
                        *pp = '\0';
                        params[blank_index++] = pp_orig;
                        pp_orig = pp + 1;
                    }
                    pp++;
                }
                *pp = '\0';
                params[blank_index] = pp_orig;
                exec(params[1], &params[1]);
            }
            else
            {
                wait(0);
            }
            p_orig = p;
        }
    }
    exit(0);
}
