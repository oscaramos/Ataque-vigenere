#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <string>

#include "keylength.h"

using namespace std;

/* Counts the frequency of all characters at n * offset. Where n is the number
 * of characters corresponding to the given offset.
 *
 * src - is the file descriptor of the source file.
 * frequencies - is the map holding the characters frequencies.
 * offset - is the offset of each charaters.
 * key_length - is the length of the key used to cipher src.
 */
void countCharFrequencies(int src, map<char, unsigned> &frequencies,
                          unsigned offset, unsigned key_length)
{
    char currentCharacter[1];
    unsigned current_offset = lseek(src, offset, SEEK_SET);
    unsigned treated = 0;
    while (read(src, currentCharacter, 1) != 0)
    {
        if (isalpha(*currentCharacter))
        {
            if (treated % key_length == 0)
            {
                frequencies[*currentCharacter]++;
            }
            treated++;
        }
        current_offset++;
    }
}

static char findMostFrequentChar(std::map<char, unsigned> &m)
{
    unsigned max_key{'a'};
    for (const auto &p : m)
    {
        if (m[max_key] < p.second)
        {
            max_key = p.first;
        }
    }
    return max_key;
}

static unsigned toNumber(char c)
{
    return toupper(c) - 'A';
}

static void char_uncipher(char *c, long key)
{
    for (unsigned i = 0; i < key; ++i)
    {
        switch (*c)
        {
        case 'A':
            *c = 'Z';
            break;
        case 'a':
            *c = 'z';
            break;
        default:
            (*c)--;
        }
    }
}

/* Unciphers a subpart. A subpart is a group of characters that have the same
 * offset.
 *
 * src - is the file descriptor of the file to uncipher.
 * key_length - is the length of the key.
 * key - is the key to uncipher with.
 */
void uncipher_subpart(int src, unsigned offset, unsigned key_length,
                      unsigned key)
{
    char currentCharacter[1];
    unsigned current_offset = lseek(src, offset, SEEK_SET);
    unsigned treated = 0;
    while (read(src, currentCharacter, 1) != 0)
    {
        if (isalpha(*currentCharacter))
        {
            if (treated % key_length == 0)
            {
                char_uncipher(currentCharacter, key);
                lseek(src, -1, SEEK_CUR);
                write(src, currentCharacter, 1);
            }
            treated++;
        }
        current_offset++;
    }
}

/* Copies src into a second file keeping alphabetic characters only and
 * converting them to lower case.
 */
static int getLowerCaseCharFile(int src)
{
    int tmp = open("tmp", O_RDWR | O_CREAT, 0666);
    char buffer[1];
    lseek(tmp, 0, SEEK_SET);
    while (read(src, buffer, 1) != 0)
    {   
        if (isalpha(*buffer))
        {
            *buffer = tolower(*buffer);
            write(tmp, buffer, 1); 
        }
    }   
    return tmp;
}

/* Attacks a text ciphered with Vigenère.
 *
 * src - is the file descriptor of the file to attack.
 * dest - is the file descriptor of the destination file.
 * key_size - is the size of the key used to cipher the file pointed by src.
 */
void attack(int src)
{
    int tmp = getLowerCaseCharFile(src);
    unsigned key_length = findKeyLength(tmp);
    for (unsigned offset = 0; offset < key_length; ++offset)
    {
        map<char, unsigned> frequencies;
        countCharFrequencies(tmp, frequencies, offset, key_length);
        
        std::cout << "offset " << offset << std::endl;
        for (auto &f: frequencies)
            std::cout << "<" << f.first << ", " << f.second << ">" << std::endl;
        
        char mostFrequent = findMostFrequentChar(frequencies);
        unsigned key = getKey(mostFrequent);
        std::cout << "Offset: " << offset << " Most frequend: " << mostFrequent << " Key: " << key;
        std::cout << " Key0: " << key0 << std::endl;
    }
    unlink("tmp");
}
