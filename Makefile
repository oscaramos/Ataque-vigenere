vigenere: cipher.o vigenere.o
	@gcc -o vigenere cipher.o vigenere.o
	@rm cipher.o
	@rm vigenere.o

cipher.o: cipher.c
	@gcc -c cipher.c

vigenere.o: vigenere.c
	@gcc -c vigenere.c
