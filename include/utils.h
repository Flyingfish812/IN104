#include "mazeEnv.h"
#include <float.h>
#define NUM_ACTIONS 4 // Assuming 4 possible actions: up, down, left, right
#define ALPHA 0.1     // Learning rate
#define GAMMA 0.9     // Discount factor
#define EPSILON 0.9   // For ε-greedy strategy

extern int directions[4][2];

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    char** mazeEnv;
    int rows;
    int cols;
    Position start;
    Position goal;
} Maze;

int chooseAction(int row, int col, double*** qValues, Maze maze, double epsilon);

double*** createQValuesTable(int rows, int cols, Maze maze);

void freeQValuesTable(double*** qValues, int rows, int cols);

void updateQValue(double*** qValues, int currentState[2], int action, int nextState[2], double reward);

void updateQValueSARSA(double*** qValues, int currentState[2], int action, int nextState[2], double reward, int nextAction);

void stepEnvironment(Maze maze, int** visited, int currentState[2], int action, int nextState[2], double* reward, int* done);

void qLearning(Maze maze, double*** qValues, int EPS);

int* sarsaLearning(Maze maze, double*** qValues, int EPS);

void exportToFile(int* list, int size, const char* filename);