# Frequential analysis
Assignment submitted by Logan Farci (47923) and Anthony Farci (49737) for the Security course of Mr Absil (ABS).

# How to use
## Command
```
./vigenere <cipher | uncipher> <source> <destination> <your key>
```
### Arguments
1. **Action:** 
   - ```cipher``` in order to cipher ```source```.
   - ```uncipher``` in order to uncipher ```source```.
2. **Source:** the path to the file to cipher.
3. **Destination:** the path to the file that will store the (un)ciphered text.
4. **Key:** is the key. It is a character string, each character corresponding to its position in the alphabet.

### Ciphering example
Let the file ```initial_message``` with the following content:

"*The Iglesia de Nuestra Señora de los Dolores de Dalcahue (in English, Church of Our Lady of Sorrows, or Church of Our Lady of Dolours of Dalcahue) is a Catholic church located in the Plaza de Armas of the town of Dalcahue, on Chiloé Island, Chile.*"

In order to cipher ```initial_message```, use the following command: 
```
./vigenere cipher initial_message ciphered_message example
```

The ciphered content is stored in the ```ciphered_message``` file and is:

"*Xee Uvwiwfa pt Yyiptdp Diñsoa pt wsw Aoxdciw ae Ppwgeeuq (xy Irdluhs, Glrrow zj Srr Xpoc sc Sagcsap, od Rsyvzh au Zyv Iapn zj Hllajcw sc Dmanelre) uh l Geqhaatg geudrs pszafto mr qhq Ewedx dq Pcqep or isi xlwz dq Heicmwfi, sk Ctxwsé Mplmco, Glflq.*"

## Uncipherin example
Let's uncipher the content of the ```ciphered_message``` file with our key: *example*. We use the following command: 
```
./vigenere uncipher ciphered_message unciphered_message example
```

```ciphered_message``` file contains the original message.

# Known bugs
- All the frequential analysis is based on the fact that the letter 'e' is the most frequent in English. Trying to attack a text that does not respect this assumption will result in gibberish. 
