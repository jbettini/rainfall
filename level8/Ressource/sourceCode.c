undefined4 main(void)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  bool bVar7;
  undefined uVar8;
  undefined uVar9;
  bool bVar10;
  undefined uVar11;
  byte bVar12;
  byte entry [5];
  char local_8b [2];
  char acStack_89 [125];
  
  bVar12 = 0;
  do {
    printf("%p, %p \n",auth,service);
    pcVar2 = fgets((char *)entry,0x80,stdin);
    bVar7 = false;
    bVar10 = pcVar2 == (char *)0x0;
    if (bVar10) {
      return 0;
    }
    iVar3 = 5;
    pbVar5 = entry;
    pbVar6 = (byte *)"auth ";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      bVar7 = *pbVar5 < *pbVar6;
      bVar10 = *pbVar5 == *pbVar6;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
      pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    } while (bVar10);
    uVar8 = 0;
    uVar11 = (!bVar7 && !bVar10) == bVar7;
    if ((bool)uVar11) {
      auth = (undefined4 *)malloc(4);
      *auth = 0;
      uVar4 = 0xffffffff;
      pcVar2 = local_8b;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar2;
        pcVar2 = pcVar2 + (uint)bVar12 * -2 + 1;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4 - 1;
      uVar8 = uVar4 < 0x1e;
      uVar11 = uVar4 == 0x1e;
      if (uVar4 < 0x1f) {
        strcpy((char *)auth,local_8b);
      }
    }
    iVar3 = 5;
    pbVar5 = entry;
    pbVar6 = (byte *)"reset";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      uVar8 = *pbVar5 < *pbVar6;
      uVar11 = *pbVar5 == *pbVar6;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
      pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    } while ((bool)uVar11);
    uVar9 = 0;
    uVar8 = (!(bool)uVar8 && !(bool)uVar11) == (bool)uVar8;
    if ((bool)uVar8) {
      free(auth);
    }
    iVar3 = 6;
    pbVar5 = entry;
    pbVar6 = (byte *)"service";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      uVar9 = *pbVar5 < *pbVar6;
      uVar8 = *pbVar5 == *pbVar6;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
      pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    } while ((bool)uVar8);
    uVar11 = 0;
    uVar8 = (!(bool)uVar9 && !(bool)uVar8) == (bool)uVar9;
    if ((bool)uVar8) {
      uVar11 = (byte *)0xfffffff8 < entry;
      uVar8 = acStack_89 == (char *)0x0;
      service = strdup(acStack_89);
    }
    iVar3 = 5;
    pbVar5 = entry;
    pbVar6 = (byte *)"login";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      uVar11 = *pbVar5 < *pbVar6;
      uVar8 = *pbVar5 == *pbVar6;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
      pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    } while ((bool)uVar8);
    if ((!(bool)uVar11 && !(bool)uVar8) == (bool)uVar11) {
      if (auth[8] == 0) {
        fwrite("Password:\n",1,10,stdout);
      }
      else {
        system("/bin/sh");
      }
    }
  } while( true );
}

// Cleaned Version 


** Very lightweight cleaned code, can contain mistakes **
** it is just for the logic

int *globalAuth;
int *globalService;


void main(void) {
    int ret;
    char buf[5];
    char s1[128];
    printf("%p %p", globalAuth, globalService);
    ret = fgets(buf,0x80,1);
    if (!ret)
        return ;
    if (!strcmp(buf, "auth "))
        globalAuth = malloc(4);
    else if (!strcmp(buf, "reset"))
        free(globalAuth);
    else if (!strcmp(buf, "service"))
        globalService = strdup(s1);
    else if (!strcmp(buf, "login")) {
        if (globalAuth[8] == 0)
            printf("PASSWORD:\n");
        else
            system("/bin/sh");
    }
}
