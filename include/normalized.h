typedef struct normalized_cell NC;


struct normalized_cell
{
   NC *list;
   char type;
   int inc;
   NC *link;
};


typedef NC N_sum, N_term, N_primary;

NC* copylist(NC *source);
int compare_tree(NC *t1, NC *t2);
void write_lists(NC *T);

/************************/

N_sum *simplify_sum( N_sum *sum, N_sum *result); 
N_sum *simplify_sum_1( N_sum *sum, N_sum *result, int min );
int Compare_Sums( N_sum *sum1, N_sum *sum2 );
int Check_c1_c2_And_R1_R2( int c1, int c2, int R1, int R2 );
int A_implies_B( NC *condition_A, NC *condition_B );
int Compare_Conditions( NC *condition1, NC *condition2 );
int Search_cond_in_expr( NC *expression, NC *condition );
NC *Delete_cond_from_expr( NC *expression, NC *condition, int count, NC *result );
NC *Remove_mult_occurence_cond_in_expr( NC *expression, NC *condition, NC *result );
NC *Remove_all_mult_occurence_in_expr_1( NC *expression1, NC *expression2, NC *result );
NC *Remove_all_mult_occurence_in_expr( NC *expression, NC *result );
NC *Substitute_in_condition( NC *condition, N_primary *p, N_sum *z, NC *result );
N_sum *SubExpr_simplify_sum( N_sum * sum, N_sum *result );
N_sum *SubExpr_simplify_terms( N_sum *sum, int term, N_sum *result );
NC *SubExpr_simplify_condition( NC *condition, NC *result );

/****************/

N_sum * Add_Sums( N_sum *, N_sum *, N_sum * );
N_sum * Add_Sums_1( N_sum *, N_sum *, N_sum * );
int Compare_Primaries( N_primary *, N_primary * );
int Compare_Terms( N_term *, N_term * );
//void symbol_for_index( int , char * );
N_term * Mult_Term_With_Term_1( N_term *, N_term *, N_term * );
N_term * Mult_Term_With_Term( N_term *, N_term *, N_term * );
N_sum * Mult_Sum_With_Term_1( N_sum *, N_term *, N_sum * );
N_sum * Mult_Sum_With_Term( N_sum *, N_term *, N_sum * );
N_sum * Mult_Sum_With_Sum( N_sum *, N_sum *, N_sum * );
N_sum * Mult_Sum_With_Sum_1( N_sum *, N_sum *, N_sum * );
N_sum * Mult_sum_with_constant( N_sum *, int, N_sum * );
N_sum * Mult_sum_with_constant_1( N_sum *, int, N_sum * );
N_term * Mult_constant_with_term( int , N_term *, N_term *);
N_term * Mult_primary_with_term( N_primary *, N_term *, N_term *);
N_term * Mult_primary_with_term_1( N_primary *, N_term *, N_term *);
N_sum *Replace_Primary_1( N_term *, N_primary *, N_sum *, N_term *, N_term *, N_sum *, N_sum * );
N_sum *Replace_Primary( N_term *, N_primary *, N_sum *, N_term *, N_term *, N_sum *, N_sum * );
N_sum *Substitute_In_Sum( N_sum*, N_primary *, N_sum *, N_sum * );
N_sum *Substitute_In_Sum_1( N_sum*, N_primary *, N_sum *, N_sum * );

/**************************************************************/



