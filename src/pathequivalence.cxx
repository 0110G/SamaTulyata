#include "Utility.h"
#include "findpath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<fcntl.h>
#include<time.h>
#include <stdio.h>
#include <sys/time.h>
EXPR subexpr[128];

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
} /*testing */

int compare_trees(NC *t1,NC *t2)
{
	if(t1!=NULL&&t2!=NULL)
	{	
		if(t1->type==t2->type&&t1->inc==t2->inc)
			return compare_trees(t1->link,t2->link)&compare_trees(t1->list,t2->list);
	}
	else if(t1==NULL&&t2==NULL) return 1;
	
	return 0;
}


/*****************************************************************************/

/*   This function check the conditions of two path passing as arguments. 
     It returns 0   if the conditions are not matched.
                1   if conditions are matched.    
                2   if conditions of tempPath matched with that of path but 
                       path has some other conditions also. In this case this 
                       path needs to add to its successor.                   */

/*****************************************************************************/    

int checkCondition(PATH P1, PATH tempPath)
{
      NC *t, *temp;
      int canProceed=0, flag=0;
	 // displayApath(path); 
      t=P1.condition; // R_\beta 
      if(t==NULL) // no condition of execution of the path beta
	  {
			if(tempPath.condition == NULL) // for this path emanates from k
           	{                                    // there is no condition of execution
				canProceed=1; // canProceed=1 indicates both the path and the outward transition have
                              // no condition of execution; so we can proceed for checking the r_\alpha s.
	
		 	}
			else
			{
				canProceed=0; // can not proceed in this path as condition in this outward paths does
								  //not mathched
			}
	 }
	 else // beta have some condition of execution
	 {
			if(tempPath.condition == NULL)
			{
				canProceed=2; //canProceed=2 indicates beta have R_\alpha but this outward path 
							  //has no R_\alpha; still we can proceed as condition may matched in
							  //the path that follows this outward path.
		 	}
			else // outward path has condition of execution
		    {  
			    canProceed=0;
			    temp=tempPath.condition->link;
                while(temp!=NULL)
	     	    {
		              t=P1.condition->link;	     
			          flag=0;
				      while(t!=NULL)
				      {
			            	if(t->inc==0)
				        	{
							    if(compare_trees(t->link,temp->link))
		     			        {
								      flag=1;
   							          break;			     			
								}
					        } 
							t=t->list;
				        }
						if(flag == 0)
						{
						   	canProceed=0;
							break;
						} 
				        temp=temp->list;
				   	}  
					if(temp == NULL)
						canProceed=3; //all components of the condition of this outward path 
 										  // have an equivalent in that of the path. 
					
			} // end else-if(tempPath->condition == NULL)
	}//end else-if(t=NULL)

	if(canProceed == 3) //lets check in other ways. It means check both conditions are totally
						// matched or not. 
	{
			temp=P1.condition->link;
           	while(temp!=NULL)
	        {
		         t=tempPath.condition->link;	     
			     flag=0;
				 while(t!=NULL)
				 {
			        	if(t->inc==0)
					    {
			             	   if(compare_trees(t->link,temp->link))
		     			       {
							      flag=1;
   						          break;			     			
								}
						} 
						t=t->list;
				}
				if(flag == 0)
				{
				   	canProceed=2; //condition of the outward paths partially matched with the
								  // condition of the path. So its required to extend this path 
					break;
				} 
				temp=temp->list;
			}  
			if(temp == NULL)
					canProceed=1; //conditions are totally matched. Need not to extend. 
	}

	return canProceed;	
}
int r_alpha_equal(r_alpha *r1,r_alpha *r2)
{
	r_alpha *temp1,*temp2;
	int flag=0;
	temp1=r1;
	temp2=r2;
	if (temp2==NULL&&temp1==NULL)
       {  
          printf("\n both r_alpha NULL");
		  return 1;
       }
	else if (temp2== NULL) 
				return 0;
		 else if(temp1==NULL) 
				return 0;

	/*** successful completetion of this loop gurrentees that all the 
	operation of r2 have an equivalent one in r1; but does not gurrenteed 
    that r1 dont have any extra operation; so we have to check the other direction**/
	while(temp2!=NULL)
	{
	    temp1=r1;
	    
        	flag=0;
       		while(temp1!=NULL)
	 	{
			if(temp2->Assign.lhs==temp1->Assign.lhs)
			{ 
				if(compare_trees(temp1->Assign.rhs,temp2->Assign.rhs))
				{
				  	flag=1;
					break;
				}
			}
			temp1=temp1->next;
		}
		if(flag==0)
			return 0;
	    temp2=temp2->next;
   	}

    /** Following loop gurrentees that r1 does not have any extra operation ***/
	temp1=r1;
	while(temp1!=NULL)
	{
	    temp2=r2;
	    flag=0;
       		while(temp2!=NULL)
	 	{
			if(temp2->Assign.lhs==temp1->Assign.lhs)
			{ 
				if(compare_trees(temp1->Assign.rhs,temp2->Assign.rhs))
				{
				    flag=1;
				    break;
				}
			}
			temp2=temp2->next;
		}
		if(flag==0)
			return 0;
	    temp1=temp1->next;
   	 }

     return 1;
	 
}

