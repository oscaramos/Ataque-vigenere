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

static int requireValidFileDescriptor(const char* path, int flags) {
    int fd = open(path, flags);
    if (fd == -1) {
        perror(strerror(errno));
        exit(1);
    }
    return fd;
}

static char toLetter(unsigned index) {
    char letters[26] = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
            'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    return letters[index];
}

static void printKey(std::vector<unsigned> keys) {
    std::cout << "La llave es \"";
    for (const auto& key : keys)
        std::cout << toLetter(key);
    std::cout << "\"" << std::endl;
}

/* Linea de compilacion
 * g++ Main.cpp frequential_analysis/kasiski.cpp frequential_analysis/keylength.cpp cipher_tool/cipher.c
-o vigenere
 */

/*
 * Ejemplos
 * Cipher
 * vigenere.exe cipher original.txt ciphed.txt MY_KEY
 *
 * Uncipher
 * vigenere.exe uncipher ciphed.txt unciphered.txt MY_KEY
 *
 * Attack
 * vigenere.exe attack ciphed.txt unciphered_by_attack.txt
 */

int main(int argc, const char* argv[]) {
    if (argc < MIN_ARGC) {
        std::cout << "uso: ./vigenere <cipher | uncipher | attack> <source> <destination> [key]\n";
        exit(3);
    }

    const char* action_type = argv[1];
    const char* source_path = argv[2];
    const char* destination_path = argv[3];

    int src = requireValidFileDescriptor(source_path, O_RDWR);
    int dest = open(destination_path, O_WRONLY|O_CREAT, 0666);

    if (strcmp("attack", action_type) == 0) {
        std::cout << "Atacando " << source_path << "...\n";
        std::vector<unsigned> keys = findKey(src, dest);
        printKey(keys);
        uncipher(src, dest, &keys[0], keys.size());
        std::cout << destination_path << " contiene el texto descifrado por ataque.\n";
    } else if (strcmp("cipher", action_type) == 0 || strcmp("uncipher", action_type) == 0) {
        const char* key = argv[4];
        const int key_len = strlen(key);

        unsigned keys[key_len];
        keyToValues(key, keys);
        if (strcmp("cipher", action_type) == 0) {
            cipher(src, dest, keys, key_len);
            std::cout << "Cifrando texto de " << source_path << " en " << destination_path << ".\n";
        } else if (strcmp("uncipher", action_type) == 0) {
            uncipher(src, dest, keys, key_len);
            std::cout << "Descifrando texto de " << source_path << " en " << destination_path << ".\n";
        }
    } else {
        std::cout << "Accion no conocida\n";
        exit(1);
    }
    close(src);
    close(dest);
    exit(0);
}
