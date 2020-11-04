#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"
#include <z3.h>
SYMTAB  stab;

SYMTAB  stab1;

// Added By Bhavya Saraf
Z3_ast mk_var_f(Z3_context ctx, const char * name, Z3_sort ty)
{
    Z3_symbol   s  = Z3_mk_string_symbol(ctx, name);
    return Z3_mk_const(ctx, s, ty);
}

void exitf_f(const char* message)
{
  fprintf(stderr,"BUG: %s.\n", message);
  exit(1);
}

void error_handler_f(Z3_context c, Z3_error_code e)
{
    printf("Error code: %d\n", e);
    exitf_f("incorrect use of Z3");
}

Z3_context mk_context_custom_f(Z3_config cfg, Z3_error_handler err)
{
    Z3_context ctx;
    Z3_set_param_value(cfg, "model", "true");
    ctx = Z3_mk_context(cfg);
    Z3_set_error_handler(ctx, err);

    return ctx;
}


Z3_context mk_context_f()
{
    Z3_config  cfg;
    Z3_context ctx;
    cfg = Z3_mk_config();
    ctx = mk_context_custom_f(cfg, error_handler_f);
    Z3_del_config(cfg);
    return ctx;
}



Z3_ast Z3Node_f(Z3_context* ctxp, EXPR expr){
  EXPRNODE tmp = *expr ;
  if (tmp.type == 0){
    	Z3_ast temp[2];
    	temp[0]=Z3Node_f(ctxp,expr->left);
    	temp[1]=Z3Node_f(ctxp,expr->right);
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
     	return mk_var_f(*ctxp, name, ty);
   }else{   
     	Z3_sort ty = Z3_mk_int_sort(*ctxp);
    	return Z3_mk_int(*ctxp, tmp.value, ty);
   }
}


NC* parseclause(char *clause,int oprt,int nt,NC* root) {
//operator = 1 for &&, =0 for ||, -1 for last
//conditions;   not=1 for !cond. and =0 for cond.

    int flag;
    char op1[30],op2[30];
    int i=0,j=0,relop;
    flag=0;
    //printf("\n satyam    %s  %d  %d \n",clause,operator,not);
    while(clause[i]!='\0')
    {
	    while(!flag)
	    {
		     switch(clause[i])
		     {
			      case '!':
				     if(clause[i+1]=='='){
					     op1[j]='\0';
					     relop=5;
					     i++;
				     }
				     else 
					     printf(" INVALID RELATIONAL OPERATOR  \n");
			             i++;
				     flag=1;
				     break;

			     case '=':
				     if(clause[i+1]=='=')
				     {
					     op1[j]='\0';
					     relop=4;
					     i++;
				     }
				     else
					    printf(" INVALID RELATIONAL OPERATOR \n");
				     i++;
				     flag=1;
				     break;			

			    case '>':
				     if(clause[i+1]=='=')
			             {
					    relop=0;
					    i++;
				     }
				     else
					    relop=1;
				     i++;
				     op1[j]='\0';
				     flag=1;
				     break;			

			    case '<':
				     if(clause[i+1]=='=')
				     {
					    relop=2;
					    i++;
				     }
				     else
					    relop=3;
				     i++;
				     op1[j]='\0';
				     flag=1;
				     break;
			    case 0:
				     op1[j]='\0';
				     op2[0]='\0';
				     flag=2;
				     relop=-1;
	                 break;
		       default :			  
			         op1[j++]=clause[i++];
		     }
        }
	if(flag==1)
	{
	    j=0;
	    while(clause[i]!='\0')
		    op2[j++]=clause[i++];
	    op2[j]='\0';
	}        
    }   
    root=create_condexpr(op1,op2,relop,oprt,nt,root);
    return root;
}


int 
isfunction(char *s) {
	 int i=0;
	 while(s[i]!='\0'&&s[i]!='(')
		i++;
 	 if(s[i]!='\0')
		 return 1;
	 return 0;
}




int 
negateoperator(int op) {
 	switch(op)
 	{
		case 0:
			return 3;
		case 1: 
			return 2;
		case 2:
			return 1;
		case 3: 
			return 0;
		case 4: 
			return 5;
		case 5:
			return 4;
	}
}


