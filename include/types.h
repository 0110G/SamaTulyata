#ifndef TYPES_H
#define TYPES_H

#define MAXNOVAR 100       /* max. no. of variables */
#define MAXNOPLACES 100     /* max no. of places */
#define MAXNOTRANS 100

#define MAXNOPOSTSET 10
#define MAXNOPRESET  10

#define MAXINITIALMARKING 10 
#define MAXSTATES 1000
#define STACKSIZE 1000
#define SYMTABSIZE 10000
#define MAXOP 200
#define QUEUESIZE 2000

#include "normalized.h"
#include <z3.h>
typedef char shortInt;
/*
typedef enum 
typedef enum OPERATORTYPE {LOGICALOP, RELOP, ARITHOP};
typedef enum NODETYPE {operator, variable, constant};
*/
typedef struct Assign
{
        int lhs;    /* left hand side (lhs) variable's symbol table index is stored here  */
        NC *rhs;  /* right hand side of the assignment is stored as an expression tree */
}DATA_TRANS;

typedef enum {
	LITERAL = 0 ,
	BINARY
} EXPRTYPE ;

typedef struct variable
  {
    char name[4]; 
    char expr[100];
    int  type ; /* int, float, bool, etc. -- only int now */
  } VARIABLE ;

/* VARIABLE var_table[MAXNOVAR] ;*/

typedef struct place 
 {
  char name[4]; /* "P1", "P2", etc. for ex. */
  /* int token_present ; not needed -- dynamic */
  int var_index ; /* index to variable table */
  int token_present; /* Valid values --
 /                   * 0 - if token is not present, i.e not marked 
                       * 1 - if token is present , i.e. marked 
                       */

    int label;
    int lcut;
    int copy;
    int cut;
    int mark;
    int no_of_preset;
    int no_of_postset ;	
    int preset[MAXNOPRESET]; 
    int postset[MAXNOPRESET]; 
    //NC* trans;
    DATA_TRANS trans[100];
 } PLACE ;


/******************************************************
 *            For the expessions
 *            The following will finally be replaced by 
 *            normalized expression data str. which will
 *            be imported from Chandan Karfa
 ******************************************************/
typedef struct exprNode
 {  int type; /* 0 -- operator, 1 -- variable, 2 -- constant  */
    int value; /* interpretation depends on "type" as follows
                      == 0 (operator)  --> 0: &, 1 -- |, 2 -- ~
		    *                       3: >, 4 -- >=, 5 -- <, 6 -- <=
		    *                       7: =, 8 -- !=
		    *                       9: +, 10 -- - (binary)
		    *                       */
   struct exprNode *left, *right; 
                  /** expressions are stored as their parse trees, which in
		   * turn would be a biary tree
		   * For the unary operators like ~ (neg) and - (unary)
		   * "right" is don't care  **/     
 } EXPRNODE;	 

typedef EXPRNODE *EXPR;




typedef struct transition
 {
  int id;
  int label;
  int mark;
  char name[4];   /* for ex. "T13", "T09" */
  int no_of_preset;
  int no_of_postset ;
  int preset[MAXNOPRESET];  /* indices to place table */
  int postset[MAXNOPOSTSET]; /* indices to place table */
  EXPR guard;
  EXPR expr;
  //int vald;
  NC *condition;
  DATA_TRANS action[100];
  int delay, deadline,cmplt;
  
  // Added
  Z3_ast z3Condition;
 } TRANSITION;

typedef struct
 { 
   int no_of_places ;
   int no_of_transitions ;
   int no_of_variables ;
   PLACE places[MAXNOPLACES];
   TRANSITION transitions[MAXNOTRANS];
   VARIABLE var_table[MAXNOVAR];
   Z3_context ctx;
   /** 
    * This array contains the index of the places 
    * which contains variables i.e places which are
    * initially marked, hence contructs the initial 
    * marking list for the pres plus model 
    *
    * NOTE : We do not store the places, as they 
    * are alrady present in the place table.
    */
   int no_of_places_initially_marked ;
   int initial_marking[MAXINITIALMARKING];
 } PRESPLUS;	 

