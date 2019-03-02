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
#include <set>

static int requireValidFileDescriptor(const char *path, int flags)
{
    int fd = open(path, flags);
    if (fd == -1)
    {
        perror(strerror(errno));
        exit(1);
    }
    return fd;
}

static void printMap(std::map<char, unsigned> m, std::string name)
{
    std::cout << name << std::endl;
    for (auto &p : m)
    {
        std::cout << "<" << p.first << ", " << p.second << ">" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    int src = requireValidFileDescriptor(argv[1], O_RDWR);
    attack(src);
    close(src);
    exit(0);
}