int 
getfunname(char *s,char *name) {
	int i=0,count=0;
	while(s[i]!='\0'&&s[i]!='(')
	{
		name[i]=s[i];
		i++;
	}
	name[i]='\0';
	if(s[i]!='\0')
	{
		while(s[i]!='\0')
		{
			if(s[i]==',') count++;
			i++;
		}
		count++;
	}
	return count;
}



void 
getarguments(char *s,char *args[10]) {
    int i = 0, count=0,j;
    char name[20];
	while(s[i]!='\0'&&s[i]!='(')
	{
		i++;
	}
	if(s[i]!='\0')
	{      
        i++;
        j=0;
		while(s[i]!='\0')
		{                                                
			if(s[i]==','||s[i]==')')
            {
                name[j]='\0';
                args[count]=(char *)malloc(strlen(name)+1);
                strcpy(args[count],name); 
                count++;
                j=0;
            }
            else
                name[j++]=s[i];
			i++;
		}		
	}
}	



NC*  
create_condexpr(char *op1,char *op2,int relop,int logicalop,int nt,NC *root) {
    int i=0,numarg=0;
	char *args[10];
	char fun[20];
	int c1,c2,f=1,val,v1,v2;	
	NC *A,*temp,*S,*T,*P,*R,*O,*S1;
    	if(nt)
		relop=negateoperator(relop);
	O=(NC*) malloc(sizeof(NC));
	O->list=NULL;
	O->type='O';
	O->inc=0;	
    	//printf( "     root value is %ld\n",root);
	R=(NC*) malloc(sizeof(NC));
	R->list=NULL;
	R->type='R';
	R->inc=relop;
	O->link=R;
        if(root==NULL)
	{
		A=(NC *)malloc(sizeof(NC));
		A->type='A';
		A->list=NULL;
		A->inc=0;
		A->link=O;
	    root=A;
	}
	else
	{
        temp=root->link;
	    while(temp->list!=NULL)
		     temp=temp->list;
        temp->list=O;	
	}
	// printf( "     root value is %ld\n",root);
	if(op2[0]=='\0') // single variable or funcation with boolean status 
	{
        	f=isfunction(op1);
		if(f)
		{
             	     if(nt) R->inc=4;
		     else R->inc=5;
                     numarg=getfunname(op1,fun);
		     getarguments(op1,args);
		     S=(NC*)malloc(sizeof(NC));
		     T=(NC*)malloc(sizeof(NC));
		     S->list=NULL;
		     S->link=T;
		     S->type='S';
		     S->inc=0;
		     T->list=NULL;
		     T->type='T';
		     T->inc=1;
		     temp=NULL; 
		     P=(NC*)malloc(sizeof(NC));
		     P->list=NULL;
		     P->type='f';
		     P->inc=indexof_symtab(fun);
		     T->link=P;
		     for(i=0;i<numarg;i++)
		     {
			       S1=(NC*)malloc(sizeof(NC));
			       S1->type='S';
			       S1->inc=0;
			       S1->list=NULL;
			       S1->link=create_term(args[i],1);
			       if(temp!=NULL)
				       temp->list=S1;
			       else
			           P->link=S1;
			       temp=S1;
		     } 
		}
		else 
		{
		     S=(NC *)malloc(sizeof(NC));
		     S->list=NULL;
		     S->link=NULL;
		     if(isconstant(op1))
		     	S->inc=constval(op1); 
		     else
		     {
			S->inc=0;
			T=(NC*)malloc(sizeof(NC));
			P=(NC*)malloc(sizeof(NC));
			T->list=NULL;
			T->link=P;
			P->list=NULL;
			P->link=NULL;
			P->type='v';
			T->type='T';
			T->inc=1;
			T->inc=indexof_symtab(op1);
			S->link=T;	
		     }
			     
		     S->type='S';
		}
		R->link=S;
    	}
        else // op2 is not NULL
	{
	        c1=isconstant(op1);
		c2=isconstant(op2);
        	if(c1&&c2)  //both are constants 
                {
	       	        val=constval(op1)-constval(op2);
			S=(NC *)malloc(sizeof(NC));
			S->list=NULL;
			S->link=NULL;
			S->inc=val;
			S->type='S';         		
	   	}
                else if(c1||c2) //only one of the operands of the relational operators are constants
		{
			S=(NC*) malloc(sizeof(NC));
			S->list=NULL;
			S->type='S';
			if(c2)
			{
			      S->inc=-1*constval(op2);
			      S->link=create_term(op1,1);
			}
			else
			{
			      S->inc=constval(op1);
			      S->link=create_term(op2,-1);
			}

		}
		else // both the operands are either variables or functions
		{
	 		 S=(NC*)malloc(sizeof(NC));
	                 S->type='S';
		         S->list=NULL;
		 	 S->inc=0;
                         v1=indexof_symtab(op1);
	                 v2=indexof_symtab(op2);
			 if(v2 > v1)
			 {
				S->link=create_term(op1,1);
				if(S->link!=NULL) 
					S->link->list=create_term(op2,-1);
                         }
			else
			{
			        S->link=create_term(op2,-1);
				if(S->link!=NULL)
					 S->link->list=create_term(op1,1);
			}
		}
		R->link=S;
	}

    // write_lists(root); 		
	return root;		
}




