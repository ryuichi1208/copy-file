#include "common.h"

#define CMD "copy_file"

char *filepath;
char *srvip;
int  verbose;

static inline void usage (int status) {
	fprintf(stderr, "%s is Error(%d) \n", CMD, status);
	exit(status);
}

int get_opt(int argc, char **argv) {
	int i, opt, optflag;

	while ((opt = getopt(argc, argv, "h:f:v")) != -1) {
		switch (opt) {
		case 'h':
			srvip = optarg;
			optflag++;
			break;
		case 'f':
			filepath = optarg;
			optflag++;
			break;
		case 'v':
			fprintf(stdout, "verbose mode ON\n");
			break;
		default:
			usage(-1);
			break;
		}
	}

	if (optflag <= 0)
		usage(-1);
}

int get_service() {
	int err;
	struct sockaddr_in servSockAddr;
	u_short servPort;

	memset(&servSockAddr, 0, sizeof(servSockAddr));	
	servSockAddr.sin_family = AF_INET;

	if (inet_aton(srvip, &servSockAddr.sin_addr) == 0) {
		err = errno;
		fprintf(stderr, "Invalid IP Address(%d).\n", err);
		exit(-1);
	}
	
	servSockAddr.sin_port = SERVICE_PORT;
}

int main (int argc, char **argv) {
	int sock;

	get_opt(argc, argv);

	sock = get_service();
}
