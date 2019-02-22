#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "cipher.h"
#define COUNT 1
#define ARGS_COUNT 5

static int requireValidFileDescriptor(const char *path, int flags) {
	int fd = open(path, flags);
	if (fd == -1) {
		perror(strerror(errno));
		exit(0);
	}
	return fd;
}

/* argv[1] is the path to the source file
 * argv[2] is the path to the destination file
 * argv[3] is the key
 */
int main(int argc, char *argv[]) 
{
	if (argc < ARGS_COUNT) {
		printf("usage: ./vigenere [cipher | uncipher] <source> <destination> <key>\n");
		exit(0);
	}
  unsigned keys[strlen(argv[4])];	
	int src = requireValidFileDescriptor(argv[2], O_RDONLY);
	int dest = open(argv[3], O_WRONLY | O_CREAT, 0666);
	
	keyToValues(argv[4], keys);

	if (strcmp("cipher", argv[1]) == 0) {
		cipher(src, dest, keys, strlen(argv[4]));
	} else {
		uncipher(src, dest, keys, strlen(argv[4]));
	}

	close(src);
	close(dest);
	exit(0);
}
