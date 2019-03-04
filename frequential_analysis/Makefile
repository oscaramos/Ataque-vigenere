attack: keylength.o kasiski.o vigenere.o
	@g++ -o attack keylength.o kasiski.o vigenere.o

keylength.o: keylength.cpp
	@g++ -c keylength.cpp

kasiski.o: kasiski.cpp
	@g++ -c kasiski.cpp

vigenere.o: vigenere.cpp
	@g++ -c vigenere.cpp

clean:
	@rm *.o
	@rm attack

