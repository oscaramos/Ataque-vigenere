#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "cipher/cipher.h"
#define COUNT 1
#define MIN_ARGC 4
#define MAX_ARGC 5

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

static void requireAKey(int argc)
{
    if (argc < MAX_ARGC)
    {
        printf("No key given! If you don't know the key, you could try a frequential analysis attack.\n");
        exit(2);
    }
}

/* argv contains (in the following order):
 * - argv[1]: 
 *  - "cipher" to cipher the content of the source file with a key.
 *  - "unipher" to uncipher the content of the source file with a key.
 *  - "attack" to uncipher the content of the source file without a key.
 * - argv[2] contains the path to the file where to store the result.
 * - argv[3] contains the key when needed.
 */
int main(int argc, char *argv[])
{
    if (argc < MIN_ARGC)
    {
        printf("usage: ./vigenere <cipher | uncipher | attack> <source> <destination> [key]\n");
        exit(3);
    }

    int src = requireValidFileDescriptor(argv[2], O_RDONLY);
    int dest = open(argv[3], O_WRONLY | O_CREAT, 0666);

    if (strcmp("attack", argv[1]) != 0)
    {
        requireAKey(argc);
        unsigned keys[strlen(argv[4])];
        keyToValues(argv[4], keys);
        if (strcmp("cipher", argv[1]) == 0)
        {
            cipher(src, dest, keys, strlen(argv[4]));
        }
        else if (strcmp("uncipher", argv[1]) == 0)
        {
            uncipher(src, dest, keys, strlen(argv[4]));
        }
    }
    else
    {
        printf("Attacking...\n");
        caesarFrequentialAnalysisAttack(src, dest);
    }

    close(src);
    close(dest);
    exit(0);
}
