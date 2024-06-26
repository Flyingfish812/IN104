#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// This file provides the necessary computational tools and functions to perform actions, learn from them, update policies, and manage the maze environment effectively.

// Actions: 0-up, 1-right, 2-down, 3-left
int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Function to choose an action using ��-greedy strategy 
int chooseAction(int row, int col, double*** qValues, Maze maze, double epsilon) {
    int availableActions[NUM_ACTIONS] = {-1};
    int numAvailableActions = 0;

    // Find the available actions
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        int nextRow = row + directions[a][0];
        int nextCol = col + directions[a][1];
        if(nextRow >= maze.rows || nextRow < 0 || nextCol >= maze.cols || nextCol < 0){
            continue;
        }
        if (maze.mazeEnv[nextRow][nextCol] != '+') {
            availableActions[numAvailableActions] = a;
            numAvailableActions++;
        }
    }

    // If available
    if (numAvailableActions > 0) {
        // Choose a random action with probability EPSILON
        if(numAvailableActions == 1){
            return availableActions[0];
        }
        if ((double)rand() / RAND_MAX < epsilon) {
            int randomIndex = rand() % numAvailableActions;
            return availableActions[randomIndex];
        }

        // Otherwise, choose the action with the highest Q-value
        int bestAction = availableActions[0];
        double maxQValue = qValues[row][col][bestAction];
        for (int i = 1; i < numAvailableActions; ++i) {
            int action = availableActions[i];
            if (qValues[row][col][action] > maxQValue) {
                maxQValue = qValues[row][col][action];
                bestAction = action;
            }
        }
        return bestAction;
    }

    return -1; // No available actions
}

int chooseDoubleAction(int row, int col, double*** qValues1, double*** qValues2, Maze maze, double epsilon) {
    int availableActions[NUM_ACTIONS] = {-1};
    int numAvailableActions = 0;

    // Find the available actions
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        int nextRow = row + directions[a][0];
        int nextCol = col + directions[a][1];
        if(nextRow >= maze.rows || nextRow < 0 || nextCol >= maze.cols || nextCol < 0){
            continue;
        }
        if (maze.mazeEnv[nextRow][nextCol] != '+') {
            availableActions[numAvailableActions] = a;
            numAvailableActions++;
        }
    }

    // If available
    if (numAvailableActions > 0) {
        // Choose a random action with probability EPSILON
        if(numAvailableActions == 1){
            return availableActions[0];
        }
        if ((double)rand() / RAND_MAX < epsilon) {
            int randomIndex = rand() % numAvailableActions;
            return availableActions[randomIndex];
        }

        // Otherwise, choose the action with the highest Q-value
        int bestAction = availableActions[0];
        double maxQValue = qValues1[row][col][bestAction] + qValues2[row][col][bestAction];
        for (int i = 1; i < numAvailableActions; ++i) {
            int action = availableActions[i];
            if (qValues1[row][col][action] + qValues2[row][col][action] > maxQValue) {
                maxQValue = qValues1[row][col][action] + qValues2[row][col][action];
                bestAction = action;
            }
        }
        return bestAction;
    }

    return -1; // No available actions
}

// Function to dynamically allocate and initialize the Q-value table
double*** createQValuesTable(int rows, int cols, Maze maze) {
    // Allocate memory for the table of pointers to 2D arrays
    double*** qValues = (double***)malloc(rows * sizeof(double**));
    for (int i = 0; i < rows; i++) {
        qValues[i] = (double**)malloc(cols * sizeof(double*));
        for (int j = 0; j < cols; j++) {
            qValues[i][j] = (double*)malloc(NUM_ACTIONS * sizeof(double));
            for (int k = 0; k < NUM_ACTIONS; k++) {
                qValues[i][j][k] = 0.0;
            }
        }
    }
    return qValues;
}

// Function to free the dynamically allocated Q-value table
void freeQValuesTable(double*** qValues, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            free(qValues[i][j]);
        }
        free(qValues[i]);
    }
    free(qValues);
}

