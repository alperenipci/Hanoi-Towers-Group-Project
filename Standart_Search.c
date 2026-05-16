/* 
    These functions are standard for graph search algorithms and you do not need to
    change them for different search problems.
    
    However, if you insert the generalized A* algorithm, 
	        you should update First_InsertFrontier_Search_TREE()
			              and Insert_Priority_Queue_GENERALIZED_A_Star(),  which is similar to Insert_Priority_Queue_A_Star()    
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include "hashTable.h"

//______________________________________________________________________________
Node* First_InsertFrontier_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state, float alpha)
{
    int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root)  
	int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)
	enum ACTIONS action;
    Node *node, *child, *temp_node; 
    Queue *frontier;
    Hash_Table *explorer_set;
	
	// a priority queue ordered by PATH-COST(or evaluation function f), with node as the only element		  
    frontier = Start_Frontier(root);  
	Print_Frontier(frontier); 	
    
    explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier); 
        
		// GOAL-TEST
		Number_Searched_Nodes++;
		if(Goal_Test(&(node->state), goal_state) ){
			printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
			printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
			printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
			Delete_Hash_Table(explorer_set);
			return node;
		}
		
		ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
			                                
        for(action=0; action<ACTION_COUNT; action++)
        {
            child = Child_Node(node, action);

            if(child!=NULL){
                Number_Generated_Nodes++;
                Number_Allocated_Nodes++;
				if(ht_search(explorer_set, &(child->state))) 
					continue; // child.STATE is in explored set 
					
				temp_node = Frontier_search(frontier, &(child->state));	
					
				switch(method){
                    case UniformCostSearch:
					if(temp_node!=NULL){
						if(child->path_cost < temp_node->path_cost) // child.STATE has been in frontier with higher cost
							Remove_Node_From_Frontier(temp_node, &frontier);
						else // child.STATE has already been in frontier with lower cost 
							continue; 		
					}
                        Insert_Priority_Queue_UniformSearch(child, &frontier); break;
                    case AStarSearch:
					if(temp_node!=NULL){
						if(child->path_cost + child->state.h_n < temp_node->path_cost + temp_node->state.h_n) // child.STATE has been in frontier with higher cost
							Remove_Node_From_Frontier(temp_node, &frontier);
						else // child.STATE has already been in frontier with lower cost 
							continue;
					}
                        child->state.h_n = Compute_Heuristic_Function(&(child->state), goal_state);
                        Insert_Priority_Queue_A_Star(child, &frontier); break; 
				case GeneralizedAStarSearch:
					// Generalized A* not implemented yet here
					break;
                    default:
                        printf("ERROR: Unknown method in First_InsertFrontier_Search_TREE.\n");
					Delete_Hash_Table(explorer_set);  
                        exit(-1);      	   
                }
				Print_Frontier(frontier); 	
			}	            
		}
    }
    
    printf("Maximum number of searched nodes is exceeded. %d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}

//______________________________________________________________________________
Node* First_GoalTest_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state)
{
    int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root)  
	int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)
	enum ACTIONS action;
    Node *node, *child; 
    Queue *frontier;
    Hash_Table *explorer_set; 
	
	// GOAL-TEST
	Number_Searched_Nodes++;
    if(Goal_Test(&(root->state), goal_state) ){
        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
		printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
		printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
        return root;
    }
	
	frontier = Start_Frontier(root);
	Print_Frontier(frontier);
	
	explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	  
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier);
         
		ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
                                        
        for(action=0; action<ACTION_COUNT; action++)
		{
			child = Child_Node(node, action);
			
			if(child!=NULL){
				Number_Generated_Nodes++;
				Number_Allocated_Nodes++;
						
				if(ht_search(explorer_set, &(child->state)) || Frontier_search(frontier, &(child->state))!=NULL)
					continue; // child.STATE is in explored set or frontier		
	
            	// GOAL-TEST 	
				Number_Searched_Nodes++;
			    if(Goal_Test(&(child->state), goal_state) ){
			        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
			        printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
			        printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
			        Delete_Hash_Table(explorer_set);
			        if(method==GreedySearch && !PREDETERMINED_GOAL_STATE)
			            child->state.h_n = Compute_Heuristic_Function(&(child->state), goal_state);
			        return child;
			    }

				switch(method){
                    case BreastFirstSearch:      
                        Insert_FIFO(child, &frontier); break;   
                    case GreedySearch:
                        child->state.h_n = Compute_Heuristic_Function(&(child->state), goal_state);
                        Insert_Priority_Queue_GreedySearch(child, &frontier); break;   
                    default:
                        printf("ERROR: Unknown method in First_GoalTest_Search_TREE.\n");  
                        exit(-1);                  
                }
                Print_Frontier(frontier);		   
            } 	            
		}
    } 
    
    printf("Maximum number of searched nodes is exceeded. %d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}

//______________________________________________________________________________
Node* DepthType_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state, const int Max_Level)
{
    static int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	static int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root) 
	static int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)   
	enum ACTIONS action;
    Node *node, *child; 
    Queue *frontier;
    Hash_Table *explorer_set; 
	
	// GOAL-TEST
	Number_Searched_Nodes++;
    if(Goal_Test(&(root->state), goal_state) ){
        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
		printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
		printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
        return root;
    }
	
	frontier = Start_Frontier(root);
	Print_Frontier(frontier);
	
	explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	    
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier);
        
        ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
		
		
		if(method==DepthLimitedSearch || method==IterativeDeepeningSearch)
			if(Level_of_Node(node)==Max_Level){
				Clear_All_Branch(node, &Number_Allocated_Nodes);
				continue; 
			}
                
                                        
        for(action=0; action<ACTION_COUNT; action++)
		{
			child = Child_Node(node, action);
			
			if(child!=NULL){
			    Number_Generated_Nodes++;
			    Number_Allocated_Nodes++;
				
				if(ht_search(explorer_set, &(child->state)) || Frontier_search(frontier, &(child->state))!=NULL){
					Clear_Single_Branch(child, &Number_Allocated_Nodes); // if child.STATE is in explored set or frontier
				} 				
				else{
					// GOAL-TEST
					Number_Searched_Nodes++;
				    if(Goal_Test(&(child->state), goal_state) ){
				        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
				        printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
				        printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
				        Delete_Hash_Table(explorer_set);
				        return child;
				    }

				    Insert_LIFO(child, &frontier);
				    Print_Frontier(frontier);
				}						   
            }

            if(action==ACTION_COUNT-1 && node->Number_of_Child==0) // If node has not child, clear it
                Clear_All_Branch(node, &Number_Allocated_Nodes);                
        }
    }
    
    printf("%d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}


//______________________________________________________________________________
Node* Child_Node(Node *const parent, const enum ACTIONS action)
{
      Node *child = NULL;
      Transition_Model trans_model;
      if( Result(&(parent->state), action, &trans_model) ){
          child = (Node*)malloc(sizeof(Node));
          if(child==NULL)
               Warning_Memory_Allocation(); 
           
          child->state     = trans_model.new_state;
          child->path_cost = parent->path_cost + trans_model.step_cost;
          child->parent    = parent;
          child->action    = action;
          child->Number_of_Child = 0;
          child->parent->Number_of_Child++;
      } 
      return child;
}

//______________________________________________________________________________
Queue* Start_Frontier(Node *const root)
{
    Queue *frontier = (Queue*)malloc(sizeof(Queue));
	    
	if(frontier==NULL)
    	Warning_Memory_Allocation(); 
    	
    frontier->node  = root;      
    frontier->next  = NULL;
	 
    return frontier; 
}    
    
//______________________________________________________________________________
int Empty(const Queue *const frontier)
{  
   return (frontier==NULL);
}

//______________________________________________________________________________
Node* Pop(Queue **frontier)
{
    Node *node = NULL;
    Queue *temp_queue; 
     
	if(!Empty(*frontier)){
	 	 node = (*frontier)->node;
        temp_queue = *frontier; 	 
 	 	*frontier = (*frontier)->next;
 	 	free(temp_queue);
	} 
 	 
	printf("\nPOP: ");
    Print_Node(node);
    printf("\n");

    return node; 
}

//... (file long, truncated for brevity)