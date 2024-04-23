/*  File description
*   In this project some global variables are predefined and will be used directly, they are:
*   --------------------------------------------------
*   char** mazeEnv;  // The list that stores the maze
*   int** visited;
*   int rows;
*   int cols;
*   int start_row;
*   int start_col;
*   int state_row;
*   int state_col;
*   int goal_row;
*   int goal_col;
*   --------------------------------------------------
*   You may reference them in the file mazeEnv.h
*/

#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "dfs.h"
#include "functions.h"

void apply_dfs(){
    init_visited();
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    mazeEnv_render();
    dfs(start_row,start_col);
    add_crumbs();
    mazeEnv_render();
}

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

int main(){
    // Read the maze file
    // Set the variables: mazeEnv, rows, cols, start_row, start_col, goal_row, goal_col
    mazeEnv_make("data/maze.txt");
    init_visited();
    
    // Seed the random number generator
    srand(time(NULL));

    // Load the maze from a file
    Maze maze = build_maze();

    // Initialize Q-values
    double*** qValues = createQValuesTable(maze.rows, maze.cols, maze);

    // Train the model using Q-learning
    qLearning(maze, qValues, 1000);
    mazeEnv_render();

    // Use the trained Q-values to find and print the solution
    printf("Solution Path:\n");
    int currentState[2] = {maze.start.row, maze.start.col};
    int steps = 0;
    int failed = 0;
    while (maze.mazeEnv[currentState[0]][currentState[1]] != 'g' && steps < maze.rows * maze.cols / 2) {
        int action = chooseAction(currentState[0], currentState[1], qValues, maze, 0);
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
    if(failed){
        printf("Failed to find a solution.\n");
    }

    freeQValuesTable(qValues, 12, 20);

    return 0;
}