// Function to update the Q-value
void updateQValue(double*** qValues, int currentState[2], int action, int nextState[2], double reward) {
    int currentRow = currentState[0];
    int currentCol = currentState[1];
    double maxQNextState = qValues[nextState[0]][nextState[1]][0];
    for (int a = 1; a < NUM_ACTIONS; ++a) {
        if (qValues[nextState[0]][nextState[1]][a] > maxQNextState) {
            maxQNextState = qValues[nextState[0]][nextState[1]][a];
        }
    }
    double qValueCurrent = qValues[currentRow][currentCol][action];
    double qValueUpdate = qValueCurrent + ALPHA * (reward + GAMMA * maxQNextState - qValueCurrent);
    qValues[currentRow][currentCol][action] = qValueUpdate;
}

void updateQValueSARSA(double*** qValues, int currentState[2], int action, int nextState[2], double reward, int nextAction) {
    int currentRow = currentState[0];
    int currentCol = currentState[1];
    int nextRow = nextState[0];
    int nextCol = nextState[1];

    double qValueCurrent = qValues[currentRow][currentCol][action];
    double qValueNext = qValues[nextRow][nextCol][nextAction];

    // SARSA update formula
    double qValueUpdate = qValueCurrent + ALPHA * (reward + GAMMA * qValueNext - qValueCurrent);
    qValues[currentRow][currentCol][action] = qValueUpdate;
}

void updateDoubleQValue(double*** qValues1, double*** qValues2, int currentState[2], int action, int nextState[2], double reward) {
    int currentRow = currentState[0];
    int currentCol = currentState[1];
    double maxQNextState1 = qValues1[nextState[0]][nextState[1]][0];
    for (int a = 1; a < NUM_ACTIONS; ++a) {
        if (qValues1[nextState[0]][nextState[1]][a] > maxQNextState1) {
            maxQNextState1 = qValues1[nextState[0]][nextState[1]][a];
        }
    }
    double maxQNextState2 = qValues2[nextState[0]][nextState[1]][0];
    for (int a = 1; a < NUM_ACTIONS; ++a) {
        if (qValues2[nextState[0]][nextState[1]][a] > maxQNextState2) {
            maxQNextState2 = qValues2[nextState[0]][nextState[1]][a];
        }
    }
    double qValueCurrent1 = qValues1[currentRow][currentCol][action];
    double qValueCurrent2 = qValues2[currentRow][currentCol][action];
    int updateNum = rand() % 2;
    if(updateNum == 0){
        // Double DQN update formula
        double qValueUpdate = qValueCurrent1 + ALPHA * (reward + GAMMA * maxQNextState2 - qValueCurrent1);
        qValues1[currentRow][currentCol][action] = qValueUpdate;
    }
    else{
        // Double DQN update formula
        double qValueUpdate = qValueCurrent2 + ALPHA * (reward + GAMMA * maxQNextState1 - qValueCurrent2);
        qValues2[currentRow][currentCol][action] = qValueUpdate;
    }
}

// Update the agent's state based on the action and calculate the reward
void stepEnvironment(Maze maze, int** visited, int currentState[2], int action, int nextState[2], double* reward, int* done) {
    int moved = 0;

    // Force the agent to move
    while(!moved){
        int newRow = currentState[0] + directions[action][0];
        int newCol = currentState[1] + directions[action][1];
        int explore = 0;

        // Ensure the new position is within the maze bounds and not a wall
        if (newRow >= 0 && newRow < maze.rows && newCol >= 0 && newCol < maze.cols && maze.mazeEnv[newRow][newCol] != '+') {
            nextState[0] = newRow;
            nextState[1] = newCol;
            if(visited[newRow][newCol] == unknown){
                visited[newRow][newCol] = known;
                explore = 1;
            }
            moved = 1;
        } else {
            // If the move is invalid (hits a wall or out of bounds), stay in the current position
            nextState[0] = currentState[0];
            nextState[1] = currentState[1];
            continue;
        }

        // Check if the goal is reached
        if (maze.mazeEnv[nextState[0]][nextState[1]] == 'g') {
            *reward = 10000; // Assign a high reward for reaching the goal
            *done = 1;     // Indicate the episode is finished
        } else if (explore){
            *reward = 1;   // If explored, give an additional reward
            *done = 0;
        } else {
            *reward = 0;
            *done = 0;
        }
    
    }
    
}

