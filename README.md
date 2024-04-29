# Reinforcement Learning Project : Maze

## Introduction

Here's our repository dedicated to Reinforcement Learning. A small project will be conducted and used as a course project at ENSTA Paris.

We aim here to implement different Reinforcement Learning algorithms in order to exit a maze first

You'll find here several folders :

/Include is for the headers

/src is for the algorithms and the environnements by themselves.

## Goal for the project

The goal of the project is to implement Reinforcement Learning algorithms in order to exit a maze first.

### Environment

The environment is a 2D grid stored in an txt file, with a start position and an exit position.

The agent is a 2D grid cell. It can move in 4 directions (up, down, left, right). It needs to find the goal position from the learning.

### Actions

The agent can move in 4 directions (up, down, left, right), these actions will be stored in an array.

### Rewards

- A reward of 0 if it stays in the same position, or it steps on the known position

- A reward of 1 if it explores a new position.

- A very large reward of 10000 if it reaches the goal position.

## Algorithm applied

In this project, we will apply the Q-learning algorithm to solve the maze.

### Q-learning

Q-learning is a Reinforcement Learning algorithm that is based on the Bellman equation. It is a model-free algorithm, which means that it does not require a model of the environment. It is an off-policy algorithm, which means that it does not require the agent to follow a specific policy.

The actions chosen depend on the Q-value table. During the training session, the Q-values will be updated by the following function:

$$
Q(s,a) = Q(s,a) + \alpha [R(s,a) + \gamma \max_{a'} Q(s',a') - Q(s,a)]
$$

where:

- $s$ is the current state

- $a$ is the action taken

- $R(s,a)$ is the reward received for taking action $a$ in state $s$

- $\alpha$ is the learning rate, which determines how much the Q-value should be updated

### Useful parameters

- $\alpha$: learning rate, which determines how much the Q-value should be updated

- $\gamma$: discount factor, which determines the importance of future rewards

- $\epsilon$: exploration rate, which determines the probability of choosing a random action

In order to encourage the agent to explore the environment, we set the $\epsilon$ very big at first (around 0.9). During the training episode, it will decrease to 0 by using the function $\epsilon_i = \epsilon \times (1 - \frac{i}{E_{\text{tot}}})$, where $E_{\text{tot}}$ is the total number of episodes (We set it to 1000 in this project).

### Functions defined

- `int chooseAction(int row, int col, double*** qValues, Maze maze, double epsilon);` is the function used to decide the next action.

- `double*** createQValuesTable(int rows, int cols, Maze maze);` is the function used to create the Q-values table.

- `void freeQValuesTable(double*** qValues, int rows, int cols);` is the function used to free the Q-values table.

- `void updateQValue(double*** qValues, int currentState[2], int action, int nextState[2], double reward, int done);` is the function used to update the Q-value table.

- `void stepEnvironment(Maze maze, int** visited, int currentState[2], int action, int nextState[2], double* reward, int* done);` is the function used to take a step in the environment.

- `void qLearning(Maze maze, double*** qValues, int EPS);` is the function used to train the agent.

### Training

In each episode, the agent will take actions and receive rewards. The Q-values will be updated using the formula given. The training will stop when the agent reaches the goal position, or reached a maximum number of steps (In this project, we set the maximum number of steps to 10000).

### Result

The agent can successfully reach the goal after training.

## Using of the code

A makefile is included in this project. In order to compile the code, you need to run the command "make" in the terminal (In Linux environment).

Then you need to run the command "./main" in the terminal. To clear the intermediate files, you need to run the command "make clean" in the terminal.