NC* 
parsecondition(char *condition,NC *root) {

   int i=0,flag=0,j;
   char clause[30];
   root=NULL;
   if(!strcmp(condition,"n_g")) return NULL; 
   while(condition[i]!='\0')
   {  
		clause[0]='\0';
		if(condition[i]=='-') 
		{
			root=NULL;
			return NULL;
		}
		j=0;
		flag=0;
	  	if(condition[i]=='!')
		{
			i++;
			flag=1;
		}

		while(condition[i]!='\0'&&condition[i]!='&'&&condition[i]!='|')
			  clause[j++]=condition[i++];

		if(condition[i]!='\0')
		{      
			
			i++;
			if(condition[i]=='&'||condition[i]=='|')
		  	{
				clause[j]='\0';
				j=0;
				if(condition[i]=='&')
					root=parseclause(clause,1,flag,root); //flag=1 for !cond. and =0 for cond.
				else
					root=parseclause(clause,0,flag,root);
				i++;
			 	continue;	
			}
			else
	        	{
			    clause[j++]=condition[i-1];
			    while(condition[i]!='\0'&&(condition[i]!='&'||condition[i]!='|'))
			    	clause[j++]=condition[i++];
			    if(condition[i]!='\0')
		            {
					    i++;
						if(condition[i]=='&'||condition[i]=='|')
		  				{
							clause[j]='\0';
							j=0;
							if(condition[i]=='&')
						        root=parseclause(clause,1,flag,root);
							else
								root=parseclause(clause,0,flag,root);
							i++;
						}
						continue;	
		             }
			    else
			    {
				    clause[j]='\0';
				    j=0;
				    root=parseclause(clause,-1,flag,root);
			    }
			}		    
		}
		else
		{ 
		    clause[j]='\0';
			j=0;
			root=parseclause(clause,-1,flag,root);
		}
   }
   return root;
}






void parseassignments(char *s,DATA_TRANS *actions)
{
   int i=0,j=0,k=0;
   char A[200];
   A[0]='\0';
	while(s[i]!='\0')
	{
		if(s[i]==',')
		{
			A[j]='\0';
			j=0;
		    //	printf("%s\n",A);
			parseA(A,&actions[k]);
			k++;
		}
		else
		{  A[j]=s[i];
			j++;
		          i++;}
	}
    A[j]='\0';
	parseA(A,&actions[k]);
	actions[++k].rhs=NULL;
	//printf("%s\n",A);
}


/***********************************************************************************************/

/*   This function is used to parse the given three adress code statements                     */


/***********************************************************************************************/


