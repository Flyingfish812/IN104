/*  File description
*   In this project some global variables are predefined and will be used directly, they are:
*   --------------------------------------------------
*   char** mazeEnv;  // A two-dimensional array of characters that stores the layout of the maze.
*   int** visited;  // A two-dimensional array of integers that tracks whether each position in the maze has been visited.
*   int rows;
*   int cols;  // Indicates the number of rows and columns in the maze.
*   int start_row;
*   int start_col;  // The row and column of the starting position in the maze.
*   int state_row;
*   int state_col;  // The row and column of the current state.
*   int goal_row;
*   int goal_col;  // The row and column of the goal position.
*   --------------------------------------------------
*   You may reference them in the file mazeEnv.h
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"
#include "dfs.h"

// Default function used to find the solution
void apply_dfs(){
    init_visited();
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    mazeEnv_render();
    dfs(start_row,start_col);
    add_crumbs();
    mazeEnv_render();
}

// Build the maze, using a customized structure
Maze build_maze(){
    Maze maze;
    Position start = {start_row, start_col};
    Position goal = {goal_row, goal_col};
    maze.mazeEnv = mazeEnv;
    maze.rows = rows;
    maze.cols = cols;
    maze.start = start;
    maze.goal = goal;
    return maze;
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Usage: %s <Learning method>\n", argv[0]);
        return 1;
    }
    // Read the maze file
    // Set the variables: mazeEnv, rows, cols, start_row, start_col, goal_row, goal_col
    mazeEnv_make("data/maze.txt");
    init_visited();
    clock_t start, end;
    
    // Seed the random number generator
    srand(time(NULL));

    // Load the maze from a file
    Maze maze = build_maze();

    // Initialize Q-values
    double*** qValues1 = createQValuesTable(maze.rows, maze.cols, maze);
    double*** qValues2;
    if(strcmp(argv[1], "basic") == 0){
        printf("Use basic Q-learning\n");
    }else if(strcmp(argv[1], "sarsa") == 0){
        printf("Use SARSA Q-learning\n");
    }else if(strcmp(argv[1], "double") == 0){
        printf("Use Double Q-learning\n");
        qValues2 = createQValuesTable(maze.rows, maze.cols, maze);
    }else{
        printf("Invalid learning method\n");
        return 1;
    }

    // Train the model using Q-learning
    int epochs = 1000;
    start = clock();
    int* log;
    if(strcmp(argv[1], "basic") == 0){
        log = qLearning(maze, qValues1, 1000);
    }else if(strcmp(argv[1], "sarsa") == 0){
        log = sarsaLearning(maze, qValues1, epochs);
    }else if(strcmp(argv[1], "double") == 0){
        log = doubleQLearning(maze, qValues1, qValues2, epochs);
    }
    end = clock();
    exportToFile(log, epochs, "Result.txt");
    printf("Time taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    // mazeEnv_render();

    // Use the trained Q-values to find and print the solution
    // Expected solution: start from (6,3) and end at (6,14)
    printf("Solution Path:\n");

    int currentState[2] = {maze.start.row, maze.start.col};
    int steps = 0;
    int failed = 0;

    while (maze.mazeEnv[currentState[0]][currentState[1]] != 'g' && steps < maze.rows * maze.cols / 2) {
        int action;
        if(strcmp(argv[1], "double") == 0){
            action = chooseDoubleAction(currentState[0], currentState[1], qValues1, qValues2, maze, 0);
        }
        else{
            action = chooseAction(currentState[0], currentState[1], qValues1, maze, 0);
        }
        printf("(%d, %d) -> ", currentState[0], currentState[1]);
        currentState[0] += directions[action][0];
        currentState[1] += directions[action][1];
        steps++;
        if(currentState[0] >= maze.rows || currentState[0] < 0 || currentState[1] >= maze.cols || currentState[1] < 0){
            failed = 1;
            break;
        }
    }

    printf("(%d, %d)\n", currentState[0], currentState[1]);
    printf("Finished solving in %d steps.\n", steps);

    if(failed){
        printf("Failed to find a solution.\n");
    }

    // Free the memory allocated for the Q-values
    freeQValuesTable(qValues1, rows, cols);
    if(strcmp(argv[1], "double") == 0){
        freeQValuesTable(qValues2, rows, cols);
    }
    
    return 0;
}