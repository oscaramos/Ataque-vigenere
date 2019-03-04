vigenere: cipher.o kasiski.o keylength.o Main.o
	@g++ -o vigenere cipher.o kasiski.o keylength.o Main.o

cipher.o: ./cipher_tool/cipher.c
	@g++ -c cipher_tool/cipher.c

kasiski.o: ./frequential_analysis/kasiski.cpp
	@g++ -c frequential_analysis/kasiski.cpp

keylength.o: ./frequential_analysis/keylength.cpp
	@g++ -c frequential_analysis/keylength.cpp

Main.o: Main.cpp
	@g++ -c Main.cpp

clean:
	@rm *.o
