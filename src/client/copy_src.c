#include "common.h"

#define CMD "copy_file"

char *filepath;
char *srvip;
int  verbose;

struct file_info {
	FILE *fp;
	int sock;
	u_int file_size;
	u_int trans_size;
} typedef file_info;

static inline void usage (int status) {
	fprintf(stderr, "USAGE :%s is Error(%d) \n", CMD, status);
	fprintf(stderr, "	-h Server IP Address\n");
	fprintf(stderr, "	-f Copy File Path\n");
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

FILE *open_file() {
	FILE *fp;

	fp = fopen(filepath, "rb");
	if(!fp) {
		fprintf(stderr, "fopen() failed(%d)", errno);
		exit(-1);
	}

	return fp;
}


int get_service(int sock) {
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

	if (sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) < 0) {
		err = errno;
		fprintf(stderr, "socket() failed(%d). \n", err);
		exit(-1);
	}

	if (connect(sock, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr)) < 0) {
		err = errno;
		fprintf(stderr, "connetc() failed(%d). \n", err);
		exit(-1);
	}

	return sock;
}

int send_file(FILE *fp) {
	int ret;

	return 0;
}

int main (int argc, char **argv) {
	int sock;
	FILE *fp;

	/* オプション解析 */
	get_opt(argc, argv);

	/* コピー用ファイルの指定 */
	fp = open_file(fp);

	/* ソケット/コネクション確立 */
	sock = get_service(sock);

	return 0;
}
