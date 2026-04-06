#ifndef __NEOSCRIPT
#define __NEOSCRIPT

enum neo_commands { DF, AD, SB, ML, DV, JP, EQ, NE, GT, LT, O, I };
typedef struct {
  signed char *ptr_k, *ptr_v, *prg;
  unsigned long ptr, prg_l, prg_c;
} neo;

signed char *neo_pointer(neo*, signed char);
neo *neo_create();
void neo_delete(neo*);
void neo_new_program(neo*, char*);
void neo_run_program(neo*);

#endif