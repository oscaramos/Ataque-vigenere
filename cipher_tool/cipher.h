/* Project: vigenere_cipher
 * Description: this file contains the declarations of functions useful to the Vigenere 
 * cipher.
 */

void keyToValues(char *, unsigned *);

void cipher(int, int, unsigned *, unsigned);

void uncipher(int, int, unsigned *, unsigned);

void caesarFrequentialAnalysisAttack(int, int);
