#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void sig_hup(int signo)
{
    fprintf(stderr, "detect SIGHUP(%d)\n", signo);
    fflush(stderr);
}

int main(int argc, char *argv[])
{
    char buf[200];
    struct sigaction sa;
    int fderr;

    close(STDERR_FILENO);
    if (-1 == (fderr = open("/tmp/mycat_detectHUP", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU))) {
        perror("open(log)"); exit(1);
    }

    sa.sa_flags = 0;
#ifdef SA_INTERRUPT
    sa.sa_flags |= SA_INTERRUPT;
#endif
    sigaction(SIGHUP, &sa, NULL);

    while (NULL != fgets(buf, sizeof(buf), stdin)) {
        fprintf(stdout, "PID[%d] : %s\n", getpid(), buf);
        fflush(stdout);
    }
    if (feof(stdin)) {
        fprintf(stderr, "stdin detect EOF\n");
    } else {
        perror("fgets(stdin)");
    }
    return 0;
}