void parseA(char *A,DATA_TRANS *D)
{
       
	char op1[20],op2[20],op3[20],op;
	int i=0,j=0,k=0;
	op1[0]='\0';op2[0]='\0';op3[0]='\0';
	op='\0';
	while(A[i]!='\0'&&A[i]!='=')
	{
		op1[j]=A[i];
		j++;
		i++;
	}
	op1[j]='\0';
	D->lhs=indexof_symtab(op1);

	if(!strcmp(op1,"-")) return;
	i++;
	if(A[i]!='\0'&&A[i]=='-')
	{
	//	if(A[i+1]!='0'&&A[i+1]>='0'&&A[i+1]<='9')
		op2[k++]=A[i++];
	}
	while(A[i]!='+'&&A[i]!='-'&&A[i]!='*'&&A[i]!='/'&&A[i]!='%'&&A[i]!='\0')
		op2[k++]=A[i++];
	op2[k]='\0';
	if(A[i]!='\0')
	{
		op=A[i];
		j=0;i++;
		while(A[i]!='\0')
		{
			op3[j]=A[i];
			j++;i++;
		}
		op3[j]='\0';
	}
	else
	{
    	op='\0';
	    op3[0]='\0';
	}
      // printf("\n\n      op2=%s  op= %c   op3= %s  \n",op2,op,op3);
	D->rhs=create_expr(op2,op,op3);
	//printf("rhs  ");
	//write_lists(D->rhs);        
}

/**********************************************************************************************/

/*     This function is used to create a copy of the list structure                           */


/**********************************************************************************************/


NC*  copylist(NC *source)
{
   NC *temp;
   if(source!=NULL)
   {
   		temp=(NC *)malloc(sizeof(NC));
		if(temp==NULL)
    		{      
	    		printf(" Memory allocation error\n");
			exit(0);
	    	}
   		temp->type=source->type;
   		temp->inc=source->inc;
   		temp->link=copylist(source->link);
   		temp->list=copylist(source->list);
        return temp;
   }
   return NULL;
}
/**********************************************************************************************/

/* This function is used to create the exression tree in normalized form                     */
/* It takes the three operands and the operator of the three address code as input           */

/**********************************************************************************************/

