/* ヘッダー */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

/* 定数 */
#define MAX_FILE_SIZE   256
#define MIN_FILE SIZE   1

/* グローバル変数 */


/* プロトタイプ宣言 */
int get_socket(const char*);
void do_service(int);
static inline void do_concrete_service(int);
void echo_back(int);
static inline void usage(int);
