#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
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

char toLetter(unsigned index) {
	char letters[26] = {
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s',
		't','u','v','w','x','y','z'
	};
	return letters[index];
}

/* Gets the key used to cipher the given key. The result is based on the 
 * assumption that e is the most frequent letter in english.
 */
static unsigned getKey(char c) {
	for (unsigned i = 0; i < 4; ++i) {
		if (c == 'a') {
			c = 'z';
		} else {
			c--;
		}
	}
	return toNumber(c);
}

/* Attacks a text ciphered with Vigenère.
 *
 * src - is the file descriptor of the file to attack.
 * dest - is the file descriptor of the destination file.
 * key_size - is the size of the key used to cipher the file pointed by src.
 */
std::vector<unsigned> findKey(int src, int dest)
{
    int tmp = getLowerCaseCharFile(src);
		std::vector<unsigned> keys;
    unsigned key_length = findKeyLength(tmp);
    for (unsigned offset = 0; offset < key_length; ++offset)
    {
        map<char, unsigned> frequencies;
				countCharFrequencies(tmp, frequencies, offset, key_length);
        char mostFrequent = findMostFrequentChar(frequencies);
        unsigned key = getKey(mostFrequent);	
				keys.push_back(key);
    }
    unlink("tmp");
		return keys; 
}