void CorrespondingPlaces(PRESPLUS M0, PRESPLUS M1 ,PATHSET P1, PATHSET P2)
 {
// printf("MDDIJDMDJIDDDJJDJD");
  
  int i,j,n,m,k;
  //PATHSET P1,P2;
  //P1=findpaths(M0);
 // P2=findpaths(M1);
  printf("The corresponding places are ");
  for(i=0;i<M0.no_of_places_initially_marked;i++)
      printf("%s",M0.places[M0.initial_marking[i]].name);

 for(i=0;i<M1.no_of_places_initially_marked;i++)
      printf("%s",M1.places[M1.initial_marking[i]].name);
 for(i=0;i<M0.no_of_places;i++){
  if( M0.places[i].no_of_postset==0)
     printf("%s",M0.places[i].name);
   }

for(i=0;i<M1.no_of_places;i++){
  if( M1.places[i].no_of_postset==0)
     printf("%s",M1.places[i].name);
   }   
    
   for(i=0;i<P1.num_of_path;i++)
   {
    // printf("PATH %d IS CHECKING...............\n\n",i+1);
     for(j=0;j<P2.num_of_path;j++)
    {
      n=r_alpha_equal(P1.path[i].transformations,P2.path[j].transformations);
      m=checkCondition(P1.path[i],P2.path[j]);
    
   if(n==1&&m==1){
     
  for(k=0;k<M0.transitions[P1.path[i].segments[P1.path[i].size-1]].no_of_preset;k++){
      if(M0.places[M0.transitions[P1.path[i].segments[P1.path[i].size-1]].preset[k]].token_present!=1)
     printf("%s\t",M0.places[M0.transitions[P1.path[i].segments[P1.path[i].size-1]].preset[k]].name);
   }
for(k=0;k<M1.transitions[P2.path[j].segments[P2.path[j].size-1]].no_of_preset;k++){
    if(M1.places[M1.transitions[P2.path[j].segments[P2.path[j].size-1]].preset[k]].token_present!=1)
     printf("%s\t",M1.places[M1.transitions[P2.path[j].segments[P2.path[j].size-1]].preset[k]].name); 
  } 

  }
  
  }

 }
}


NC* creat_expr()
{
 NC *S,*T,*P;
 S=(NC*)malloc(sizeof(NC));
 T=(NC*)malloc(sizeof(NC));
 P=(NC*)malloc(sizeof(NC)); 
 //S=root;
 S->inc=0;
 T->inc=0;
 P->inc=0;
 
 S->type='S';
 T->type='T';
 P->type='v';
 S->link=T;
 T->link=P;
 P->link=NULL;
 S->list-NULL;
 T->list=NULL;
 P->list=NULL;
 return S;
    
 }
void make_cmplt(PRESPLUS model,PATHSET paths)
{
  int i,j;
 for(i=0;i<paths.num_of_path;i++)
   {

   for(j=paths.path[i].size-1;j>=0;j--)
       model.transitions[paths.path[i].segments[j]].cmplt=0;
   } 

}

