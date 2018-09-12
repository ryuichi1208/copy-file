#include "common.h"

static inline void usage (int status){
	fputs ("\
		argument count mismatch error.\n\
		please input a service name or port number.\n\
		", stderr);
	exit (status);
}

int main (int argc, char **argv) {
	int sock;

	if (argc != 2) {
		usage(EXIT_FAILURE);
	}   

	if ((sock = get_socket(argv[1])) == -1){
		fprintf(stderr, "get_socket() failure.\n");
 		exit(EXIT_FAILURE);
	}

	do_service(sock);

	close(sock);

	return EXIT_SUCCESS;
}
