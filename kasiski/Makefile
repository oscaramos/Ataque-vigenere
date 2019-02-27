run: kasiski.o vigenere.o
	g++ -o run kasiski.o vigenere.o

kasiski.o: kasiski.cpp
	g++ -c kasiski.cpp

vigenere.o: vigenere.cpp
	g++ -c vigenere.cpp

clean:
	rm *.o
	rm run

