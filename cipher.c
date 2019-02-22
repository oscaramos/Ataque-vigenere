/* This file contains all the functions useful to the Vigenere cipher. 
 *
 * author - Logan Farci (47923), group D113
 */

#include <unistd.h> //read, write
#include "ctype.h" //toupper
#define COUNT 1

/* Converts the given character to its position in the alphabet (from 0).
 *
 * c - is the character to convert.
 */
static unsigned toNumber(char c) {
	return toupper(c) - 'A';
}

/* Converts the given string to independant key values.
 *
 * For instance: 
 *	- key is "KEY"
 *  - after calling this function, values content is {11, 5, 25}.
 *
 * key - is a key user to cipher (or uncipher).
 * values - is an array that should contain the key values.
 */
void keyToValues(char *key, unsigned *values) {
	unsigned i;
	for	(i = 0; key[i] != '\0'; i++) {
		values[i] = toNumber(key[i]); 
	}
}

/* Increments the current step. When step has reached the limit, it is reset.
 *
 * current_step - is the current step.
 * limit - is the limit to the steps.
 */
static void next(unsigned *current_step, const unsigned limit) {
	if (*current_step == limit - 1) {
		*current_step = 0;
	} else {
		(*current_step)++;
	}
} 

/* Ciphers the given character with the given key.
 *
 * c - is the character to cipher.
 * key - is the key to cipher c with.
 */
static void char_cipher(char *c, long key) {
	for (unsigned i = 0; i < key; ++i) {
	  	switch (*c) {
	  		case 'Z':
	  			*c = 'A';
	  			break;
	  		case 'z':
	  			*c = 'a';
	  			break;
	  		default:
	  		(*c)++;
  	}
	}
}

/* Ciphers the content of src based on the given keys and stores the result in
 * dest.
 *
 * src - is the file descriptor of the file to cipher the content from.
 * dest - is the file desriptor of the file to store the ciphered content in.
 * keys - are the key values used to cipher.
 * keys_size - is the size of keys array.
 */
void cipher(int src, int dest, unsigned *keys, unsigned keys_size) {
	char buffer[COUNT];
	unsigned step = 0;
	while (read(src, buffer, COUNT) != 0) {
		if (isalpha(*buffer)) {
			char_cipher(buffer, keys[step]);
			next(&step, keys_size);
		}
		write(dest, buffer, COUNT);
	} 
}

/* Unciphers the given character with the given key.
 *
 * c - is the character to uncipher.
 * key - is the key to uncipher c with.
 */
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

/* Unciphers the content of src based on the given keys and stores the result 
 * in dest.
 *
 * src - is the file descriptor of the file to uncipher the content from.
 * dest - is the file desriptor of the file to store the unciphered content in.
 * keys - are the key values used to uncipher.
 * keys_size - is the size of keys array.
 */
void uncipher(int src, int dest, unsigned *keys, unsigned keys_size) {
	char buffer[COUNT];
	unsigned step = 0;
	while (read(src, buffer, COUNT) != 0) {
		if (isalpha(*buffer)) {
			char_uncipher(buffer, keys[step]);
			next(&step, keys_size);
		}
		write(dest, buffer, COUNT);
	} 
}
