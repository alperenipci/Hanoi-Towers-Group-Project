/* 
    These functions are compulsory for search algorithms but they are specific
    to problems. More clearly, you must update their blocks but do not change
    their input and output parameters.

    Also, you can add new functions at the end of file by declaring them in GRAPH_SEARCH.h
*/

#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>

static const char* peg_name(int index)
{
    switch(index){
        case 0: return "A";
        case 1: return "B";
        case 2: return "C";
        default: return "?";
    }
}

//______________________________________________________________________________
State* Create_State()
{
    static int disk_count = 0;
    static int call_count = 0;
    State *state = (State*)malloc(sizeof(State));
    if(state == NULL)
        Warning_Memory_Allocation();

    call_count++;
    if(call_count == 1){
        do{
            printf("Enter the number of disks (3, 5 or 9): ");
            scanf("%d", &disk_count);
        } while(disk_count != 3 && disk_count != 5 && disk_count != 9);

        state->num_disks = disk_count;
        for(int i = 0; i < state->num_disks; i++)
            state->disk[i] = 0; // all disks start on peg A
    }
    else{
        state->num_disks = disk_count;
        for(int i = 0; i < state->num_disks; i++)
            state->disk[i] = 2; // goal is all disks on peg C
    }

    state->h_n = 0.0f;
    return state;
}

//______________________________________________________________________________
void Print_State(const State *const tower_state)
{
    for(int peg = 0; peg < PEG_NUMBER; peg++){
        printf("Peg %s:[", peg_name(peg));
        int first = 1;
        for(int disk = tower_state->num_disks - 1; disk >= 0; disk--){
            if(tower_state->disk[disk] == peg){
                if(!first) printf(",");
                printf("%d", disk + 1);
                first = 0;
            }
        }
        printf("] ");
    }
}

//______________________________________________________________________________
void Print_Action(const enum ACTIONS action)
{
    switch(action){
        case Move_A_B: printf("Move_A_B"); break;
        case Move_A_C: printf("Move_A_C"); break;
        case Move_B_A: printf("Move_B_A"); break;
        case Move_B_C: printf("Move_B_C"); break;
        case Move_C_A: printf("Move_C_A"); break;
        case Move_C_B: printf("Move_C_B"); break;
        default:       printf("Move_Unknown"); break;
    }
}

//______________________________________________________________________________
static int top_disk_on_peg(const State *const state, int peg)
{
    for(int disk = 0; disk < state->num_disks; disk++){
        if(state->disk[disk] == peg)
            return disk;
    }
    return -1;
}

//______________________________________________________________________________
static int action_source_target(const enum ACTIONS action, int *source, int *target)
{
    switch(action){
        case Move_A_B: *source = 0; *target = 1; return 1;
        case Move_A_C: *source = 0; *target = 2; return 1;
        case Move_B_A: *source = 1; *target = 0; return 1;
        case Move_B_C: *source = 1; *target = 2; return 1;
        case Move_C_A: *source = 2; *target = 0; return 1;
        case Move_C_B: *source = 2; *target = 1; return 1;
        default: return 0;
    }
}

//______________________________________________________________________________
int Result(const State *const parent_state, const enum ACTIONS action, Transition_Model *const trans_model)
{
    int source, target;
    if(!action_source_target(action, &source, &target))
        return FALSE;

    int source_top = top_disk_on_peg(parent_state, source);
    if(source_top == -1)
        return FALSE; // no disk to move

    int target_top = top_disk_on_peg(parent_state, target);
    if(target_top != -1 && source_top > target_top)
        return FALSE; // cannot place larger disk on smaller one

    State new_state = *parent_state;
    new_state.disk[source_top] = target;
    trans_model->new_state = new_state;
    trans_model->step_cost = 1.0f;
    return TRUE;
}

//______________________________________________________________________________
float Compute_Heuristic_Function(const State *const state, const State *const goal)
{
    int misplaced = 0;
    for(int i = 0; i < state->num_disks; i++){
        if(state->disk[i] != goal->disk[i])
            misplaced += (1 << i);
    }
    return (float)misplaced;
}

//_______________ Update if your goal state is not determined initially ___________________________________
int Goal_Test(const State *const state, const State *const goal_state)
{
    if(PREDETERMINED_GOAL_STATE)
        return Compare_States(state, goal_state);
    else
        return 1;
}

// ==================== WRITE YOUR OPTIONAL FUNCTIONS (IF REQUIRED) ==========================
