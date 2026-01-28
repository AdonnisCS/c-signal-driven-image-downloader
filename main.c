#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

volatile int i=0; // use this to determine next image

static void download(int sig);
static void shutdown_handler(int sig);

static void install_handlers(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = download;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);

    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) 
    {
        perror("sigaction(SIGUSR1)");
        exit(1);
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = shutdown_handler;

    //block SIGINT during handler
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);

    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) < 0) 
    {
        perror("sigaction(SIGINT)");
        exit(1);
    }
}

static void download(int sig) {
    (void)sig;

    if (i > 99) 
    {
        // Max reached
        printf("The maximum images have been reached\n");
        fflush(stdout);
        return;
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork");
        return;
    }

    if (pid == 0) 
    {
        //child process
        int n = (int)i; //current image index
        char cmd[256];
        snprintf(
            cmd, sizeof(cmd),
            "curl -o img/img%02d-%d.png http://turtle.cs.uchicago.edu:8000/img/%d",
            n, (int)getpid(), n
        );

        //curl
        (void)system(cmd);

        _exit(0);
    }

    i++;
}

static void shutdown_handler(int sig) {
    (void)sig;

    while (1) {
        pid_t w = waitpid(-1, NULL, 0);
        if (w > 0) {
            continue;
        }
        if (w == -1)
         {
            if (errno == ECHILD) 
            {
                break; //if we have no more children
            }
            // If interrupption
            if (errno == EINTR) 
            {
                continue;
            }
            //cases for random errors!
            perror("waitpid");
            break;
        }
    }

    exit(0);
}

int main(void) {
    install_handlers();

    printf("%d\n", (int)getpid());
    fflush(stdout);

    while (1) 
    {
        pause();
    }

    return 0;
}
