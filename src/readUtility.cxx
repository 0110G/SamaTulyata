#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <z3.h>
using namespace std;
extern SYMTAB  stab;

/* READ PLACES OF PRES+ MODEL*/


Z3_ast mk_var(Z3_context ctx, const char * name, Z3_sort ty)
{
    Z3_symbol   s  = Z3_mk_string_symbol(ctx, name);
    return Z3_mk_const(ctx, s, ty);
}

void exitf(const char* message)
{
  fprintf(stderr,"BUG: %s.\n", message);
  exit(1);
}

void error_handler(Z3_context c, Z3_error_code e)
{
    printf("Error code: %d\n", e);
    exitf("incorrect use of Z3");
}

Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err)
{
    Z3_context ctx;

    Z3_set_param_value(cfg, "model", "true");
    ctx = Z3_mk_context(cfg);
    Z3_set_error_handler(ctx, err);

    return ctx;
}


Z3_context mk_context()
{
    Z3_config  cfg;
    Z3_context ctx;
    cfg = Z3_mk_config();
    ctx = mk_context_custom(cfg, error_handler);
    Z3_del_config(cfg);
    return ctx;
}



Z3_ast Z3Node(Z3_context* ctxp, EXPR expr){
  EXPRNODE tmp = *expr ;
  if (tmp.type == 0){
    	Z3_ast temp[2];
    	temp[0]=Z3Node(ctxp,expr->left);
    	temp[1]=Z3Node(ctxp,expr->right);
	    switch(tmp.value){
		      case 1:
		      return Z3_mk_and(*(ctxp),2,temp);
		      break;
		      case 2:
		      return Z3_mk_gt(*(ctxp),temp[0],temp[1]);
		      break;
		      case 3:
		      return Z3_mk_lt(*(ctxp),temp[0],temp[1]);
		      break;
		      case 4:
		      return Z3_mk_or(*(ctxp),2,temp);
		      break;
		      case 5:
		      return Z3_mk_ge(*(ctxp),temp[0],temp[1]);
		      break;
		      case 6: //printf("<=");
		      return Z3_mk_le(*(ctxp),temp[0],temp[1]);
		      break;
		      case 7:
		      return Z3_mk_eq(*(ctxp),temp[0],temp[1]);
		      break;
		      case 8:
		      return Z3_mk_not(*(ctxp),Z3_mk_eq(*(ctxp),temp[0],temp[1]));
		      break;
		      case 9:
		      return Z3_mk_add(*(ctxp),2,temp);
		      break;
		      case 10: 
		      return Z3_mk_sub(*(ctxp),2,temp);
		      break;
		      case 11:
		      return Z3_mk_mul(*(ctxp),2,temp);
		      break;
		      case 12:
		      return Z3_mk_div(*(ctxp),temp[0],temp[1]);
		      break;
		      case 13:
		      break;
	     }
   }
   else if(tmp.type ==1){
   	Z3_sort ty = Z3_mk_int_sort(*ctxp);
     	char name[2];
     	name[0]=tmp.value;
     	name[1]='\0';
     	return mk_var(*ctxp, name, ty);
   }else{   
     	Z3_sort ty = Z3_mk_int_sort(*ctxp);
    	return Z3_mk_int(*ctxp, tmp.value, ty);
   }
}  


PRESPLUS readplace (PRESPLUS model){
  int no_of_places, no_of_variables;
  int place_index, trans_index, var_index;

  printf("\n Enter the number of place : ");
  scanf("%d", &no_of_places);
  model.no_of_places = no_of_places ;

  for(place_index = 0 , var_index = 0 ; place_index < no_of_places; 
		  place_index++)
  { 
    printf("\n Enter the %dth place name : ",place_index);
    scanf("%s", model.places[place_index].name) ;
    int nameIdx ;
    char name[4];
    // char* name = (char*)malloc(sizeof(char));
    printf("\n Enter the name of the variable associated with that place : ");
    getchar();  // dummy read to consume the line feed give earlier in stdin
    scanf("%s",name) ;
    
   /* search for its index in the "model.var_table" */
   int here;
   here = searchForVariable (model, name);
	printf("Till Here\n");
   if (here == -1)
   {	   
	printf("Till HereX\n");
     /*** enter this variable name in model.var_table[var_index]
      *  then put "var_index++"  in model.places[place_index].var_index
      *  ***/
     strcpy(name,model.var_table[var_index].name);
     model.places[place_index].var_index = var_index; 
     var_index++;
    }
    else{ /*** put "here" in model.places[place_index].var_index ***/
	printf("Till HereY\n");           
	model.places[place_index].var_index = here;
    }
	printf("Till HereOK\n");
  }

  /* Update the number of total variables */
  model.no_of_variables = var_index ; 
  return model ;
}

/* Added Function: Bhavya Saraf*/
void PrintExpr(EXPR ex){
	if(ex == NULL){
		return;
	}
	int x = 0;
	std::queue<std::pair<EXPR, std::pair<int, int> > >que;
	que.push({ex, {x, -1}});
	while(!que.empty()){
		EXPR cur = que.front().first;
		int id = que.front().second.first;
		int parId = que.front().second.second;
		printf("Type: %d, Value: %d ID: %d Parent ID: %d\n", cur->type, cur->value, id, parId);
		que.pop();		
		if(cur->left != NULL){
			que.push({cur->left, {x+1, id}});
			x++;
		}
		if(cur->right != NULL){
			que.push({cur->right, {x+1, id}});
			x++;
		}
	}		
}


/* READ TRANSITIONS OF PRES+ MODEL*/

