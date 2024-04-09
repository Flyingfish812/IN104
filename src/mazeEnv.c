#include "mazeEnv.h"
#include "functions.h"

char** mazeEnv;  // The list that stores the maze
int** visited;
int rows;
int cols;
int start_row;
int start_col;
int state_row;
int state_col;
int goal_row;
int goal_col;

// Allocate a memory for the maze
void alloc_mazeEnv(){
    mazeEnv = malloc(rows * sizeof(char*));

    for(int i=0; i<rows; i++) {
        mazeEnv[i] = malloc(cols * sizeof(char*));
    }
}

// Read the maze from a file
void mazeEnv_make(char* file_name){
    char c;
    char rows_s[3] ={'\0'};
    char cols_s[3] ={'\0'};
    int rows_i = 0;
    int cols_i = 0;
    int swap = 0;

    FILE* file = fopen(file_name, "r");
    if (file == NULL){
    printf("Pointeur nul");
    }

    if (file) {
        /* lire la premiere ligne avant EOF */
        while( (c=getc(file)) != EOF) {
            if(c== '\n'){
                break;
            } else if (c==',') {
                swap = 1;
            } else if (!swap) {
                rows_s[rows_i]=c;
                rows_i++;
            } else {
                cols_s[cols_i]= c;
                cols_i++;
            }
        }
    }

    /* convertir le string en nombre */
    rows = atoi(rows_s);
    cols = atoi(cols_s);

    alloc_mazeEnv();

    for (int i=0; i<rows; i++){
        for (int j=0; j < cols; j++){
            c = getc(file);

            if (c=='\n'){
                c = getc(file);
            } else if (c == 's'){
            start_row = i;
            start_col = j;
            } else if ( c == 'g'){
            goal_row = i;
            goal_col = j;
            }

            mazeEnv[i][j] = c;
        }
    }

    fclose(file);
}

// Print the maze
void mazeEnv_render(){
    for (int i=0; i<rows; i++) {
        for (int j=0; j< cols; j++){
            printf("%c ", mazeEnv[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void mazeEnv_render_pos(){
    mazeEnv[state_row][state_col] = 'o';
    for (int i=0; i<rows; i++) {
        for (int j=0; j< cols; j++){
            printf("%c ", mazeEnv[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Reset the maze to the initial state
void mazeEnv_reset(){
    state_row = start_row;
    state_col = start_col;
}

// Faire une action & observer récompense et où on se trouve
envOutput mazeEnv_step(action a){
    int reward = 0;
    int done = 0;
    envOutput stepOut;

    if (a==up){
       state_row = max(0,state_row -1);
    }else if (a==down){
       state_row = min(rows,state_row +1);
    }else if (a==right){
       state_col = min(cols,state_col +1);
    }else if (a==left){
       state_col = max(0,state_col -1);
    }
    
    if((state_row == goal_row) && (state_col == goal_col)){
       reward = 10000;
       done   = 1;
    }

    stepOut.reward = reward;
    stepOut.done   = done;
    stepOut.new_col = state_col;
    stepOut.new_row = state_row; 

    return stepOut;
}

action env_action_sample(){
    return (enum action)(rand() % number_actions);
}

void alloc_visited()
{
    visited = malloc(rows * sizeof(int*));
    int i;
    for (i = 0; i < rows; ++i){
            visited[i] = malloc(cols * sizeof(int*));
    }
}


void init_visited()
{
    alloc_visited();

    int i, j;
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            if (mazeEnv[i][j] == '+') {
                visited[i][j] = wall;
            } else if (mazeEnv[i][j] == 'g') {
                visited[i][j] = goal;
            } else {
                visited[i][j] = unknown;
            }
        }
    }
}

void add_crumbs(){
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            if (visited[i][j] ==crumb){
                mazeEnv[i][j] ='.';
            }
        }
    }
    mazeEnv[start_row][start_col]= 's';
}

