/* ヘッダー */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
<<<<<<< HEAD
#include <errno.h>
#include <getopt.h>
=======
#include <stddef.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
>>>>>>> d7de5e7dcd7360fd94f69e6ee9ad69efc4c7170d

/* 定数 */
#define MAX_FILE_SIZE   256
#define MIN_FILE SIZE   1
<<<<<<< HEAD
#define SERVICE_PORT	8080

/* Alias */
typedef unsigned long	u_long;
typedef unsigned int 	u_int;
typedef unsigned short 	u_short;
=======
#define SERVICE_PORT	9000
#define QUEUELIMIT 	5
#define DEBAG		0
>>>>>>> d7de5e7dcd7360fd94f69e6ee9ad69efc4c7170d

/* グローバル変数 */
static int serv;

/* プロトタイプ宣言 */
int get_socket(const char*);
void do_service(int);
static inline void do_concrete_service(int);
void echo_back(int);
static inline void usage(int);
double avg(double,double);
void print_indent(size_t i);

/* エイリアス */
typedef unsigned short	uint_16;
typedef unsigned int	uint_32;
typedef unsigned long	uint_64;

/* マクロ */
#define BUILD_BUG_ON(condition) ((void)BUILD_BUG_ON_ZERO(condition))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/* ファイルメタデータ */
struct metadata {
	uint_16	mode;
	uint_16 uid;
	uint_16 gid;
	uint_16 path;
	uint_32 file_size;
	uint_32 chunk_size;
} typedef metadata;