PRESPLUS readtransition(PRESPLUS model)
{ 
	printf("Till Here2\n");
   int idx ;
   char* c;
   printf("\n Enter the number of transitions : ");
   scanf("%d",&model.no_of_transitions);
	
	printf("Till Here3\n");

   for (idx = 0; idx < model.no_of_transitions; idx++)
   {
      char* name = (char*)malloc(4*sizeof(char));
      printf("\n Enter the %dth transition name : ",idx);
      scanf("%s",name) ;
       
      /* search for its index in the "model.transitions" 
       * The part is redundant, though I have tried to
       * use model.transitions[idx].name
       * bu here is some eror
       */ 
      if(searchForTransition(model, name) >= 0)
      {
		idx-- ;
                model.no_of_transitions-- ;
		continue ;
      }

     for(int i=0; i< 4; i++)
     {
       model.transitions[idx].name[i]=name[i];
     }         
      char* expression = (char*)malloc(50*sizeof(char));
      printf("\n Enter the expression associated with transition : ");
      scanf("%s",expression);
	
	// Added Line
	printf("Expr Scanned! Bhavya Edit %s\n", expression);

      model.transitions[idx].expr = ParseExpression(expression);

	PrintExpr(model.transitions[idx].expr);



      parseassignments(expression,model.transitions[idx].action);

      char* guardCondition = (char*)malloc(50*sizeof(char)) ;
      printf("\n Enter the guard condition ( if guard conditions are not present type n_g)........: ");
      scanf("%s",guardCondition);
	  if(strcmp(guardCondition,"n_g")==0)
        strcpy(guardCondition,"1=1");
      model.transitions[idx].guard = ParseExpression(guardCondition);
	
	// Orignal Condition	
	//model.transitions[idx].condition = parsecondition(guardCondition,model.transitions[idx].condition);

	printf("OKAWD\n");  
	
	// Added by Bhavya Saraf
	model.ctx = mk_context();
	model.transitions[idx].z3Condition = Z3Node(&model.ctx, model.transitions[idx].guard);

      printf ("\n Enter the number of places in preset : ");
      scanf("%d",&(model.transitions[idx].no_of_preset));

      int presetIdx ;
      int placeIndex ;
      for (presetIdx = 0 ; presetIdx < model.transitions[idx].no_of_preset ; presetIdx++)
      {
	 char* name = (char*)malloc(4*sizeof(char)) ;
	 printf("\n Enter %dth preset name : ", presetIdx);
	 scanf("%s", name);
	/* search for its index in the "model.places" */

         placeIndex = searchForPlace(model,name) ;
         model.transitions[idx].preset[presetIdx] = placeIndex ; 
	 if(placeIndex == -1)
	 {
		/* Error Situation : Need to give an error,
                   though the -1 index is already inserted
                 */
	 }
       }
       printf ("\n Enter the number of places in postset : ");
       scanf("%d",&(model.transitions[idx].no_of_postset)) ;
       for (presetIdx  = 0; presetIdx < model.transitions[idx].no_of_postset; presetIdx++)
       {
         char* name = (char*)malloc(4*sizeof(char)) ;
         printf("\n Enter %dth postset name : ", presetIdx);         
         scanf("%s",name) ;
         /* search for its index in the "model.places" */

         placeIndex = searchForPlace(model,name) ;
	 model.transitions[idx].postset[presetIdx]  = placeIndex;
         if(placeIndex == -1)
	 {
		/* Error Situation : Need to give an error,
                   though the -1 index is already inserted
                 */
         }
	
       }
     }
    return model;
}

/* Read Initial marking*/

PRESPLUS readInitialMarking(PRESPLUS model)
{
    int no_of_marked_places ;
    printf("\n Reading the initial marking ...") ;
    printf("\n Enter the number of places containing marking : ") ;
    scanf("%d",&no_of_marked_places) ;
	
    model.no_of_places_initially_marked = no_of_marked_places ;
	
    int idx ;
    int initial_marking_index = 0 ;
    for(idx=0; idx< no_of_marked_places; idx++)
    {
        char* name = (char*)calloc(4,sizeof(char)) ;
        printf(" Enter the name of %dth marked place : ",idx) ;
        scanf("%s",name) ;

          /* search for its index in the "model.places" */

	  int placeIndex = searchForPlace(model, name) ;
           
	   if(placeIndex >= 0 )
	   {
		printf("In readInitialMarking: placeIndex=%d\n",placeIndex);	   	
		model.initial_marking[initial_marking_index++] = placeIndex ;
	   	model.places[placeIndex].token_present = 1 ;
           }
           else
           {
	  	/* Error situation */
           }
    }
    return model ;
}


PRESPLUS connectSetPlaces(PRESPLUS model)
{
	int i=0,prenum=0,postnum=0,j=0,k=0,val=0;
	for(i=0;i<model.no_of_places;i++)
	{
		model.places[i].no_of_preset=0;
		model.places[i].no_of_postset=0;
	}
	for(i=0;i<model.no_of_transitions;i++)
	{
		prenum=postnum=0;
		if((prenum=model.transitions[i].no_of_preset)!=0)
		{
			for(j=0;j<prenum;j++)
			{
				val=++model.places[model.transitions[i].preset[j]].no_of_postset;
				model.places[model.transitions[i].preset[j]].postset[val-1]=i;
			}
		}
		if((postnum=model.transitions[i].no_of_postset)!=0)
		{
			for(j=0;j<postnum;j++)
			{
				val=++model.places[model.transitions[i].postset[j]].no_of_preset;
				model.places[model.transitions[i].postset[j]].preset[val-1]=i;
			}
		}
	}
	return model;
}





