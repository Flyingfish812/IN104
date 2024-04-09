#include "mazeEnv.h"
#define NUM_ACTIONS 4 // Assuming 4 possible actions: up, down, left, right
#define ALPHA 0.1     // Learning rate
#define GAMMA 0.9     // Discount factor
#define EPSILON 0.1   // For ε-greedy strategy

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

int chooseAction(int row, int col, double qValues[row][col][NUM_ACTIONS]);

double*** createQValuesTable(int rows, int cols);

void freeQValuesTable(double*** qValues, int rows, int cols);

void updateQValue(double qValues[rows][cols][NUM_ACTIONS], int currentState[2], int action, int nextState[2], double reward, int done);

void stepEnvironment(Maze* maze, int currentState[2], int action, int nextState[2], double* reward, int* done);

void qLearning(Maze maze, double qValues[rows][cols][NUM_ACTIONS]);

