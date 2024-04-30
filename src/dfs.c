#include "dfs.h"

// Deep first search algorithm to verify the solution
<<<<<<< HEAD

=======
>>>>>>> 56d79b6a46d45548f191233a9f0c4367e038c46d
int dfs(int row, int col){
    int* current = &visited[row][col];

    if(*current == goal){
        return 1;
    }else if(*current == unknown){
        *current = known;

        if (dfs(row, col-1)){
            *current = crumb;
            return 1;
        } else if (dfs(row+1, col)){
            *current = crumb;
            return 1;
        }  else if (dfs(row, col+1)){
            *current = crumb;
            return 1;
        }  else if (dfs(row-1, col)){
            *current = crumb;
            return 1;
        }

    }

    return 0;
<<<<<<< HEAD
}
=======
}


// int main(){
//    mazeEnv_make("../data/maze.txt");
//    init_visited();

//    printf("%d, %d \n", rows, cols);
//    printf("number of actions :  %d \n", number_actions); 
//    mazeEnv_render();
//    dfs(start_row,start_col);
//    add_crumbs();
//    mazeEnv_render();
//    return 0;
// }

>>>>>>> 56d79b6a46d45548f191233a9f0c4367e038c46d