// Main Q-learning function
int* qLearning(Maze maze, double*** qValues, int EPS) {
    int* log = (int*)malloc(sizeof(int) * EPS);
    srand(time(NULL)); // For random number generation
    int done, step, action;
    double reward;
    int nextState[2];

    // Main loop for Q-learning (simplified for illustration)
    for (int episode = 0; episode < EPS; ++episode) {
        int currentState[2] = {start_row, start_col};
        double epsilon = EPSILON * (1 - (double)episode / EPS); // Decrease epsilon as episodes progress
        done = 0;
        step = 0;
        
        while (!done && step < 10000) {
            reward = 0.0;
            nextState[0] = currentState[0];
            nextState[1] = currentState[1];
            action = chooseAction(currentState[0], currentState[1], qValues, maze, epsilon);
            stepEnvironment(maze, visited, currentState, action, nextState, &reward, &done);
            updateQValue(qValues, currentState, action, nextState, reward);
            currentState[0] = nextState[0];
            currentState[1] = nextState[1];
            // printf("(%d, %d) -> ", currentState[0], currentState[1]);
            step++;
        }
        log[episode] = step;
    }

    return log;
}

// SARSA Q-learning function
int* sarsaLearning(Maze maze, double*** qValues, int EPS) {
    int* log = (int*)malloc(sizeof(int) * EPS);
    srand(time(NULL)); // For random number generation
    int done, step, action, nextAction;
    double reward;
    int nextState[2];

    // Main loop for Q-learning (simplified for illustration)
    for (int episode = 0; episode < EPS; ++episode) {
        int currentState[2] = {start_row, start_col};
        double epsilon = EPSILON * (1 - (double)episode / EPS); // Decrease epsilon as episodes progress
        done = 0;
        step = 0;
        
        while (!done && step < 10000) {
            reward = 0.0;
            nextState[0] = currentState[0];
            nextState[1] = currentState[1];
            action = chooseAction(currentState[0], currentState[1], qValues, maze, epsilon);
            stepEnvironment(maze, visited, currentState, action, nextState, &reward, &done);
            nextAction = chooseAction(nextState[0], nextState[1], qValues, maze, epsilon);
            updateQValueSARSA(qValues, currentState, action, nextState, reward, nextAction);
            currentState[0] = nextState[0];
            currentState[1] = nextState[1];
            // printf("(%d, %d) -> ", currentState[0], currentState[1]);
            step++;
        }
        log[episode] = step;
    }

    return log;
}

int* doubleQLearning(Maze maze, double*** qValues1, double*** qValues2, int EPS) {
    int* log = (int*)malloc(sizeof(int) * EPS);
    srand(time(NULL)); // For random number generation
    int done, step, action, nextAction;
    double reward;
    int nextState[2];

    // Main loop for Q-learning (simplified for illustration)
    for (int episode = 0; episode < EPS; ++episode) {
        int currentState[2] = {start_row, start_col};
        double epsilon = EPSILON * (1 - (double)episode / EPS); // Decrease epsilon as episodes progress
        done = 0;
        step = 0;
        
        while (!done && step < 10000) {
            reward = 0.0;
            nextState[0] = currentState[0];
            nextState[1] = currentState[1];
            action = chooseDoubleAction(currentState[0], currentState[1], qValues1, qValues2, maze, epsilon);
            stepEnvironment(maze, visited, currentState, action, nextState, &reward, &done);
            updateDoubleQValue(qValues1, qValues2, currentState, action, nextState, reward);
            currentState[0] = nextState[0];
            currentState[1] = nextState[1];
            // printf("(%d, %d) -> ", currentState[0], currentState[1]);
            step++;
        }
        log[episode] = step;
    }

    return log;
}

void exportToFile(int* list, int size, const char* filename) {
    FILE* file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL) {
        printf("Failed to open file");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", list[i]); // Write each integer to the file with a newline
    }

    fclose(file); // Close the file
}