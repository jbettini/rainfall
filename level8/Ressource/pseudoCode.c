/* 
PseudoCode :
-----------
Variables
-----------

globalAuth
globalService

char*   s1[2]   (local_8b)
char*   s2[125] (acStack_89)

Entry   (entry)
pbVar1  (pbVar5)
pbVar2  (pbVar6)

pcVar1  (pcVar2)

bVar1   (bVar7)
bVar2   (bVar10)
bVar3   (bVar12) = 0;

iVar1   (iVar3)

uVar1   (uVar8)
uVar2   (uVar11)
uVar3   (uVar4)
uVar4   (uVar9)
----------
Code
----------

Boucle sur true:
    --------------------------------------
    [PREMIER BLOC]

    affiche l'adresse de globalAuth et globalService
    lis 128 char sur stdin, le stock dans un buffer(enry), et recupere le retour de fgets dans pcVar1
    assigne false a bVar1
    assigne a bVar2 le retour de la condition if (pcVar1 == NULL) si elle est true quitte le programme
    assigne 5 a iVar1
    assigne entry a pbVar1
    assigne a pbVar6 la chaine "auth "
    do while tan que bVar2 :
        si iVar1 est egale a 0:
            break;
        iVar1 -= 1
        si pbVar1[0] < pbVar[0] ? bVar1 = true : bVar1 = false
        si pbVar1[0] == pbVar[0]? bVar2 = true : bVar2 = false
        si bVar3 ? avance l'adresse de pbVar1 de 1 : recule l'adresse de pbVar1 de 1
        si bVar3 ? avance l'adresse de pbVar2 de 1 : recule l'adresse de pbVar2 de 1
    --------------------------------------
    [DEUXIEME BLOC]

    assigne a uVar1 0
    si le contraire de bVar1 et bVar2 == bVar1 ? uVar2 = true : uVar2 = false
    si uVar2 est different de 0 :
        globalAuth = malloc(4)
        globalAuth[0] = 0;
        assigne a uVar3 HEXA_MAX (0xffffffff)
        assigne s1 a pcVar1
        do while tan que c1 != '\0':
            si uVar3 == 0:
                break:
            ubarVar3 -= 1
            c1 = pcVar[0]
            si bVar3 ? avance l'adresse de pcVar1 de 1 : recule l'adresse de pcVar1 de 1
        inverse tout les byte de uVar3 et soustrait 1
        si uVar3 est inferieur a 1e (30) ? uVar1 = true : uVar1 = false
        si uVar3 est egale a 1e (30) ? uVar2 = true : uVar2 = false
        si uVar3 est inferieur a 31:
            copie s1 dans globalAuth 
    
    --------------------------------------
    [TROISIEME BLOC]

    assigne 5 a iVar1
    assigne entry a pbVar1
    assigne la string "reset" a pbVar2
    do while tan que uVar2 :
        si iVar1 == 0 :
            break;
        iVar1 -= 1
        si pbVar1[0] < pbVar[0] ? uVar1 = true : uVar1 = false
        si pbVar1[0] == pbVar[0]? uVar2 = true : uVar2 = false
        si bVar3 ? avance l'adresse de pbVar1 de 1 : recule l'adresse de pbVar1 de 1
        si bVar3 ? avance l'adresse de pbVar2 de 1 : recule l'adresse de pbVar2 de 1
    uVar4 = 0;
    si le contraire de uVar1 et uVar2 == uVar1 ? uVar1 = true : uVar1 = false
    si uVar1:
        free(globalAuth)

    --------------------------------------
    [QUATRIEME BLOC]

    iVar = 6
    assigne entry a pbVar1
    assigne la string "reset" a pbVar2
    do while tan que uVar1 :
        si iVar1 == 0 :
            break;
        ivar1 -= 1;
        si pbVar1[0] < pbVar[0] ? uVar4 = true : uVar4 = false
        si pbVar1[0] == pbVar[0]? uVar1 = true : uVar1 = false
        si bVar3 ? avance l'adresse de pbVar1 de 1 : recule l'adresse de pbVar1 de 1
        si bVar3 ? avance l'adresse de pbVar2 de 1 : recule l'adresse de pbVar2 de 1
    uVar2 = 0
    si le contraire de uVar1 et uVar4 == uVar4 ? uVar1 = true : uVar1 = false
    si uVar1 :
        si l'adresse de entry est inferieur a 0xfffffff8 ? uVar2 = true : uVar2 = false
        si s2 est egale a 0x0 ? uVar1 = true : uVar1 = false
        assigne a service une copie de s2

    --------------------------------------
    [CINQUIEME BLOC]

    iVar1 = 5
    assigne entry a pbVar1
    assigne la string "login" a pbVar2
    do while tan que uVar1 :
        si iVar1 == 0 :
            break;
        ivar1 -= 1;
        si pbVar1[0] < pbVar[0] ? uVar2 = true : uVar2 = false
        si pbVar1[0] == pbVar[0]? uVar1 = true : uVar1 = false
        si bVar3 ? avance l'adresse de pbVar1 de 1 : recule l'adresse de pbVar1 de 1
        si bVar3 ? avance l'adresse de pbVar2 de 1 : recule l'adresse de pbVar2 de 1

    si le contraire de uVar1 et uVar2 == uVar2:
        si auth[8] == 0:
            print sur stdout (Password:\n)
        sinon
            lance un shell
*/


