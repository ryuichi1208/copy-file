#include "common.h"

#define CMD "copy_file"

char *filepath;
char *srvip;
static int verbose;
static int mode;

struct file_info {
	FILE *fp;
	int sock;
	off_t file_size;
	u_int trans_size;
} typedef file_info;

static inline void usage (int status) {
	fprintf(stderr, "USAGE :%s is Error(%d) \n", CMD, status);
	fprintf(stderr, "	-h Server IP Address\n");
	fprintf(stderr, "	-f Copy File Path\n");
	exit(status);
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

	servSockAddr.sin_port = htons(SERVICE_PORT);

	printf("port %d\n", servSockAddr.sin_port);

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		err = errno;
		fprintf(stderr, "socket() failed(%d). \n", err);
		exit(-1);
	}

	if (connect(sock, (struct sockaddr*) &servSockAddr, sizeof(servSockAddr)) < 0) {
		err = errno;
		fprintf(stderr, "connetc() failed(%d). \n", err);
		exit(-1);
	}

	return sock;
}

static void
get_file_info() {
	return;
}

int send_file(file_info *fin) {
	int ret;
	struct stat info;

	ret = lstat(filepath, &info);
	if (!ret) {
		fprintf(stderr, "fstat() failed(%d)\n", errno);
		usage(-1);
	} else {
		fin->file_size = info.st_size;
		if (fin->file_size)
	}
	

	return 0;
}

int main (int argc, char **argv) {
	int sock, ret;
	FILE *fp;

	/* ファイル情報構造体 */
	file_info *fin;
	memset(&fin[0], 0, sizeof(file_info));

	/* オプション解析 */
	get_opt(argc, argv);

	/* コピー用ファイルの指定 */
	fp = open_file();

	/* ソケット/コネクション確立 */
	sock = get_service(sock);

	fin->fp = fp;
	fin->sock = sock;	

	/* ファイル送信 */
	ret = send_file(&fin);
	if (!ret)
		fprintf(stderr, "send() failed\n");

	return 0;
}
