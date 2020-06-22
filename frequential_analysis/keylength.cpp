#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <set>

#define MIN_COUNT 10
#define MAX_COUNT 10

using namespace std;

typedef map<string, unsigned> SubstringMap;
static void getDistance(const char *str, unsigned offset, SubstringMap &strs,
                 set<unsigned> &distances)
{
    string substring{str};
    auto result = strs.find(substring);
    if (result == strs.end())
    {
        strs.insert(pair<string, unsigned>(substring, offset));
    }
    else
    {
        unsigned last_offset = result->second;
        distances.insert(offset - last_offset);
    }
}

void findRepeatedSubstrings(int src, SubstringMap &strs, set<unsigned> &distances)
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
            getDistance(buffer, offset, strs, distances);
            lseek(src, offset + 1, SEEK_SET);
            count++;
        }
        offset++;
        count = MIN_COUNT;
    }
}

void countDistanceDivisors(unsigned distance, map<unsigned, unsigned> &divisors)
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
        countDistanceDivisors(distance, divisors);
    }
}

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