void find_R_and_r_alpha(PRESPLUS model,PATH* path)
{
   int i,j,k,l,m,n,p,q,r;
   //make_cmplt(model,paths);
 //for(i=0;i<paths.num_of_path;i++)
   //{
   
    
    NC *c1,*prev,*T,*P,*S,*Q;
    r_alpha *temp1,*temp2,*temp3,*mytemp;
    //r_alpha *temp,*temp4;
     j=0;
    NC *result;
    NC *temp_rhs, *nc_ptr,*tmp_rhs;	
    char ch[4],c;
    char *sym_value;
        sym_value = (char * ) malloc( 100*sizeof( char ) );
    int index;
    TRANSITION *trans,*temp;
    
    result=NULL;
    prev=NULL;
    if(path->size!=0)
         
     j=path->size-1; 
     //temp1=paths.path[i].transformations;
     //temp2=temp1;
     path->transformations=NULL;
    while(j>=0)
    {
           c1=copylist(model.transitions[path->segments[j]].condition);
           if(c1!=NULL)
           {
	    //   temp1=path->transformations;
	    //   while(temp1!=NULL)
	    //   {

	       // 	c1=substitute_in_condition(c1,temp1->Assign.lhs,temp1->Assign.rhs);
		//        temp1=temp1->next;
	    //   }
               if(result==NULL)
               {
                    result=c1;
                    //	printf("\n the condition before applying DTS \n");
            	   //write_lists(result);
            	    	//printf("\n\n"); 
                    prev=c1->link;		    			
               }
               else
               {    
               	   prev->list=c1->link;
           	 //  prev=prev->list;
                 //  free(c1);
               }
		while(prev->list !=NULL)
		{
		    prev=prev->list;
			    
		}

           } 
          
          
            	//write_lists(model.transitions[paths.path[i].segments[j]].action[0].rhs); 
	  
		//printf("\n");
	//while(nextpn->actions[i].rhs!=NULL)
	  // {
		//printf("for path \n");
		temp1=path->transformations;
		/*if(temp1==NULL)
		// printf("It is nuLL");
		 else {printf("(%d)",temp1->Assign.lhs);}
		//printf("(%p->)\n",temp);*/
		temp2=temp1;
		if(temp1==NULL)
		{
			//printf("IT is in IF");
			 temp3=(r_alpha *)malloc(sizeof(r_alpha));
			 temp3->Assign.lhs=model.transitions[path->segments[j]].action[0].lhs;
			// printf("(%d->)\n",temp3->Assign.lhs);
			 temp3->Assign.rhs=copylist(model.transitions[path->segments[j]].action[0].rhs);
			 temp3->next=NULL;
			 path->transformations=temp3;
		}
		else
		{
			/* Here I have to add code to transform the rhs of the actions[i]. 
			   For example, If the action[i].rhs=a+b, and a is (c+d) in the path 
			   transformation, then action[i].rhs will become c+d+b, this will now 
			   store as the present transfer value of action[i].lhs */ 

			
			
		/*	temp_rhs=copylist(nextpn->actions[i].rhs);
			nc_ptr=temp_rhs;
		//	if(nc_ptr->list==NULL && nc_ptr->link==NULL) // rhs is a constant
				
				
				
			prev=nc_ptr;
			nc_ptr=nc_ptr->link; //Term level
			if(nc_ptr->list==NULL) //formula consists of one term
			{
				prev=nc_ptr;
				nc_ptr=nc_ptr->link; // primary level
				if(nc_ptr->list ==NULL && (nc_ptr->type!='D'||nc_ptr->type!='M')) 
				{			//consist of one variable

					// searching whether any updated value of the variable exist or not  
					temp1=path->transformations;
					while(temp1!=NULL)
					{
						if(temp1->Assign.lhs==nc_ptr->inc)
							break;
						temp1=temp1->next;		
					}	
					if(temp1!=NULL) //updated value exist
					{
						temp_rhs=copylist(temp1->Assign.rhs);				
						write_lists(temp_rhs);
					}
					
				}	
			} 
			
		 	
			while((temp1!=NULL)&&(temp1->Assign.lhs<nextpn->actions[i].lhs))
			 {
				 temp2=temp1;
				 temp1=temp1->next;
			 }
			 if(temp1!=NULL)
			 {
				 if(temp1->Assign.lhs==nextpn->actions[i].lhs)		
				 {
				     temp3=(r_alpha *)malloc(sizeof(r_alpha));
				     temp3->Assign.lhs=nextpn->actions[i].lhs;
				     temp3->Assign.rhs=copylist(temp_rhs);
				     if(temp1==path->transformations)
				         path->transformations=temp3;
				     else
				          temp2->next=temp3;
				     temp3->next=temp1->next;	 					 
				 }
				 else 
				 {
	  	                     temp3=(r_alpha *)malloc(sizeof(r_alpha));
				     temp3->Assign.lhs=nextpn->actions[i].lhs;
				     temp3->Assign.rhs=copylist(temp_rhs);
				     if(temp1==path->transformations)
				         path->transformations=temp3;
				     else
				         temp2->next=temp3;
				     temp3->next=temp1;	 
				 }
			 }
			 else
			 {
                  		 temp3=(r_alpha *)malloc(sizeof(r_alpha));
			         temp3->Assign.lhs=nextpn->actions[i].lhs;
			         temp3->Assign.rhs=copylist(temp_rhs);
			         temp2->next=temp3;
				 temp3->next=NULL;
    			 }  */
	



                             //printf("It is in else \n");
			
			if(temp1!=NULL)
			 {
			 //printf("It is loop\n");
			    if(temp1->Assign.lhs<model.transitions[path->segments[j]].action[0].lhs)
			    {	 temp2=temp1;
				 temp1=temp1->next;
			    }
			 }
			 if(temp1!=NULL)
			 {
				 if(temp1->Assign.lhs==model.transitions[path->segments[j]].action[0].lhs)		
				 {

			//		temp1->Assign.rhs=substitute_in_sum(temp1->Assign.rhs,
			//				nextpn->actions[i].lhs, nextpn->actions[i].rhs);
					 temp1->Assign.rhs=NULL;
					 temp1->Assign.rhs=copylist(model.transitions[path->segments[j]].action[0].rhs);
			
					 temp3=temp1;
				 }
				 else 
				 {
	  	                     temp3=(r_alpha *)malloc(sizeof(r_alpha));
				     temp3->Assign.lhs=model.transitions[path->segments[j]].action[0].lhs;
				     temp3->Assign.rhs=copylist(model.transitions[path->segments[j]].action[0].rhs);
				     if(temp1==path->transformations)
				         path->transformations=temp3;
				     else
				         temp2->next=temp3;
				     temp3->next=temp1;	 
				 }
			 }
			 else
			 {
                  		 temp3=(r_alpha *)malloc(sizeof(r_alpha));
			         temp3->Assign.lhs=model.transitions[path->segments[j]].action[0].lhs;
			         temp3->Assign.rhs=copylist(model.transitions[path->segments[j]].action[0].rhs);
			         temp2->next=temp3;
				temp3->next=NULL;
    			 }  

		 }
			    
		 //    printf("\n lhs value is %d \n",temp3->Assign.lhs);
	         // write_lists(temp3->Assign.rhs);
		 //i=i+1;
	  	
           j=j-1;
    }
    //free(temp1);
    
    path->condition=result;
    //write_lists(result);
   // printf("\n");
    //paths.path[i].transformations=temp2;
     mytemp=path->transformations;
    /*while(mytemp!=NULL){
    //printf("Mytemp::");
    write_lists(mytemp->Assign.rhs);
    printf("\n"); 
       mytemp=mytemp->next;
    }*/
  
}

