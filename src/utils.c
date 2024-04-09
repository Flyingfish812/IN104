#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Function to choose an action using ε-greedy strategy
int chooseAction(int row, int col, double qValues[row][col][NUM_ACTIONS]) {
    // Explore with a probability of EPSILON
    if ((double)rand() / RAND_MAX < EPSILON) {
        return rand() % NUM_ACTIONS;
    }

    // Otherwise, exploit (choose the best action)
    int bestAction = 0;
    double maxQValue = qValues[row][col][0];
    for (int a = 1; a < NUM_ACTIONS; ++a) {
        if (qValues[row][col][a] > maxQValue) {
            maxQValue = qValues[row][col][a];
            bestAction = a;
        }
    }
    return bestAction;
}

// Function to dynamically allocate and initialize the Q-value table
double*** createQValuesTable(int rows, int cols) {
    // Allocate memory for the table of pointers to 2D arrays
    double*** qValues = (double***)malloc(rows * sizeof(double**));
    for (int i = 0; i < rows; ++i) {
        qValues[i] = (double**)malloc(cols * sizeof(double*));
        for (int j = 0; j < cols; ++j) {
            qValues[i][j] = (double*)malloc(NUM_ACTIONS * sizeof(double));
            // Initialize Q-values to 0.0 for all actions
            for (int a = 0; a < NUM_ACTIONS; ++a) {
                qValues[i][j][a] = 0.0;
            }
        }
    }
    return qValues;
}

// Function to free the dynamically allocated Q-value table
void freeQValuesTable(double*** qValues, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            free(qValues[i][j]);
        }
        free(qValues[i]);
    }
    free(qValues);
}

// Function to update the Q-value
void updateQValue(double qValues[rows][cols][NUM_ACTIONS], int currentState[2], int action, int nextState[2], double reward, int done) {
    double maxQNextState = qValues[nextState[0]][nextState[1]][0];
    for (int a = 1; a < NUM_ACTIONS; ++a) {
        if (qValues[nextState[0]][nextState[1]][a] > maxQNextState) {
            maxQNextState = qValues[nextState[0]][nextState[1]][a];
        }
    }
    double qValueCurrent = qValues[currentState[0]][currentState[1]][action];
    double qValueUpdate = qValueCurrent + ALPHA * (reward + GAMMA * maxQNextState * (1 - done) - qValueCurrent);
    qValues[currentState[0]][currentState[1]][action] = qValueUpdate;
}

// Update the agent's state based on the action and calculate the reward
void stepEnvironment(Maze* maze, int currentState[2], int action, int nextState[2], double* reward, int* done) {
    int newRow = currentState[0] + directions[action][0];
    int newCol = currentState[1] + directions[action][1];

    // Ensure the new position is within the maze bounds and not a wall
    if (newRow >= 0 && newRow < maze->rows && newCol >= 0 && newCol < maze->cols && maze->mazeEnv[newRow][newCol] != '+') {
        nextState[0] = newRow;
        nextState[1] = newCol;
    } else {
        // If the move is invalid (hits a wall or out of bounds), stay in the current position
        nextState[0] = currentState[0];
        nextState[1] = currentState[1];
    }

    // Check if the goal is reached
    if (maze->mazeEnv[nextState[0]][nextState[1]] == 'g') {
        *reward = 100; // Assign a high reward for reaching the goal
        *done = 1;     // Indicate the episode is finished
    } else {
        *reward = -1; // Assign a small penalty for each move to encourage efficiency
        *done = 0;
    }
}


// Main Q-learning function
void qLearning(Maze maze, double qValues[rows][cols][NUM_ACTIONS]) {
    srand(time(NULL)); // For random number generation

    // Main loop for Q-learning (simplified for illustration)
    for (int episode = 0; episode < 1000; ++episode) {
        int currentState[2] = {maze.start.row, maze.start.col};
        int done = 0;
        
        while (!done) {
            int action = chooseAction(currentState[0], currentState[1], qValues);
            int nextState[2];
            double reward;
            stepEnvironment(&maze, currentState, action, nextState, &reward, &done);
            updateQValue(qValues, currentState, action, nextState, reward, done);
            currentState[0] = nextState[0];
            currentState[1] = nextState[1];
        }
    }

    // After learning, use qValues for decision making or further processing
}

// Assume all the required structures and functions are already defined above

