# Soluce
```
bonus3@RainFall:~$ ./bonus3 ""
```

## Description du code

-   2 variable dont declare : v5 qui sera un fd, est ptr qui servira de buffer pour lire dans fichier
-   le fd contenant le mot de passe est ouvert et stocker dans v5
-   si v5 est null ou argc est different de 2 cela quitte le programme
-   le mot de passe est lu et stocker dans ptr
-   un \0 est mis a l'index du resultat de atoi de argv[1]
-   si argv[1] et ptr sont egale un shell est lancer

Du coup, si argv[1] est vide cela mettra a l'index 0 un \0, et si cela arrive alors ptr lui aussi est vide lors de la comparaison entre les 2

