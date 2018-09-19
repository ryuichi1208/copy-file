#include "common.h"

int main(int argc, char* argv[]) {

	int servSock;
	int clitSock;
	struct sockaddr_in servSockAddr;
	struct sockaddr_in clitSockAddr;
	unsigned short servPort;
	unsigned int clitLen;

	if ( argc != 1) {
		fprintf(stderr, "argument count mismatch error.\n");
		exit(EXIT_FAILURE);
	}

	if (DEBAG) {
		if ((servPort = (unsigned short) atoi(argv[1])) == 0) {
			fprintf(stderr, "invalid port number.\n");
			exit(EXIT_FAILURE);
		}
	}

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	memset(&servSockAddr, 0, sizeof(servSockAddr));
	servSockAddr.sin_family      = AF_INET;
	servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servSockAddr.sin_port        = htons(SERVICE_PORT);

	if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr) ) < 0 ) {
		perror("bind() failed.");
		exit(EXIT_FAILURE);
	}

	if (listen(servSock, QUEUELIMIT) < 0) {
		perror("listen() failed.");
		exit(EXIT_FAILURE);
	}

	printf("service start %d\n", servSockAddr.sin_port);

	while(1) {
		clitLen = sizeof(clitSockAddr);
		if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0) {
			perror("accept() failed.");
			exit(EXIT_FAILURE);
		}

	printf("connected from %s.\n", inet_ntoa(clitSockAddr.sin_addr));
	close(clitSock);
	}

	return EXIT_SUCCESS;
}
