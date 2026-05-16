#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#define MAX_DISKS 9

// ====== WRITE YOUR COMPULSORY (BUT SPECIFIC TO THE PROBLEM) DATA TYPES =======

enum ACTIONS // All possible actions for Tower of Hanoi
{
    Move_A_B, Move_A_C,
    Move_B_A, Move_B_C,
    Move_C_A, Move_C_B
};

typedef struct State
{
    int num_disks;
    int disk[MAX_DISKS]; // disk[i] = peg index of disk i (0 = A, 1 = B, 2 = C)
    float h_n;          // Heuristic estimate to goal
} State;

// ================== YOU DO NOT NEED TO CHANGE THIS PART ======================

enum METHODS
{
    BreastFirstSearch = 1,   UniformCostSearch = 2,        DepthFirstSearch = 3,
    DepthLimitedSearch = 4,   IterativeDeepeningSearch = 5, GreedySearch = 6,
    AStarSearch = 7, GeneralizedAStarSearch = 8
};

// This struct is used to determine a new state and action in transition model
typedef struct Transition_Model
{
    State new_state;
    float step_cost;
}Transition_Model;

typedef struct Node
{
    State state;
    float path_cost;
    enum ACTIONS action; //The action applied to the parent to generate this node
    struct Node *parent;
    int Number_of_Child; // required for depth-first search algorithms
}Node;

typedef struct Queue  // Used for frontier
{
    Node *node;
    struct Queue *next;
}Queue;



#endif
