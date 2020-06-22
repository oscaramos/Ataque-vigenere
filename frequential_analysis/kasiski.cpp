#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "keylength.h"

using namespace std;

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

static unsigned getKey(char c)
{
    for (unsigned i = 0; i < 4; ++i)
    {
        if (c == 'a')
            c = 'z';
        else
            c--;
    }
    return toNumber(c);
}

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
