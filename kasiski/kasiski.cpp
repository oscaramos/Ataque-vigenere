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
#define MIN_COUNT 5 // Replace this value
#define MAX_COUNT 5 // Replace this value
#define MAX_DIVISOR 30000

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
static void findRepeatedSubstrings(int src, SubstringMap &strs, set<unsigned> &offsets) {
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

/* Tells if a is divisible by b.
 *
 * a - is the dividend.
 * b - is the divisor.
 */
static bool isDivisibleBy(unsigned a, unsigned b){
    return a % b == 0;
}

/* Counts the number of distances divisible by a given divisor.
 *
 * divisors - is an empty map.
 * distances - are the distances between the different repeated substrings.
 */
static void countNbDivisibleDistances(map<unsigned, unsigned> &divisors, set<unsigned> &distances) {
    const unsigned min_divisor = 2;
		for (const auto &distance : distances) {
        for (unsigned divisor{min_divisor}; divisor < MAX_DIVISOR; ++divisor) {
            if (isDivisibleBy(distance, divisor)) {
                divisors[divisor]++;
            }
        }
    }
}

/* Finds the divisor that can divide the most of the distances.
 *
 * m - is a map on the divisor with the divisor number of possible distance
 * division.
 */
static unsigned findMaxDivisor(std::map<unsigned, unsigned> &m){
    unsigned max_key{2};
		for (const auto &p : m) {
			if (m[max_key] < p.second) {
				max_key = p.first;
			}
		}
    return max_key;
}

/* Finds the length of the key used to cipher a text with Vigènere cipher.
 *
 * distances - are the distances between all the substring.
 */
static unsigned findKeyLength(set<unsigned> &distances) {
  map<unsigned, unsigned> divisors;
	countNbDivisibleDistances(divisors, distances);
	return findMaxDivisor(divisors);
}

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
	lseek(src, offset, SEEK_SET);
	while (read(src, currentCharacter, 1) != 0) {
		frequencies[*currentCharacter]++;
		lseek(src, key_length, SEEK_CUR);
	} 
}

static char findMostFrequentChar(std::map<char, unsigned> &m){
    unsigned max_key{'a'};
		for (const auto &p : m) {
			if (m[max_key] < p.second) {
				max_key = p.first;
			}
		}
    return max_key;
}

static unsigned toNumber(char c) {
	return toupper(c) - 'A';
}

static void char_uncipher(char *c, long key) {
	for (unsigned i = 0; i < key; ++i) {
		switch (*c) {
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
	while (read(src, currentCharacter, 1) != 0) {
		if (isalpha(*currentCharacter)) {
			if (treated % key_length == 0) {
				char_uncipher(currentCharacter, key);
				lseek(src, -1, SEEK_CUR);
				write(src, currentCharacter, 1);
			}
			treated++;
		}
		current_offset++;
	}
}

static int getTmpFile(int src) {
	int tmp = open("tmp", O_RDWR | O_CREAT, 0666);
	char buffer[1];
	lseek(tmp, 0, SEEK_SET);
	while (read(src, buffer, 1) != 0) {
		if (isalpha(*buffer)) {
			*buffer = tolower(*buffer);
			write(tmp, buffer, 1);
		}
	}
	return tmp;
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
void attack(int src) {
	map<string, unsigned> substrings;
	set<unsigned> distances;
	int tmp = getTmpFile(src);
	findRepeatedSubstrings(tmp, substrings, distances);
	unsigned key_length = findKeyLength(distances);
	cout << "Key length is " << key_length << endl;
	for (unsigned offset = 0; offset < key_length; ++offset) {
		map<char, unsigned> frequencies;
		countCharFrequencies(tmp, frequencies, offset, key_length);
		char mostFrequent = findMostFrequentChar(frequencies);
		unsigned key = getKey(mostFrequent);
		uncipher_subpart(src, offset, key_length, key);
		unlink("tmp");
	}
}