NC*  create_expr(char *op2,char op, char *op3)
{
   int c1,c2,v1,v2, i, j;
   NC *S,*T,*P,*S1,*S2;
   char temp[100], op2_temp[100];
   c1=isconstant(op2); // c1 = 1 indicates op2 is constant; = 0 otherwise
   c2=isconstant(op3);
   if(op3[0]=='\0')   // handling unary operators
   {
	S=(NC*) malloc(sizeof(NC));
        S->list=NULL;
        S->type='S';
	if(c1)
	{
	   S->link=NULL;
	   S->inc=constval(op2);
	}
	else    // handling simple assignments
	{ 
	  if(op2[0] == '-')
	  {
		i=1;
		j=0;
		while(op2[i] != '\0')
		{
			op2_temp[j++]=op2[i++];
		}
		op2_temp[j]='\0';
	  	S->link=create_term(op2_temp,-1);
	  }
	  else
		S->link=create_term(op2,1);
	  S->inc=0;
	}
   }
   else if(c1&&c2)  // handling constants in which both operands are constants
   {
	   int val;
	   switch(op)
	   {
		   case '+':
			   val=constval(op2)+constval(op3);
			   break;
		   case '-':
			   val=constval(op2)-constval(op3);
		           break;
		   case '*':
			   val=constval(op2)*constval(op3);
			   break;
		   case '/':
			   val=constval(op2)/constval(op3);
			   break;
		   case '%':
			   val=constval(op2)%constval(op3);
			   break;
	   }
	   S=(NC *)malloc(sizeof(NC));
	   S->list=NULL;
	   S->type='S';
	   S->link=NULL;
	   S->inc=val;
   }
   else if(c1||c2)  // handling single constant expressions
   {
	  S=(NC *)malloc(sizeof(NC));
	  S->list=NULL;
	  S->type='S';
	  switch(op)
	  {
		  case '+':
			    if(c2)
			    {
				     S->inc=constval(op3);
				     S->link=create_term(op2,1);
			    }
			    else 
			    {
				    S->inc=constval(op2);
				    S->link=create_term(op3,1);
			    }
			    break;
	         case '-':
			    
			    if(c2) 
			    {
				    S->inc=-1*constval(op3);
				    S->link=create_term(op2,1);
                	    }
			    else 
			    {
				    S->inc=constval(op2);
				    S->link=create_term(op3,-1);
			    }
			   break;
		 case '*':
			   if(c2)
			        S->link=create_term(op2,constval(op3));
			   else
			    	S->link=create_term(op3,constval(op2));
			   S->inc=0;
			   break;
		 case '/':
		 case '%':
			  T=(NC*)malloc(sizeof(NC));
			  P=(NC*)malloc(sizeof(NC));
			  T->type='T';
			  S->link=T;
			  T->link=P;
			  T->list=NULL;
			  P->list=NULL;
			  if(op=='/')
			      P->type='D';
			  else
			      P->type='M';
			  			 	  
			  S1=(NC*)malloc(sizeof(NC));
			  S2=(NC*)malloc(sizeof(NC));
			  S2->type='S';
			  S1->type='S';
			  S1->list=S2;
			  S2->list=NULL;
			  P->link=S1;
			  if(c2)
			  {
				  S2->inc=constval(op3);
				  S2->link=NULL;
				  S1->link=create_term(op2,1);
				  S1->inc=0;
			  }
			  else
			  {
				  S1->inc=constval(op2);
				  S1->link=NULL;
				  S2->link=create_term(op3,1);
				  S2->inc=0;
		          }		  
			  break;
	  }		  
   }
   else // handling expressions involving both variables
   {  
          S=(NC*)malloc(sizeof(NC));
	  S->type='S';
	  S->list=NULL;
	  S->inc=0;
          v1=indexof_symtab(op2);
	  v2=indexof_symtab(op3);
	  switch(op)
	  {
		  case '+':
		  case '-':
			if(v2 > v1)
			{
			    S->link=create_term(op2,1);
			    if(S->link!=NULL)
			    {
			           if(op=='-')  
				        S->link->list=create_term(op3,-1);    
			           else
				    	S->link->list=create_term(op3,1);
			    }
			}
			else
			{
				if(op=='-')
				        S->link=create_term(op3,-1);
				else
					S->link=create_term(op3,1);
			        if(S->link!=NULL)
				      S->link->list=create_term(op2,1);
			}
			break;
		  case 	'*':
		      
			S1=(NC*)malloc(sizeof(NC));
			S2=(NC*)malloc(sizeof(NC));
			S1->link=NULL;
			S2->link=NULL;
			S1->type='v';
			S2->type='v';
			if(v2>v1)
			{
				S1->inc=v1;
				S2->inc=v2;
			}
			else
			{
				S1->inc=v2;
				S2->inc=v1;
			}
			S1->list=S2;
			S2->list=NULL;
			T=(NC *)malloc(sizeof(NC));
			T->list=NULL;
			T->type='T';
			T->inc=1;
			T->link=S1;
			S->link=T;
			break;
		  case  '/':
		  case  '%':
		    S1=(NC *)malloc(sizeof(NC));
			S2=(NC*)malloc(sizeof(NC));
			S1->list=S2;
			S2->list=NULL;
			S1->type='S';
			S2->type='S';
			S1->inc=0;
			S2->inc=0;
			S1->link=create_term(op2,1);
			S2->link=create_term(op3,1);
			T=(NC*)malloc(sizeof(NC));
			P=(NC *)malloc(sizeof(NC));
			P->list=NULL;
			T->list=NULL;
			T->type='T';
			if(op=='/')
				P->type='D';
			else
				P->type='M';
			T->inc=1;
			P->inc=0;
			T->link=P;
			P->link=S1;
			S->link=T;
			break;
	  }
   }	   
   return S;
}	

/******************************************************************************************/

/*  This function is used to create a term for a particular normalized term               */
/*  It takes the symbolic constant and the integer value as input and returns the pointer */
/*  to the normalized binary tree                                                         */

/******************************************************************************************/


NC* create_term(char *op,int val)
{
	NC *T,*P;
	int i=indexof_symtab(op);  //get the index of the symbol
	if(i>=0)
	{
		T=(NC *)malloc(sizeof(NC));
		P=(NC*) malloc(sizeof(NC));
		T->link=P;
		P->list=NULL;
		T->list=NULL;
		P->link=NULL;
		P->inc=i;
		P->type='v';
		T->type='T';
		T->inc=val;
		return T;
	}
	else 
		return NULL;
}

