#ifndef MAZEENV_H
#define MAZEENV_H

#include <stdio.h>
#include <stdlib.h>

extern char** mazeEnv;
extern int** visited;
extern int rows;
extern int cols;
extern int start_row;
extern int start_col;
extern int state_row;
extern int state_col;
extern int goal_row;
extern int goal_col;

enum terrain{
    unknown,
    wall,
    known,
    goal,
    crumb
};

enum action{
     up,
     down,
     left,
     right,
     number_actions
};

typedef enum action action ;

struct envOutput{
  int new_col;
  int new_row;
  int reward;
  int done;

};

typedef struct envOutput envOutput;

void alloc_mazeEnv();

void mazeEnv_make(char* );

void mazeEnv_render();

void mazeEnv_render_pos();

void mazeEnv_reset();

envOutput mazeEnv_step(action a);

action env_action_sample();

void alloc_visited();

void init_visited();

void add_crumbs();

#endif /* MAZEENV_H */
