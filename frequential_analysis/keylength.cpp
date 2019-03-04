#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <string>

// Greater the substrings are, the more chance to find the right length
#define MIN_COUNT 10 // Replace this value
#define MAX_COUNT 10 // Replace this value

using namespace std;

typedef map<string, unsigned> SubstringMap;

/* Saves the given string with its offset in the given map.
 *
 * str - is the string used as the key.
 * offset - is the offset where string was located in its file.
 * strs - is the map where to save the data.
 */
static void save(const char *str, unsigned offset, SubstringMap &strs,
                 set<unsigned> &offsets)
{
    string ngram{str};
    auto result = strs.find(ngram);
    if (result == strs.end())
    {
        strs.insert(pair<string, unsigned>(ngram, offset));
    }
    else
    {
        unsigned last_offset = result->second;
        offsets.insert(offset - last_offset);
    }
}

/* Finds all the repeated substrings in the file described by src of a size
 * between MIN_COUNT and MAX_COUNT.
 *
 * src - is the file descriptor of the file to search in.
 * strs - is a map on the substring with the substring occurences as value. 
 */
void findRepeatedSubstrings(int src, SubstringMap &strs, set<unsigned> &offsets)
{
    char buffer[MAX_COUNT];
    off_t end_offset = lseek(src, 0, SEEK_END);
    off_t offset = lseek(src, 0, SEEK_SET);
    unsigned count = MIN_COUNT;
    while (offset + count <= end_offset)
    {
        while (offset + count <= end_offset && count <= MAX_COUNT)
        {
            read(src, buffer, count);
            buffer[count] = '\0';
            save(buffer, offset, strs, offsets);
            lseek(src, offset + 1, SEEK_SET);
            count++;
        }
        offset++;
        count = MIN_COUNT;
    }
}

void findDistanceDivisors(unsigned distance, map<unsigned, unsigned> &divisors)
{
    for (int divisor = 2; divisor <= distance; divisor++)
    {
        if (distance % divisor == 0)
        {
            divisors[divisor]++;
        }  
    }
}

static void findDistancesDivisorsFrequencies(map<unsigned, unsigned> &divisors, 
                                           const set<unsigned> &distances)
{
    for (const auto &distance : distances)
    {
        findDistanceDivisors(distance, divisors);    
    }
}

/* Finds the divisor that can divide the most of the distances.
 *
 * divisors - is a map on the divisor with the divisor number of possible distance
 * division.
 */
static unsigned findMostFrequentDivisor(std::map<unsigned, unsigned> &divisors)
{
    unsigned mostFrequentDivisor{2};
    for (const auto &p : divisors)
    {
        if (divisors[mostFrequentDivisor] < p.second)
        {
            mostFrequentDivisor = p.first;
        }
    }
    return mostFrequentDivisor;
}

/* Finds the length of the key used to cipher a text with Vigènere cipher.
 *
 * distances - are the distances between all the substring.
 */
unsigned findKeyLength(int src)
{
    map<string, unsigned> substrings;
    map<unsigned, unsigned> divisors;
    set<unsigned> distances;
    findRepeatedSubstrings(src, substrings, distances);
    findDistancesDivisorsFrequencies(divisors, distances);
    unsigned key_length = findMostFrequentDivisor(divisors);
    return key_length;
}