void Associate_R_and_r_alpha(PRESPLUS model, PATHSET paths)
{
   int i,j;
   
   for(i=0;i<paths.num_of_path;i++)
     {
    // printf("\n///////////////////////////////////PATH %d /////////////////////////// \n",i);
       find_R_and_r_alpha(model,&paths.path[i]);
     
     }



}
/*void pathcomputation(PRESPLUS model,PATHSET paths)
{

  char *sym_value;
        sym_value = (char * ) malloc( 1000*sizeof( char ) );

    r_alpha *temp1;

   int i=0,j,k;
	 for(i=0;i<paths.num_of_path;i++) {
          
              printf("\n//////////////////path %d/////////////////////\n\n",i+1);
      
              //k=paths.path[i].size-1;
                      
              printf("\ncondition:");
              write_lists(paths.path[i].condition);
              printf("\nTransformation:");
               temp1=paths.path[i].transformations;
		  while(temp1!=NULL)
		  {
		          symbol_for_index( temp1->Assign.lhs, sym_value );
			  printf("\n %s := ", sym_value );
			  
			  write_lists(temp1->Assign.rhs);
			  temp1=temp1->next;
		  }
                 printf("\n");
          }

}*/
EXPR findsubexpr(EXPR expr)
{
  EXPRNODE tmp = *expr ;
 if(tmp.value==7)
     return expr->right;

 else findsubexpr(expr->left);
}
char findleft(EXPR expr)
{
  EXPRNODE tmp = *expr ;
  if(expr->left==NULL)
   return tmp.value;
  else findleft(expr->left);

}

int Validation(PRESPLUS model,PATHSET P)
  {
    Initial(&model);
    int i,j;
    for(i=0;i<P.num_of_path;i++)
       for(j=0;j<P.path[i].size;j++)
          model.transitions[P.path[i].segments[j]].mark=1;
    
     for(i=0;i<model.no_of_transitions;i++)
        {
          if(model.transitions[i].mark!=1)
            return 0;
        }
       for(i=0;i<P.num_of_path;i++){
           if(model.places[model.transitions[P.path[i].segments[P.path[i].size-1]].preset[0]].label==2)
             {

               if(model.places[model.transitions[P.path[i].segments[0]].postset[0]].label!=2)
                        return 0;
             }
          }
       return 1;
      
 }    


