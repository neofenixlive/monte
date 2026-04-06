#include "neo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

signed char *neo_pointer(neo *n, signed char k) {
  int i;
  for(i = 0; i < n->ptr; i++) {
    if(n->ptr_k[i] == k) return &n->ptr_v[i];
  }
  n->ptr = i + 1;
  n->ptr_k = realloc(n->ptr_k, sizeof(char)*n->ptr);
  n->ptr_v = realloc(n->ptr_v, sizeof(char)*n->ptr);
  n->ptr_k[i] = k; n->ptr_v[i] = 0;
  return &n->ptr_v[i];
}

neo *neo_create() {
  neo *n = malloc(sizeof(neo));
  n->ptr = 0;
  n->prg_l = 0;
  n->prg_c = 0;
  n->ptr_k = NULL;
  n->ptr_v = NULL;
  n->prg = NULL;
  return n;
}

void neo_delete(neo *n) {
  free(n->ptr_k);
  free(n->ptr_v);
  free(n->prg);
  free(n);
}

void neo_new_program(neo *n, char *txt) {
  char *str = NULL;
  char c = 1, s = 0;
  long i = 0, l = 0;
  free(n->prg); n->prg = NULL;
  while(c != '\0') {
    c = txt[i];
    if(c == '#') s = !s;
    else if(!s && c == ';') {
      signed char datcom[3] = { 0, 0, 0 };
      char *partxt[3] = { NULL, NULL, NULL };
      char *t = strtok(str, ",");
      int i = 0;
      while(t != NULL) {
        partxt[i] = t;
        t = strtok(NULL, ",");
        i++;
      }
      switch(partxt[0][0]) {
        case ':': datcom[0] = DF; break;
        case '+': datcom[0] = AD; break;
        case '-': datcom[0] = SB; break;
        case '*': datcom[0] = ML; break;
        case '/': datcom[0] = DV; break;
        case '&': datcom[0] = JP; break;
        case '=': datcom[0] = EQ; break;
        case '~': datcom[0] = NE; break;
        case '>': datcom[0] = GT; break;
        case '<': datcom[0] = LT; break;
        case '!': datcom[0] = O;  break;
        case '?': datcom[0] = I;  break;
      }
      datcom[0] = (datcom[0] & 0xF) << 4;
      if(partxt[1]) {
        if(partxt[1][0] == '$') {
          datcom[0] |= 1;
          datcom[1]      = (signed char) strtol(&partxt[1][1], NULL, 10);
        } else datcom[1] = (signed char) strtol(&partxt[1][0], NULL, 10);
      }
      if(partxt[2]) {
        if(partxt[2][0] == '$') {
          datcom[0] |= 2;
          datcom[2]      = (signed char) strtol(&partxt[2][1], NULL, 10);
        } else datcom[2] = (signed char) strtol(&partxt[2][0], NULL, 10);
      }
      if(n->prg == NULL) n->prg_l = 0;
      n->prg = realloc(n->prg, sizeof(char)*(n->prg_l+3));
      n->prg[n->prg_l]   = datcom[0];
      n->prg[n->prg_l+1] = datcom[1];
      n->prg[n->prg_l+2] = datcom[2];
      n->prg_l += 3;
      free(str); str = NULL;
    }
    if(!s && !(c == '\n' || c == '\t' || c == ' ' || c == '#' || c == ';')) {
      if(str == NULL) l = 0;
      str = realloc(str, l+2);
      str[l] = c;
      str[l+1] = '\0';
      l++;
    }
    i++;
  }
}

void neo_run_program(neo *n) {
  printf("\033[H\033[2J");
  n->prg_c = 0;
  while(n->prg_c < n->prg_l) {
    signed char *com    = &n->prg[n->prg_c];
    signed char *dat[2] = { NULL, NULL };
    switch(com[0] & 0xF) {
      case 0: dat[0] = &com[1];                dat[1] = &com[2]; break;
      case 1: dat[0] = neo_pointer(n, com[1]); dat[1] = &com[2]; break;
      case 2: dat[0] = &com[1];                dat[1] = neo_pointer(n, com[2]); break;
      case 3: dat[0] = neo_pointer(n, com[1]); dat[1] = neo_pointer(n, com[2]); break;
    }
    switch((com[0] >> 4) & 0xF) {
      case DF: *dat[0] = *dat[1];           break;
      case AD: *dat[0] = *dat[0] + *dat[1]; break;
      case SB: *dat[0] = *dat[0] - *dat[1]; break;
      case ML: *dat[0] = *dat[0] * *dat[1]; break;
      case DV: *dat[0] = *dat[0] / *dat[1]; break;
      case JP: n->prg_c = *dat[0]*3-3;      break;
      case EQ: if(*dat[0] == 0) n->prg_c += *dat[1]*3-3; break;
      case NE: if(*dat[0] != 0) n->prg_c += *dat[1]*3-3; break;
      case GT: if(*dat[0] > 0)  n->prg_c += *dat[1]*3-3; break;
      case LT: if(*dat[0] < 0)  n->prg_c += *dat[1]*3-3; break;
      case O: printf("%c", *dat[0]); break;
      case I: scanf(" %c", dat[0]);  break;
    }
    n->prg_c += 3;
  }
}