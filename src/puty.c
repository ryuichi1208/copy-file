#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
/* for CentOS5.x */
/* #include <pty.h> */
/* for NetBSD1.6 */
#include <util.h>
pid_t
pty_fork(int *ptrfdm, char *slave_name,
    struct termios *slave_termios,
    struct winsize *slave_winsize)
{
    int fdm, fds;
    pid_t pid;
    char pts_name[20];
    if (-1 == openpty(&fdm, &fds, slave_name, slave_termios, slave_winsize)) {
        perror("openpty()"); exit(1);
    }
    if (-1 == (pid = fork())) {
        perror("fork()"); exit(1);
    } else if (0 == pid) {
        /* child */

        /* create new session, child becomes session leader,
         * new process group leader, has no control terminal yet.
         */
        if (-1 == setsid()) {
            perror("setsid()"); exit(1);
        }
        /* open pty slave device 1st, then child becomes
         * control process.
         */
        if (-1 == open(slave_name, O_RDWR)) {
            perror("open(slave_name)"); exit(1);
        }
        /* close unused pty master device */
        close(fdm);

        /* set control terminal */
        if (-1 == ioctl(fds, TIOCSCTTY, (char*)0)) {
            perror("ioctl(fds, TIOCSCTTY)"); exit(1);
        }

        if (NULL != slave_termios) {
            if (-1 == tcsetattr(fds, TCSANOW, slave_termios)) {
                 perror("tcsetattr(slave_termios)"); exit(1);
            }
        }
        if (NULL != slave_winsize) {
            if (-1 == ioctl(fds, TIOCSWINSZ, slave_winsize)) {
                 perror("ioctl(TIOCSWINSZ)"); exit(1);
            }
        }
        /* stdin/out/err fileno duplication */
        if (STDIN_FILENO != dup2(fds, STDIN_FILENO)) {
            perror("dup2(STDIN_FILENO)"); exit(1);
        }
        if (STDOUT_FILENO != dup2(fds, STDOUT_FILENO)) {
            perror("dup2(STDOUT_FILENO)"); exit(1);
        }
        if (STDERR_FILENO != dup2(fds, STDERR_FILENO)) {
            perror("dup2(STDERR_FILENO)"); exit(1);
        }
        close(fds);
        return 0;
    } else {
        /* parent */
        *ptrfdm = fdm;
        return pid;
    }
}

#define BUFSIZE 512
static volatile sig_atomic_t sigcaught;
static void sig_term(int signo) { sigcaught = 1; }
void loop(int ptym)
{
    pid_t child;
    int nread;
    char buf[BUFSIZE];
    struct sigaction sa;
    sa.sa_handler = sig_term;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
#ifdef SA_INTERRUPT
    sa.sa_flags |= SA_INTERRUPT;
#endif
    if (-1 == (child = fork())) {
        perror("fork() for loop"); exit(1);
    } else if (0 == child) {
        /* child copies stdin to ptym */
        for (;;) {
            if (-1 == (nread = read(STDIN_FILENO, buf, sizeof(buf)))) {
                perror("read(stdin)"); exit(1);
            } else if (0 == nread) {
                fprintf(stderr, "child(read stdin, write ptym) detect EOF from stdin.\n");
                break; /* EOF */
            }
            if (nread != write(ptym, buf, nread)) {
                perror("write(ptym)"); exit(1);
            }
        }
        fprintf(stderr, "send SIGTERM to parent...\n");
        kill(getppid(), SIGTERM);
        close(ptym);
        fprintf(stderr, "PID[%d] closed ptym, sleeping(10)...\n", getpid());
        sleep(10);
        fprintf(stderr, "PID[%d] awaken, terminates.\n", getpid());
        exit(0);
    }
    if (-1 == sigaction(SIGTERM, &sa, NULL)) {
        perror("sigaction(SIGTERM)"); exit(1);
    }
    for (;;) {
        errno = 0;
        if (0 >= (nread = read(ptym, buf, sizeof(buf)))) {
            /* error or signal or EOF */
            break;
        }
        if (nread != write(STDOUT_FILENO, buf, nread)) {
            perror("write(stdout)"); exit(1);
        }
    }
    if (0 == nread) {
        fprintf(stderr, "parent(read ptym, write stdin) detect EOF from ptym.\n");
    }
    if (errno) {
        perror("read(ptym)");
    }
    if (0 == sigcaught) {
        fprintf(stderr, "send SIGTERM to child(%d)...\n", child);
        /* error or EOF, tell child termination */
        kill(child, SIGTERM);
    } else {
        fprintf(stderr, "child sent SIGTERM, maybe terminated.\n");
    }
    close(ptym);
    fprintf(stderr, "PID[%d] closed ptym, sleeping(10)...\n", getpid());
    sleep(10);
    fprintf(stderr, "PID[%d] awaken, terminates.\n", getpid());
}

int main(int argc, char *argv[])
{
    int fdm;
    pid_t pid;
    char slave_name[20];
    struct termios orig_termios;
    struct winsize size;

    if (2 > argc) {
        fprintf(stderr, "usage: %s commands...\n", argv[0]);
        return 1;
    }

    if (-1 == tcgetattr(STDIN_FILENO, &orig_termios)) {
        perror("tcgetattr()"); exit(1);
    }
    if (-1 == ioctl(STDIN_FILENO, TIOCGWINSZ, &size)) {
        perror("ioctl(TIOCGWINSZ)"); exit(1);
    }
    pid = pty_fork(&fdm, slave_name, &orig_termios, &size);
    if (0 == pid) {
        if (-1 == execvp(argv[1], &argv[1])) {
            perror("execvp()"); exit(1);
        }
    }
    fprintf(stderr, "slave name = %s\n", slave_name);

    loop(fdm);

    fprintf(stderr, "done\n");

    return 0;
}