/*****************************************************************************/

/*     Arguments are here two r_alpha ( first one of outward transitions(head)
       and  second one for path transformation(path->transformations)). 
       This function compares them. 
       Returns 1 if they equal; otherwise returns 0          				 */

/*****************************************************************************/


void findEquivalent(PRESPLUS N0,PRESPLUS N1)
{
  printf("adwawd\n\n");
  PATHSET P1,P2;
  int i,j,n,m,count=0,temp;
  char *sym_value;
        sym_value = (char * ) malloc( 1000*sizeof( char ) );
  r_alpha *temp1;

       // int i,p;  	
	//int tfront;






        struct timeval tv1;
	struct timezone tz1;
	struct timeval tv2;
	struct timezone tz2;
       
        settimeofday(&tv1, &tz1);  /*for testing */ 
        gettimeofday(&tv1, &tz1); /*for testing */
        
        clock_t start,end; /*for testing */
        start=clock();    /*for testing */
        double cpu0  = get_cpu_time();

		printf("Reached Point 2\n");

       // gettimeofday(&tv1, &tz1);
       P1=ConstructAllPaths(&N0);

		printf("Reached Point 3\n");


       P2=ConstructAllPaths(&N1);
       //if(Validation(M0,P1)&&Validation(M1,P2)){
       printf("<--------------------The Models are Valid----------------->\n");
       Associate_R_and_r_alpha(N0,P1);
      // pathcomputation(M1,P1);
       Associate_R_and_r_alpha(N1,P2);
       //pathcomputation(M1,P2);
     //CorrespondingPlaces(M0,M1,P1,P2);
   printf("\n##################### PATH  EQUIVALANCE #######################\n\n");
   
   for(i=0;i<P1.num_of_path;i++)
   {
     n=0;m=0;
     //printf("PATH %d IS CHECKING...............\n\n",i+1);
     for(j=0;j<P2.num_of_path;j++)
    {
      n=r_alpha_equal(P1.path[i].transformations,P2.path[j].transformations);
      m=checkCondition(P1.path[i],P2.path[j]);
     //printf("\n%d-->%d\n",n,m);
      if(n==1||m==0){
        count++,temp=i;
        printf("PATH %d OF MODEL 1 IS MATCHED WITH PATH %d OF MODEL 2\n\n",i+1,j+1);
       // printf("\nTHE CONDITION IS\n");
       // write_lists(P1.path[i].condition);
        // printf("\nTHE TRANSFORMATION IS\n");
          temp1=P1.path[i].transformations;
          // write_lists(temp1->Assign.rhs);
		  while(temp1!=NULL)
		  {
		          symbol_for_index( temp1->Assign.lhs, sym_value );
			 // printf("\n %s := ", sym_value );
			  
			  //write_lists(temp1->Assign.rhs);
			  temp1=temp1->next;
		  }
                 printf("\n");
             
       break;
         }//end if
           
    } 
   if(temp!=i)
     printf("\n\n>>>>Failed to find the corresponding path %d of MODEL 1 in MODEL 2>>> \n \n ",i+1);

  }
 if(count==P1.num_of_path)
  {
  printf("\n\n<<<<<<<<<<<<<<<<< THE TWO MODEL ARE EQUIVALENT >>>>>>>>>>>>>>>>>\n\n");

 }  
else
    printf("\n\n<<<<<<<<<<<<<<<<<< THE TWO MODEL ARE NOT EQUIVALENT >>>>>>>>>>>>>>>>>>>\n\n"); 

printf("\n\n\n\n###################### Verification Report ##############################\n\n");

 printf("\n No. of places in N0: %d and No. of places in N1: %d\n", N0.no_of_places, N1.no_of_places);

 printf("\n No. of paths in initial path cover of M0: %d and No. of paths in actual path cover of M0: %d\n", 
        P1.num_of_path, P2.num_of_path);
  gettimeofday(&tv2, &tz2);
 settimeofday(&tv2, &tz2); /*for testing*/
//gettimeofday(&tv2, &tz2); /*for testing*/ 
 double cpu1  = get_cpu_time(); /*for testing*/ 
  end=clock();
  double extime=(double) (end-start)/CLOCKS_PER_SEC; /*for testing*/
 // printf("\n get_cpu_time execution time is and %lf microsecs\n", cpu1-cpu0);
  printf("\n get_cpu_time execution time is %ld sec and %ld microsecs\n", tv2.tv_sec - tv1.tv_sec, ((tv2.tv_usec - tv1.tv_usec)-1435));
 
  printf("\n##########################################################################\n");
 }



