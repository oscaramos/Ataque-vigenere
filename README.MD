# Descripcion
Tarea para el curso de seguridad

# Compilacion
Usar el comando
```
$ g++ Main.cpp frequential_analysis/kasiski.cpp frequential_analysis/keylength.cpp cipher_tool/cipher.c -o vigenere
```
# ¿Como usar?
## Plantilla de comandos
```
./vigenere <cipher | uncipher | attack> <source> <destination> [key]
```

### Argumentos
1. **Action:** 
   - ```cipher``` para cifrar ```source``` y guardar el resultado en ```destination```.
   - ```uncipher``` para descifrar ```source``` y guardar el resultado en ```destination```.
   - ```attack``` Para hacer un analisis de frecuencia en ```source``` y guardar el resultado en ```destination```.
2. **Source:** La direccion del archivo a cifrar
3. **Destination:** La direccion del archivo a guardar el resultado
4. **Key:** Es la llave, una cadena de texto, cada llave corresponde a una posicion en el alfabeto. Si se esta atacando puede omitir este campo.


### Ejemplos
#### Cifrando un texto
```
./vigenere cipher original.txt ciphed.txt KEY
```

#### Descifrando un texto
```
./vigenere uncipher ciphed.txt unciphered.txt KEY
```

#### Ataque por analisis de frecuencia
```
./vigenere attack ciphed.txt unciphered_by_attack.txt
```


# Prueba
## Archivo de prueba
Se uso este texto de ejemplo para comprobar la utilidad del algoritmo:
```
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus quis dapibus lectus, ut sollicitudin sapien. Morbi non pretium lectus. Nullam et dapibus felis, eu rhoncus urna. In pellentesque rhoncus dolor, ut aliquam quam molestie eu. Etiam euismod tempus bibendum. Vestibulum id dignissim tellus. Nunc elementum libero nibh, a egestas ex ornare ac. Aliquam efficitur scelerisque dictum. Donec aliquet dolor nulla, in condimentum nisi molestie et. Cras porttitor augue eget nisi imperdiet, et ultrices diam commodo. Pellentesque bibendum tristique purus, ac finibus dui euismod id. Maecenas sit amet sagittis risus. Duis at nunc a ligula molestie tincidunt.

Aliquam quis purus quis nulla blandit elementum at at magna. Integer turpis velit, maximus ut vulputate sit amet, vehicula sit amet felis. Maecenas suscipit aliquam lectus vitae auctor. Etiam at massa orci. Proin gravida leo elit, dictum consequat nisi aliquam id. Praesent gravida tempus mauris vitae scelerisque. Duis venenatis eleifend eros at sodales. Quisque quis magna eget erat lacinia molestie ut non massa. Donec eget magna facilisis, accumsan libero sed, scelerisque dolor. Sed efficitur sagittis tincidunt. Etiam lacus leo, tincidunt nec rutrum nec, gravida sit amet ex. Donec in leo non lectus porttitor sollicitudin. Pellentesque vitae ex massa.

Donec ac nibh sagittis, suscipit ligula nec, bibendum metus. Aliquam enim tortor, sodales finibus volutpat ac, sollicitudin consequat massa. Suspendisse potenti. Etiam efficitur quam velit. Ut porta tellus ac diam lobortis, non ullamcorper nisl mollis. Nullam sit amet ornare lorem, eget maximus lectus. Fusce id orci ut justo molestie ornare. In hac habitasse platea dictumst. Vivamus sed tristique elit. Donec at euismod nunc. Donec feugiat tempus consectetur. Maecenas mollis dolor lectus, et vehicula metus tincidunt a.

Suspendisse tincidunt ultrices ipsum in iaculis. Duis id malesuada dolor, nec semper diam. Praesent scelerisque commodo eleifend. Vivamus vitae consequat ex, non tristique risus. Pellentesque blandit arcu sit amet ligula accumsan molestie. Suspendisse in nibh lectus. Aenean varius urna et quam blandit, sit amet tristique arcu lacinia. Nam fermentum egestas eleifend. Donec ligula urna, dignissim a velit non, scelerisque dictum magna. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Cras suscipit semper semper. Etiam condimentum nunc et nulla efficitur volutpat.

Donec gravida, felis a ullamcorper euismod, massa dolor accumsan dui, luctus finibus arcu orci sit amet turpis. Duis in venenatis ante. Duis dictum interdum vulputate. Aliquam auctor lobortis mi. Cras rhoncus viverra elit a dapibus. Donec molestie erat vitae arcu faucibus, sed condimentum lectus convallis. In hac habitasse platea dictumst. Suspendisse potenti. Pellentesque condimentum molestie felis. Maecenas pellentesque tellus ut semper rhoncus. Pellentesque sodales et ex eget scelerisque. Etiam blandit efficitur tortor egestas tempor.

Morbi vitae pretium erat, a vehicula sem. Ut et accumsan orci, vitae pharetra ex. Praesent imperdiet accumsan nunc vestibulum congue. Duis eget diam in turpis ullamcorper facilisis a vitae nisl. Proin scelerisque felis ut nunc porta rutrum. Mauris sed neque augue. Praesent sodales neque aliquet, congue tellus at, scelerisque est.

Curabitur quis nisi id tellus consectetur bibendum nec quis erat. Aenean sit amet erat quis sapien consequat efficitur. Aliquam erat volutpat.
```

Con la llave "KEY"

Este archivo esta contenido en el archivo `original.txt`

## Resultado
Se comprobo que usando la secuencia de comandos descritos arriba se puede descrifrar efectivamente el texto de prueba
el archivo resultante tiene como nombre `unciphered_by_attack.txt` 