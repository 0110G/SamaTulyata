#ifndef FINDPATH_H_
#define FINDPATH_H_

typedef struct queue {
	/* Queue data structure to be used in tyraversal and flooding
	 */
	int head,tail;
	int* arr;
} Queue;

typedef struct prepost {
	/**
	 * Data structure used for backward traversal
	 * Holds a place and transition pair
	 * can be used by a transition and place both
	 */
        //int label;
	int p;
	int t;
} Prepost;

typedef struct dummy {
	int p;
	Prepost *pp;
	int num_preplace;

	/*data structure to hold the transitions in path finding routine*/
	//int** seq;
        char** myseq;
	char** no_seq;
	int size;
        int label;

	/*Reserved for future use*/
	int BFS_visit;
} dummy_place;

typedef struct dummy1 {
	int t;
	prepost *pp;
	int num_posttrans;
        int label;
	/*Reserved for future use*/
	int BFS_visit;
} dummy_trans;

#endif
