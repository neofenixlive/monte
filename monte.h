#ifndef __MONTE
#define __MONTE

enum monte_commands { DF, AD, SB, ML, DV, JP, EQ, NE, GT, LT, O, I };
typedef struct {
  signed char *ptr, *prg;
  unsigned long prg_l, prg_c;
} monte;

monte *monte_create();
void monte_delete(monte*);
void monte_new_program(monte*, char*);
void monte_run_program(monte*);

#endif
