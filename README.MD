# Frequential analysis tool
Assignment submitted by Logan Farci (47923) and Anthony Farci (49737) for the Security course of Mr Absil (ABS) at HE2B ESI (2018-2019).

# Setup
Once the project is cloned locally, run the following commands...
```
$ cd <path to the project directory>
$ make
```
**Note:** the code use the POSIX operating system API. This code only run Unix systems.
# How to use
## General ommand
```
./sec <cipher | uncipher | attack> <-vigenere | -caesar> <source> <destination> [your key]
```

### Arguments
1. **Action:** 
   - ```cipher``` in order to cipher ```source``` and store the result in ```destination```.
   - ```uncipher``` in order to uncipher ```source``` and store the result in ```destination```.
   - ```attack``` in order to make frequential analysis on ```source``` and store the result in ```destination```.
2. **Cipher type:**
    - ```-vigenere``` in order to use *Vigenère* ciphering.
    - ```-caesar``` in order to use *Caesar* ciphering.
2. **Source:** the path to the file to read from.
3. **Destination:** the path to the file that will store the result of the treatment.
4. **Key:** is the key. It is a character string, each character corresponding to its position in the alphabet. A key of size 1 should be used with Caesar cipher. Also, when attacking the key optional (when given it is not treated).

### Examples
#### Ciphering example
In order to cipher ```initial_message```, use the following command: 
```
./sec cipher -vigenere initial_message ciphered_message example
```
The ciphered content is stored in the ```ciphered_message``` file.

**Note:** ciphering in Vigenère or in Caesar will be treated the same way.

#### Unciphering example
In order to uncipher ```ciphered_message```, use the following command: 
```
./sec uncipher -vigenere ciphered_message unciphered_message example
```
The uciphered content is stored in the ```unciphered_message``` file.

**Note:** unciphering Vigenère or Caesar will be treated the same way.

#### Vigenère frequential analysis example
In order to break ```ciphered_message```, use the following command: 
```
./sec attack -vigenere ciphered_message broken_message
```
The uciphered content is stored in the ```broken_message``` file.

#### Caesar frequential analysis example
In order to break ```ciphered_message```, use the following command: 
```
./sec attack -caesar ciphered_message broken_message
```
The uciphered content is stored in the ```broken_message``` file.

# Known bugs
This frequential analysis implementation is based on the fact that the letter 'e' is the most frequent in English. Trying to analyse and uncipher (without the key) a text that does not respect this assumption will result in gibberish.

Also, during the frequential analysis, if the key length divides the text in subtexts with a different most frequent character than 'e', the analysis won't be abble to find the right key.
