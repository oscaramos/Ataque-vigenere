vigenere: cipher.o main.o
	@gcc -o vigenere cipher.o main.o

cipher.o: ./cipher/cipher.c
	@gcc -c ./cipher/cipher.c

main.o: main.c
	@gcc -c main.c
