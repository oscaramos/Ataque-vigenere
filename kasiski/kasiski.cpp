#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <string>
#define MIN_COUNT 3
#define MAX_COUNT 3

using namespace std;

typedef map<string, unsigned> SubstringMap;

/* Saves the given string with its offset in the given map.
 *
 * str - is the string used as the key.
 * offset - is the offset where string was located in its file.
 * strs - is the map where to save the data.
 */
static void save(const char * str, unsigned offset, SubstringMap &strs, 
                 set<unsigned> &offsets) 
{
    string ngram{str};
    auto result = strs.find(ngram);
    if (result == strs.end()) {
        strs.insert(pair<string, unsigned>(ngram, offset));
    } else {
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
void findRepeatedSubstrings(int src, SubstringMap &strs, set<unsigned> &offsets) {
	char buffer[MAX_COUNT];
	off_t end_offset = lseek(src, 0, SEEK_END);
	off_t offset = lseek(src, 0, SEEK_SET);
	unsigned count = MIN_COUNT;
	while (offset + count <= end_offset) {
		while (offset + count <= end_offset && count <= MAX_COUNT) {
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

static bool isDivisibleBy(unsigned distance, unsigned divisor){
    return distance%divisor == 0;
}

static void countNbDivisible(map<unsigned, unsigned> &divisors, std::set<unsigned> &distances){
    for(const auto &distance : distances) {
        for(unsigned divisor{2}; divisor < 30000; ++divisor){
            if(isDivisibleBy(distance, divisor)) {
                divisors[divisor]++;
            }
        }
    }
}

static unsigned findMaxDivisor(std::map<unsigned, unsigned> &m){
    unsigned max_key{2};
		for (const auto &p : m) {
			if (m[max_key] < p.second) {
				max_key = p.first;
			}
		}
    return max_key;
}

unsigned findKeyLength(set<unsigned> &distances) {
	map<unsigned, unsigned> divisors;
	countNbDivisible(divisors, distances);

	for (auto &p : divisors) {
		cout << "<" << p.first << ", " << p.second << ">" << endl;
	}

	return findMaxDivisor(divisors);
}