/****************************************************************************************/

/*    This function is used to find the integer value represented by a string           */

/****************************************************************************************/



int
constval( char *s )
{
  int val = 0, i;

  if( s[0] != '-' ){
    for( i = 0; s[i] != '\0'; i++ )
      val = val * 10 + s[i] - 48;  
  }       // calculating constant value
  else {
    for( i = 1; s[i] != '\0'; i++ )
      val = val * 10 + s[i] - 48; 
    val = -1 * val;
  }
  

  return val;
}
 



/*****************************************************************************************/

/* This function is used to check whether the given string is an integer constant or not */
/* It returns 1 if it is a constant otherwise it will return 0                           */

/*****************************************************************************************/

int isconstant(char *s)
{
   int i=0;
   if(s[0]=='\0')
	   return 0;   
   if(s[0]=='-'||s[0]=='+')  //check for the sign of the constant
   {  
	   i=1;
	   while(s[i]!='\0') 
	   {
		   if(s[i]>='0'&&s[i]<='9') i++;
		   else return 0;
	   }
	   if(i==strlen(s)) return 1;  
   }
   else  // this handles unsigned numbers
   {
	   
   	  while(s[i]!='\0')
   	  {
		  if(s[i]>='0'&&s[i]<='9') i++;
		  else return 0;
   	  }
   	 if(i==strlen(s)) return 1;
   }
}

/*********************************************************************************/

/*   This function is used to find the index value of a symbol in a symbol table */
/*   If the symbol is already in the table then it will return the index  value  */
/*   otherwise it will add the symbol into the symbol table                      */
/*   if the string is null then it will return -1                                */

/*********************************************************************************/
 
int 
indexof_symtab( char *symbol )       
{
   int i;
   if( strcmp( symbol, "\0" ) )  // if the symbol is null string then return -1
     {

      	
       for( i = 0; i < stab.numsymbols; i++ )
	 if( !strcmp( stab.sym_tab[i], symbol ) ){

 
	    return stab.val_tab[i];
         }
       
   i = stab.numsymbols++; // symbol not found in the symbol table
   stab.sym_tab[i] = ( char * ) malloc( strlen( symbol ) +1 );
   strcpy( stab.sym_tab[i], symbol );//copy the symbol into the symbol table
   stab.val_tab[i] = constval( symbol );    
   return stab.val_tab[i];
     }
   return -1;
}

/*************************************************************************/


void
symbol_for_index( int n , char *sym_value ){
  int i;
  for( i = 0; i < stab.numsymbols; i++ )
    if( stab.val_tab[i] == n ){
      strcpy( sym_value, stab.sym_tab[i] );
    }
}




