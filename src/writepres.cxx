#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"
 void  DecompilingEXPRNodefile(FILE *fp, EXPR expr)
{

 
  
  EXPRNODE tmp = *expr ;
  if (tmp.type == 0)
  {
    switch(tmp.value)
    {
      case 1: fprintf(fp,"&");
      break;
      case 2: fprintf(fp,">");
      break;
      case 3: fprintf(fp,"<");
      break;
      case 4: fprintf(fp,"|");
      break;
      case 5: fprintf(fp,">=");
      break;
      case 6: fprintf(fp,"<=");
      break;
      case 7: fprintf(fp,"=");
      break;
      case 8: fprintf(fp,"!=");
      break;
      case 9: fprintf(fp,"+");
      break;
      case 10: fprintf(fp,"-");	
      break;
      case 11: fprintf(fp,"*"); 
      break;
      case 12: fprintf(fp,"/"); 
      break;
      case 13: fprintf(fp,"_");
      break; 
    }
   }
   else if(tmp.type ==1)
   {
     fprintf(fp,"%c",tmp.value);
   }
   else if(tmp.type == 2)
  {   
     fprintf(fp,"%d",tmp.value);
  }
  return ;
}  
	
void DecompilingExprfile(FILE *fp,EXPR expr)
{
  

  if (expr->left != NULL)
  {
    DecompilingExprfile(fp, expr->left);
  }

  DecompilingEXPRNodefile(fp,expr);

  if(expr->right!= NULL)
  {
    DecompilingExprfile(fp, expr->right);
  }
  return ;
} 	 	 
int writeFile (PRESPLUS model)
{ 
 FILE *fp;
  char *sym_value;
        sym_value = (char * ) malloc( 100*sizeof( char ) );
 char name[100];
 printf(".....Enter file name.......... :\n");
 scanf("%s", name);
 //char prename[200]="./examples/presplus/";
 //strcat(prename,name);
 //fp = fopen(prename,"w");
 fp = fopen(name, "w");
 int i,j,k;
 // VARIABLE temp1;
  //ffprintf(fp,"**********Printing PRES+ on file******************** ");
  //ffprintf(fp,"\n \n");
 
  fprintf(fp,"\n Number of Places = %d", model.no_of_places);

//  fprintf("\n Printing Places ... \n");
  for(i=0;i< model.no_of_places;i++)
  {
    fprintf(fp,"\n Place : %s ",model.places[i].name);
    fprintf(fp,"\n Associated variable : %s",model.var_table[model.places[i].var_index].name);
    //fprintf(fp,"\n Associated token value:%d",model.places[i].token_present);
    fprintf(fp,"\n\n") ;
  }

  // fprintf("Number of Variables = %d\n", model.no_of_variables);
  fprintf(fp,"\n Number of Transitions = %d", model.no_of_transitions); 
 
 // ffprintf(fp,"\n Printing Transitions ... \n");
  for(i=0;i<model.no_of_transitions;i++)
  {
    fprintf(fp,"\n Transition : %s",model.transitions[i].name);
    fprintf(fp,"\n Transition function :");
    DecompilingExprfile(fp,model.transitions[i].expr);
    printf("\n Normalized Transition function :");
     k=0;
     while(model.transitions[i].action[k].rhs!=NULL)
	{
         	symbol_for_index( model.transitions[i].action[k].lhs, sym_value );
		//printf( "* %s( ", sym_value );
		printf("\n%s  :=  ", sym_value );
		
		write_lists(model.transitions[i].action[k].rhs);
		k++;
	}
    fprintf(fp,"\n Guard conditions :");
    DecompilingExprfile(fp,model.transitions[i].guard); 
    printf("\n Normalized Guard conditions:");
     write_lists(model.transitions[i].condition);
    //Decompilingexpr(model.transitions[i].expr);
    //fprintf(fp,"\n Transition function: %s",model.transitions[i].expr);  
   // ffprintf(fp,"\n Printing preset list : ") ;
    fprintf(fp, "\n Printing Preset list(%d) :",model.transitions[i].no_of_preset);
    for (j=0; j < model.transitions[i].no_of_preset; j++)
    {
         fprintf(fp,"%s ", model.places[model.transitions[i].preset[j]].name);
    }

    //ffprintf(fp,"\n Printing postset list : ") ;
    fprintf(fp, "\n Printing Postset list(%d) : ",model.transitions[i].no_of_postset);
    for (j=0 ; j< model.transitions[i].no_of_postset;j++)
    {
        fprintf(fp,"%s ", model.places[model.transitions[i].postset[j]].name);
    }
    fprintf(fp,"\n\n") ;

  }

  fprintf(fp,"\n Printing initial marking ... ");
  fprintf(fp,"\n Total number of initially marked places : %d",model.no_of_places_initially_marked);
  fprintf(fp,"\n Initially marked places are : ");
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
     fprintf(fp,"%s ",(model.places[model.initial_marking[i]].name));
  }
  fprintf(fp,"\n\n") ;
  //ffprintf(fp,"******************Printing PRES+ on file is compleated*****************************");
fclose(fp);
return 0;
}

