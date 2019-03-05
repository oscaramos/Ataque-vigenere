#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

#include "cipher_tool/cipher.h"
#include "frequential_analysis/kasiski.h"
#include "frequential_analysis/keylength.h"

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
        std::cout << "No key given! If you don't know the key, you could try a"; 
        std::cout << "frequential analysis attack.\n";
        exit(2);
    }
}

// Converts the given number to the letter of the latin alphabet corresponding.
static char toLetter(unsigned index)
{
    char letters[26] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
        'r','s','t','u','v','w','x','y','z'
    };
    return letters[index];
}

// Prints the given vector of keys as a string.
static void printKey(std::vector<unsigned> keys) 
{
    std::cout << "The key is \"";
    for (const auto &key : keys) std::cout << toLetter(key);
    std::cout << "\"" << std::endl;
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

    int src = requireValidFileDescriptor(argv[2], O_RDWR);
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
			std::cout << "Attacking " << argv[2] << "...\n";
			std::vector<unsigned> keys = findKey(src, dest);
            printKey(keys);
			unsigned *k = &keys[0];
			uncipher(src, dest, k, keys.size());
            std::cout << argv[3] << " contains the unciphered text.\n";
    }
    close(src);
    close(dest);
    exit(0);
}
