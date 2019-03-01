#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "kasiski.h"
#define COUNT 1
#define MIN_ARGC 4
#define MAX_ARGC 5

#include <map>

static int requireValidFileDescriptor(const char *path, int flags) {
	int fd = open(path, flags);
	if (fd == -1) {
		perror(strerror(errno));
		exit(1);
	}
	return fd;
}

static int getTmpFile(int src) {
	int tmp = open("tmp", O_RDWR | O_CREAT, 0666);	
	char buffer[1];
	printf("Start copying.\n");
	lseek(tmp, 0, SEEK_SET);
	while (read(src, buffer, 1) != 0) {
		if (isalpha(*buffer)) {
			*buffer = tolower(*buffer);
			write(tmp, buffer, 1);
		}
	}
	printf("Copied!\n");
	return tmp;
}

int main(int argc, char *argv[]) 
{
	int src = requireValidFileDescriptor(argv[1], O_RDONLY);
  std::map<std::string, std::vector<unsigned>> strs;
	int tmp = getTmpFile(src);
	findRepeatedSubstrings(tmp, strs);
  for (const auto &p : strs) {
  	std::cout << "chaine: [" << p.first << "] occurences: " << p.second.size();
  	std::cout << std::endl;
  }
	unlink("tmp");
	close(src);
	exit(0);
}
