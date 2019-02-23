/* This file contains the declarations of functions useful to the Vigenere 
 * cipher.
 *
 * author - Logan Farci (47923), group D113
 */

void keyToValues(char *, unsigned *);

void cipher(int, int, unsigned *, unsigned);

void uncipher(int, int, unsigned *, unsigned);

void caesarFrequentialAnalysisAttack(int, int);
