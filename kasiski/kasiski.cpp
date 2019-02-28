#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <string>
#define MIN_COUNT 3
#define MAX_COUNT 6

typedef std::map<std::string, std::vector<unsigned>> SubstringMap;

/* Saves the given string with its offset in the given map.
 *
 * str - is the string used as the key.
 * offset - is the offset where string was located in its file.
 * strs - is the map where to save the data.
 */
static void save(const char * str, unsigned offset, SubstringMap &strs) {
    std::string s{str};
    auto result = strs.find(s);
    if (result == strs.end()) {
        std::vector<unsigned> offsets{offset};
        strs.insert(std::pair<std::string, std::vector<unsigned>>(s, offsets));
    } else {
        result->second.push_back(offset);    
    }
}

/* Finds all the repeated substrings in the file described by src of a size
 * between min_size and maxsize.
 *
 * src - is the file descriptor of the file to search in.
 * min_size - is the minimal size of the subtrings to search for.
 * max_size - is the maximal size of the subtrings to search for.
 */
void findRepeatedSubstrings(int src, SubstringMap &strs) {
	char buffer[MAX_COUNT];
	off_t end_offset = lseek(src, 0, SEEK_END);
	off_t offset = lseek(src, 0, SEEK_SET);
	unsigned n_read = -1;
	unsigned count = MIN_COUNT;
	while (offset + count <= end_offset) {
		count = MIN_COUNT;
		while (offset + count <= end_offset && count <= MAX_COUNT) {
			n_read = read(src, buffer, count);
			buffer[count] = '\0';
      save(buffer, offset, strs);
			lseek(src, offset, SEEK_SET);
			count++;
		}
		offset++;
	}
}
