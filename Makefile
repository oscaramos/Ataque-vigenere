vigenere: cipher.o main.o
	@gcc -o vigenere cipher.o main.o

cipher.o: ./cipher_tool/cipher.c
	@gcc -c ./cipher_tool/cipher.c

main.o: main.c
	@gcc -c main.c