typedef PLACE MARKING[10];


/*    This structure is used to hold the information of an assignment operation */


typedef struct sym_table
{
	char *sym_tab[SYMTABSIZE];
	int val_tab[SYMTABSIZE];
	int numsymbols;
}SYMTAB;

typedef struct node_list
{
	NC *node;
	struct node_list *next;
}NCL;

extern int indexof_symtab(char *s);
extern void symbol_for_index( int , char * );

extern int isconstant(char *s);
extern int constval(char *s);

extern NC* create_expr(char *, char ,char *);

extern NC* create_term(char *op,int val);

extern NC* parsecondition(char *s,NC *root);
extern void parseassignments(char *s,DATA_TRANS *action);
extern void parseA(char *s,DATA_TRANS *action);

extern NC*  parseclause(char *clause,int op,int flag,NC *root);
extern NC* create_condexpr(char *op1,char *op2,int relop,int logicalop,int flag,NC *root);
extern void  write_lists( NC *root );
extern int isfunction(char *s);

/* Cutpoint data structure contained here */

typedef struct cutpoint {
	int num_of_cp;
	int cplist[MAXNOPLACES];
	int cptype[MAXNOPLACES];
} CUTPOINT;

typedef struct ralpha_node
{
	DATA_TRANS  Assign;
	struct ralpha_node *next;
}r_alpha;

typedef struct path {
	
 int *segments;
      
  int size;
  int *label;
  NC *condition;
  r_alpha *transformations;
}PATH;

typedef struct path1 {
  int num_of_path;
  PATH* path;
} PATHSET;
typedef PATHSET SET_OF_PATHS;

typedef struct pm_at_hand// This holds places marking at hand
{
 int num_of_marking;
 int mp[MAXNOPLACES];

}MARK_H;

  
typedef int *SET_OF_TRANS;

typedef struct 
{
  int size;  
  SET_OF_TRANS t_seq;
} CONCURRENT_TR_SET;
typedef struct 
{
  int num_seq;
  CONCURRENT_TR_SET* tseq;
}STACK_CONCURRENT_TRANS;
typedef STACK_CONCURRENT_TRANS SET_TRANS;

typedef struct CST
{
	int s0;
	int s1;
	struct CST *next;
}CPQ;

/* Structure for corresponding state -***/
typedef struct CPState
{
    int s0;
    int s1;
}CPS;

/* It is a queue of corresponding state -***/
typedef struct CSQ
{
    int front;
    int end;
    CPS cstate[QUEUESIZE];
}CPS_Q;
/*  This holds the complete infomation of a state in the FSMD    */
/*
typedef struct state_struct
{
    char  state_id[100];  /*  this is the name of the state  */
   
//    int   node_type; // 0: start state; 1: a state with no outward transition 2: any other state
     
    /*  this is the type of the state,it may be start state or
     *  final state or any cutpoint or any other state          */
   
//    int   numtrans; /*this holds the number of transitions of that state */
   
//        TRANSITION_ST  *translist;
    /* this is the list of all transitions from this state  */
   
//    struct state_struct *next;
//    /* this is the pointer to the next state by name in the FSMD  */
   
//}NODE_ST;  /* This is the structure for a state */

  /*  NODE_ST is the name of the structure holding the state information   */

/*
typedef struct fsmd_struct
{
    char name[150]; //name of the FSMD
   
    int  numstates; //no. of states in the FSMD

    NODE_ST  states[MAXSTATES]; //Descriptions of the states
   
}FSMD;

typedef struct FSM
{
	int places[MAXNOPLACES][MAXNOPLACES];
	int transitions[MAXNOPLACES-1][MAXNOTRANS];
	int states[MAXNOPLACES];
	int stateNum;
	int transNum;
	int relNum;
	int relFSM[MAXNOPLACES][3];
}interFSM;
*/

#endif