PRESPLUS readFile (FILE *fp)
{ 
  PRESPLUS model;
/*  FILE *fp;
  char name[100];
  printf("..........Enter file name :.........\n");
  scanf("%s", name);
  fp = fopen(name,"r"); 
*/  
  int i,j;
  
 
 // fprintf(fp,"**********Printing PRES+ on file******************** ");
  fprintf(fp,"\n \n");
  fprintf(fp,"\n Number of Transitions = %d", model.no_of_transitions);
  fscanf(fp,"\n Number of Places = %d", &model.no_of_places);
  //printf("\n Number of Places : %d",model.no_of_places);/* test case*/
  //printf("\n Printing Places ... \n");/* test case*/
  int var_index = 0;
  for(i=0;i< model.no_of_places;i++,var_index++)
  {
    fscanf(fp,"\n Place : %s ",model.places[i].name);
    //printf("\n Place name : %s", model.places[i].name);/* test case*/
    fscanf(fp,"\n Associated variable : %s",model.var_table[var_index].name);
    model.places[i].var_index = var_index ;
    //printf("\n Associated Variables : %c", model.var_table[model.places[i].var_index].name);/* test case*/
    fscanf(fp,"\n\n") ;
    //printf("\n\n");/* test case*/
  }


  int placeIDX ;
  
  fscanf(fp,"\n Number of Transitions = %d", &model.no_of_transitions);
  //printf("\n Number of Transitions : %d", model.no_of_transitions);/* test case*/
  

  for(i=0;i<model.no_of_transitions; i++)
  {
       fscanf(fp,"\n Transition : %s",model.transitions[i].name);
       //printf("\n Transition Name : %s",model.transitions[i].name);/* test case*/
       char* expression = (char*)malloc(50*sizeof(char));
       fscanf(fp,"\n  Transition function :%s",expression);
      model.transitions[i].expr = ParseExpression(expression);


       parseassignments(expression,model.transitions[i].action);//change
      // printf("\n Enter the expression associated with transition :%s",expression);/* test case*/
       char* guardCondition = (char*)malloc(50*sizeof(char)) ;
      
	   fscanf(fp,"\n Guard conditions :%s",guardCondition);
	   
	   // Added by Bhavya Saraf
	   if(strcmp(guardCondition,"n_g")==0){
        	strcpy(guardCondition,"1=1");
	   }

       model.transitions[i].guard = ParseExpression(guardCondition);
      
	  //printf("\n Enter the guard condition associated with transition :%s",guardCondition);/* test case*/
       
	   model.transitions[i].condition=parsecondition(guardCondition,model.transitions[i].condition);
      
		model.ctx = mk_context_f();
		model.transitions[i].z3Condition = Z3Node_f(&model.ctx, model.transitions[i].guard);

	printf("Reached Till Here\n");

      fscanf(fp, "\n Printing Preset list(%d) :",&model.transitions[i].no_of_preset);
      //printf("\n Preset list (%d) ",model.transitions[i].no_of_preset);/* test case*/
        
    for (j=0; j < model.transitions[i].no_of_preset; j++)
    {
         char* name = (char*) malloc(4*sizeof(char)) ;
         fscanf(fp,"%s ", name);
        // printf("%s", name);/* test case*/
         placeIDX = searchForPlace(model,name);
         if( placeIDX >= 0 )
         {
		 model.transitions[i].preset[j] =  placeIDX;
		 placeIDX++ ;
	 }  
        // printf("%s", model.places[model.transitions[i].preset[j]].name);/* test case*/ 
   }

    fprintf(fp,"\n Printing postset list : ") ;/* test case*/
    fscanf(fp, "\n Printing Postset list(%d) : ",&model.transitions[i].no_of_postset);
    //printf("\n Post set List (%d) ",model.transitions[i].no_of_postset);/* test case*/
    for (j=0 ; j< model.transitions[i].no_of_postset;  j++ )
    {
         char* name = (char*)malloc(4*sizeof(char)) ;
         fscanf(fp,"%s", name); 
         //printf("%s", name); /* test case*/
         /* search for its index in the "model.places" */

         placeIDX = searchForPlace(model,name) ;
	       model.transitions[i].postset[j]  = placeIDX;
         if(placeIDX == -1)
	 {
		/* Error Situation : Need to give an error,
                   though the -1 index is already inserted
                 */
         }
    fscanf(fp,"\n\n") ;
    //printf("\n\n"); /* test case*/
   }
  }
  fscanf(fp,"\n Printing initial marking ... ");
  //printf(" ");/* test case*/
  fscanf(fp,"\n Total number of initially marked places : %d",&model.no_of_places_initially_marked);
  //printf("\n Total No of Initial Marking: %d",model.no_of_places_initially_marked);/* test case*/
  fscanf(fp,"\n Initially marked places are : ");
  //printf("\n Initial marked places are : ");/* test case*/
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
      char* name = (char*)malloc(4*sizeof(char)) ;
      fscanf(fp,"%s",name) ;
     // printf(" %s",name); /* test case*/

          /* search for its index in the "model.places" */

	  int placeIDX = searchForPlace(model, name) ;
           
	   if(placeIDX >= 0 )
	   {
	   	model.initial_marking[i] = placeIDX ;
	   	model.places[placeIDX].token_present = 1 ;
           }
           else
           {
	  	/* Error situation */
           }
  }
  fscanf(fp,"\n\n") ;
  //printf("\n\n");/* test case*/
  //fprintf(fp,"******************Printing PRES+ on file is compleated*****************************");

//printpresplus(model);/* test case*/
//fclose(fp);
 model = connectSetPlaces (model) ;
return model;
}

