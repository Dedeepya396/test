#include "neonate.h"
#include <dirent.h>
#include <ctype.h>
void neonate(char *input)
{
    int wait_time = -1;
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(input, " \t");
    int i = 0;
    while (token != NULL)
    {
        if (i == 2)
        {
            wait_time = atoi(token);
        }
        i++;
        token = strtok(NULL, " \t");
    }
    printf("%d\n", wait_time);
    executeNeonate(wait_time);
}
int is_key_pressed()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == EOF)
    {
        return 0;
    }

    return ch == 'x';
}

void executeNeonate(int wait_time)
{
    struct dirent *d;
    DIR *Di = opendir("/proc");
    int recent_pid = 0;
    if (Di == NULL)
    {
        printf("Error in opening proc directory\n");
        return;
    }
    while ((d = readdir(Di)) != NULL)
    {
        if (is_key_pressed())
            break;
        if (d->d_type == DT_DIR)
        {
            if (is_key_pressed())
                break;
            if (isdigit(d->d_name[0]))
            {
                int curr_pid = atoi(d->d_name);
                if (curr_pid > recent_pid)
                {
                    if (is_key_pressed())
                        break;
                    printf("%d\n", curr_pid);
                    recent_pid = curr_pid;
                    sleep(wait_time);
                    if (is_key_pressed())
                        break;
                }
            }
            if (is_key_pressed())
                break;
        }
        if (is_key_pressed())
            break;
    }
}