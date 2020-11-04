#include "Utility.h"
#include "findpath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/time.h>
#define RM -999
//EXPR subexpr[128];

  
int search_cp(CUTPOINT cp, int p)
{
  printf("search_cp() is called is called ................\n");
  int i=0;
   for(i=0;i<cp.num_of_cp;i++){
      if(cp.cplist[i]==p){
        // printf("search_cp() is ended //////////////////");
         return 1;}
      }
printf("search_cp() is ended //////////////////");
     return 0;
}
CUTPOINT findcutpoints(PRESPLUS *model, CUTPOINT cp) {
	/**
	 * The cutpoint finding routine
	 * uses the naive algorithm, places with 0 presets/0 postsets/more than 1 presets
	 * marks type as 0 for in-ports, 1 for out-ports, 2 for intermediate cutpoints
	 */
printf("\nfindcutpoints() is bigen ...............\n");

	int i=0,j,k, count=0, num=0, flag=0;
	cp.num_of_cp=0;
	for(i=0;i<MAXNOPLACES;i++)
		cp.cplist[i]=0;
        for(i=0;i<model->no_of_places;i++)
              model->places[i].cut=0;
	for(i=0;i<model->no_of_places;i++) {
		if( model->places[i].no_of_preset==0 || model->places[i].no_of_postset==0){
                         model->places[i].cut=1;
			cp.num_of_cp++;
			cp.cplist[cp.num_of_cp-1]=i;
			if(model->places[i].no_of_preset==0)
                              {
			cp.cptype[cp.num_of_cp-1] = 1;
                                //model->places[i].label=1;
                                model->places[i].lcut=1;
                               
                              }
			else {
				cp.cptype[cp.num_of_cp-1] = 0;
			       // model->places[i].label=0;
                                model->places[i].lcut=1;
                                 
                              }
		}
		if(model->places[i].no_of_preset>1){
		
		for(j=0;j<model->places[i].no_of_preset;j++)
		  {
		  
		  for(k=0;k<model->transitions[model->places[i].preset[j]].no_of_preset;k++)
		    {
		       if((model->places[i].var_index <=
		       model->places[model->transitions[model->places[i].preset[j]].preset[k]].var_index)&&      
		       (!search_cp(cp,i)))
		           {
                            model->places[i].cut=1;	             
                            cp.num_of_cp++;
			    cp.cplist[cp.num_of_cp-1]=i;
                            //model->places[i].label=2;
                            cp.cptype[cp.num_of_cp-1] = 2;	                        
		           }		    
		    }
		  }
	}
       }
	for(i=0;i<cp.num_of_cp;i++)
		printf("%s(type=%d)\t",model->places[cp.cplist[i]].name,cp.cptype[i]);
	return cp;

}

SET_TRANS Initialize_trans(SET_TRANS T)
{
  int i;
  T.tseq=(CONCURRENT_TR_SET*)calloc(MAXNOTRANS,sizeof(CONCURRENT_TR_SET));
  for(i=0;i<MAXNOTRANS;i++){
    T.tseq[i].t_seq=(SET_OF_TRANS)calloc(MAXNOTRANS,sizeof(int));
   T.num_seq=0;
   }
return T;
}

void Initial(PRESPLUS *model){
 int i;
 for(i=0;i<model->no_of_transitions;i++)
 {
   model->transitions[i].mark=0;
   //model->transitions[i].val=0;
 }
 for(i=0;i<model->no_of_places;i++)
   model->places[i].copy=0;

}

/* This function is used to mark the transitions*/
void mark(CONCURRENT_TR_SET Te,PRESPLUS *model)
{
 //printf("\nmark() is called.............\n");
  int i,j;
   
  for(i=0;i<Te.size;i++){
        model->transitions[Te.t_seq[i]].mark=1;
   //printf("\n\n\n<<<<< %s is marked=%d >>>>>\n",model->transitions[Te.t_seq[i]].name,model->transitions[Te.t_seq[i]].mark);
      }
//printf("mark() is ended //////////\n");
}
//It is used to find the differece of two sets

MARK_H Difference(MARK_H P,int p)
{
//printf("\n Difference() is called...............\n");

  // int i,low=0,high=P.num_of_marking,middle;
   int i=0;
  while(i<P.num_of_marking)
   {
     
     if(P.mp[i]==p)
          {
          // printf("\n%d is found in Mnew\n",p);
     
            
           P.mp[i]=P.mp[P.num_of_marking-1];
           P.num_of_marking--;
         }
     else{
           i++;
         }
         
  }   
 
//printf(" Difference() is ended //////////\n");
   return P;

}/* It is used to find the differece between two sets */


//It is used to find the Union of Two sets

MARK_H Union(MARK_H P,int p)
{
 //printf("\nUnion() is called ...............\n");
  int i;
  for(i=0;i<P.num_of_marking;i++)
   {
     if(P.mp[i]==p){
    // printf("%d is already present in Mh\n",p);
//printf("\n Union() is ended ///////////\n");
       
        return P;}
   }
  P.mp[P.num_of_marking++]=p;
 //printf("%d is inserted in Mh",p);
//printf("\n Union() is ended ///////////\n");
 return P;

}

// It is used to add the given path to the set of paths
SET_OF_PATHS Update_Q(SET_OF_PATHS paths,SET_TRANS T,PRESPLUS *model)
{
   int i,j,pathsize=0;
    int k=paths.num_of_path,size;
        paths.num_of_path++;
	if(k==0) {
		paths.path=(PATH*)calloc(1,sizeof(PATH));
	}
	else {
		paths.path=(PATH*)realloc(paths.path,(k+1)*sizeof(path));
                
	   }
     
  for(i=0;i<T.num_seq;i++)
  {
   for(j=0;j<T.tseq[i].size;j++)
    {
      pathsize+=T.tseq[i].size;

    }
  }
   paths.path[k].size=pathsize;
   paths.path[k].segments=(int*)calloc(pathsize,sizeof(int));
  size=0;
//printf("path %d is \t ",k);
  for(i=0;i<T.num_seq;i++)
  {
   for(j=T.tseq[i].size-1;j>=0;j--)
    {
       paths.path[k].segments[size++]=T.tseq[i].t_seq[j];
     // model->transitions[paths.path[k].segments[size-1]].vald=1;     
       //printf("%d\t",paths.path[k].segments[size-1]);
     }
     
   }
printf("\n");
//printf("Update_Q() is ended ////////////////////////\n");
return paths;

}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

CONCURRENT_TR_SET Remove_duplicate(CONCURRENT_TR_SET tseq)
{
  int i=0,j;
 qsort(tseq.t_seq,tseq.size,sizeof(int),compare);
 for(j=0;j<tseq.size;j++)
  {
    if(tseq.t_seq[i]!=tseq.t_seq[j])
     {
        i++;
        tseq.t_seq[i]=tseq.t_seq[j];
     }  
  }
tseq.size=i+1; 
return tseq;
} 
int Check_t(CONCURRENT_TR_SET tseq,int t)
 {
//printf("\nCheck_t() is called...........\n");
   int i,j;
   for(j=0;j<tseq.size;j++)
      {
        if(tseq.t_seq[j]==t){
       // printf("\n Check_t() is ended ................\n");
            return 1;}
     
     }
//printf("\n Check_t() is ended ................\n");
 return 0;
 }/* wheather the given transition is already exits in the set of transition
     list or not (eliminate the duplicate)*/
//This function is used to Construct single path
SET_TRANS ConstOnePath(MARK_H Pset,SET_TRANS Tsh,PRESPLUS *model,SET_TRANS T)
{

 // printf("\nConstOnePath() is called ..................\n");
    int i,j,k,count,var=0,size=0;
    //printf("\nThe size of Tsh is ........%d\n ",Tsh->num_seq);
    if(Tsh.num_seq==0)
       return T; 
  /*  printf("\n ######################## The elements of Pset are ######################## \n");          
    for(i=0;i<Pset.num_of_marking;i++)
    printf("%s \t",model->places[Pset.mp[i]].name);
    printf("\n ############################################################### \n");*/

    for(i=0;i<Pset.num_of_marking;i++)
       {
       for(j=0;j<model->places[Pset.mp[i]].no_of_preset;j++)
          {
            //printf("%s it is checking\n",model->transitions[model->places[Pset.mp[i]].preset[j]].name);
            if(Check_t(Tsh.tseq[Tsh.num_seq-1],model->places[Pset.mp[i]].preset[j]))
               {
                 
                 //printf("It is here in if \n"); 
                     var=1;
                T.tseq[T.num_seq].t_seq[size++]=model->places[Pset.mp[i]].preset[j];
               } 
           }
        }
     //printf("It is here %d\n",T.num_seq);
    if(var==1){
    // printf("It is here %d\n",var);
     T.tseq[T.num_seq].size=size;
    T.tseq[T.num_seq]=Remove_duplicate(T.tseq[T.num_seq]);
   
    T.num_seq+=1;
    var=0;
    
    Tsh.num_seq--;}


    if(Tsh.num_seq==0||T.num_seq==0)
      return T;
    else
        {
         // quicksort ( Pset.mp, 0, Pset.num_of_marking-1 );
          for(j=0;j<T.tseq[T.num_seq-1].size;j++)
               {
                  for(k=0;k<model->transitions[T.tseq[T.num_seq-1].t_seq[j]].no_of_postset;k++)
                    {

                      Pset=Difference(Pset,model->transitions[T.tseq[T.num_seq-1].t_seq[j]].postset[k]);
                    
                   }
               }
        }
        qsort(Pset.mp,Pset.num_of_marking,sizeof(int),compare);

   /*printf("\n ######################## The elements of Pset are ######################## \n");          
   for(i=0;i<Pset.num_of_marking;i++)
   printf("%s \t",model->places[Pset.mp[i]].name);
      printf("\n ############################################################### \n");
      /* printf("\nT.num_seq for union is %d\n",T.num_seq);
  printf("\n  ############## The elements of T.tseq[num_seq-1] in ConstOnePath are ###########################\n");
      for(j=0;j<T.tseq[T.num_seq-1].size;j++){
       printf("%s\t",model->transitions[T.tseq[T.num_seq-1].t_seq[j]].name);
   printf("  \n");
    }
 printf("\n################ The elements of TSh in ConstOnePath are ended ################## \n");*/

      /*for(i=0;i<T.num_seq;i++)
          {*/// printf("\ntttttttttttttt\n");
              
              for(j=0;j<T.tseq[T.num_seq-1].size;j++)
                  { //count=0;
                    for(k=0;k<model->transitions[T.tseq[T.num_seq-1].t_seq[j]].no_of_preset;k++)
 
                      { // printf("\ncut is %d\n",model.places[model.transitions[T.tseq[T.num_seq-1].t_seq[j]].preset[k]].cut);
                      if(model->places[model->transitions[T.tseq[T.num_seq-1].t_seq[j]].preset[k]].cut!=1)
                               //count++;
                       //printf("\n %s is inserted in Pset\n",model.places[model.transitions[T.tseq[T.num_seq-1].t_seq[j]].preset[k]].name);
                       Pset=Union(Pset,model->transitions[T.tseq[T.num_seq-1].t_seq[j]].preset[k]);
                      }
                
              }
  
           
      if(Pset.num_of_marking==0)
           return T;
      return ConstOnePath(Pset,Tsh,model,T);
  
}



int Check_marking(int p, PRESPLUS *model)
{
 //printf("\nCheck_marking() is called..............\n");
  int i,j;
  
  for(i=0;i<model->places[p].no_of_postset;i++)
      {
  //        printf("%s.mark=%d\n",model.transitions[model.places[p].postset[i]].name,model.transitions[model.places[p].postset[i]].mark);
         if(model->transitions[model->places[p].postset[i]].mark==0){
//printf("Check_marking() is ended///////////////\n");
            return 0;     }
   
     }
//printf("Check_marking() is ended///////////////\n");
  return 1;


}


// It is used to Compute the CrossProduct

SET_TRANS CrossProduct(int a[],int index,int n,SET_TRANS temp,SET_TRANS CPT, int *m)
{
//printf("\nCrossProduct() is called............ \n");
 int i,j;
// static int m;
 if(index==n)
  { 
     
     //printf("\n");
     
     for(i=0;i<temp.num_seq;i++){
      // printf("%d ",a[i]);
      // printf("***T.num_seq-->%d\n",CPT.num_seq); 
      CPT.tseq[*m].t_seq[i]=a[i];
    // printf("m=%d,S.tseq[m].t_seq[i]=%d \n",*m,CPT.tseq[*m].t_seq[i]);
        }
      *m=*m+1;
     return CPT;
  }
  
  for( j=0;j<temp.tseq[index].size;j++)
   {
    a[index]=temp.tseq[index].t_seq[j];
     CrossProduct(a,index+1,n,temp,CPT,m);
   // printf("***T.num_seq-->%d\n",CPT.num_seq); 
   }
 
// printf("***T.num_seq-->%d\n",CPT.num_seq); 
}
int Search_Mh(MARK_H Mh,int p)
{
  int i;
  for(i=0;i<Mh.num_of_marking;i++)
    {
      if(Mh.mp[i]==p)
       return 1;

    }
  return 0;


}
int same_sequense(CONCURRENT_TR_SET temp1,CONCURRENT_TR_SET temp2)
{
  int i,j;
  if(temp1.size!=temp2.size){
     return 0;}
  else
     {
      for(i=0;i<temp1.size;i++)
       {
       if(temp1.t_seq[i]!=temp2.t_seq[i]){
        return 0;}
      }
      return 1;
   }
}

SET_TRANS Remove_Repeated_sequence(SET_TRANS temp,PRESPLUS *model)
{
  int i,j;
    for(i=0;i<temp.num_seq;i++)
    {
      for(j=i+1;j<temp.num_seq;j++)
       {
         if(same_sequense(temp.tseq[i],temp.tseq[j])){
          // printf("\nIt is here Remove\n");
           //for(k=0;k<temp.)
            temp.tseq[j].size=RM;}
             
       }
    
    }

return temp;

}
// It Computes the All Sets Of Concurrent Transitions 
SET_TRANS ComputeAllSetsOfConcurrentTransitions(MARK_H Mh,PRESPLUS *model)
{
 //printf("\nComputeAllSetsOfConcurrentTransitions() is called..................\n");
  SET_TRANS temp,CPT;
  temp=Initialize_trans(temp);
  CPT=Initialize_trans(CPT);
  int i,j,k,temp1=1,count=0,m,size,p,q;
  temp.num_seq=0;
   for(i=0;i<Mh.num_of_marking;i++)
     {
          size=0;
       for(j=0;j<model->places[Mh.mp[i]].no_of_postset;j++)
         {
           m=0;
          // printf("\nFor %s is\n",model->transitions[model->places[Mh.mp[i]].postset[j]].name);
           for(k=0;k<model->transitions[model->places[Mh.mp[i]].postset[j]].no_of_preset;k++)
            {
                 if(Search_Mh(Mh,model->transitions[model->places[Mh.mp[i]].postset[j]].preset[k]))
                     {m++;}
             }
            // printf("\nm=%d\n",m);
              if(m==model->transitions[model->places[Mh.mp[i]].postset[j]].no_of_preset){
                
               //printf("\nIN IF\n ");
               temp.tseq[temp.num_seq].t_seq[size]=model->places[Mh.mp[i]].postset[j];
               size=size+1;
               temp.tseq[temp.num_seq].size=size;
               }
           
           
           } 
           
           if(temp.tseq[temp.num_seq].size!=0){
                               
             temp.num_seq++;
            }
           /* printf("************* Elements of temp are bigen %s*************\n",model->places[Mh.mp[i]].name);

            //for(p=0;p<temp.num_seq;p++){
            printf("temp.tseq[%d].size=%d\n",temp.num_seq-1,temp.tseq[temp.num_seq-1].size);
            for(q=0;q<temp.tseq[temp.num_seq-1].size;q++)
              {

                  printf(" %s ",model->transitions[temp.tseq[temp.num_seq-1].t_seq[q]].name);
              }
            printf("\n");
          
         printf("********** Elements of temp are ended ***********\n");*/
            
          
     }
   //printf("temp->num_seq=%d\n",temp.num_seq);
   
   //
   
  /*printf("************* Elements of temp are bigen *************\n");
   for(i=0;i<temp.num_seq;i++){
     // printf("temp.tseq[%d].size=%d\n",i,temp.tseq[i].size);
     for(j=0;j<temp.tseq[i].size;j++)
      {
          printf(" %s ",model->transitions[temp.tseq[i].t_seq[j]].name);
       }
    printf("\n");
   } 
 printf("********** Elements of temp are ended ***********\n");*/
  if(temp.num_seq==0)
    { 
        // printf("\nComputeAllSetsOfConcurrentTransitions() is ended //////////////////////\n");
         return temp;
    }
  else if(temp.num_seq==1)
        {
           //printf("The elements When temp.num_seq==1 (%d)\n",CPT.num_seq);
          CPT.tseq[CPT.num_seq].size=temp.tseq[0].size;
           for(i=0;i<temp.tseq[0].size;i++)
              {
                //CPT.tseq[CPT.num_seq].size=1;
                CPT.tseq[0].t_seq[i]=temp.tseq[0].t_seq[i];
                //printf("%d \t",CPT.tseq[CPT.num_seq-1].t_seq[0]);
              }
              CPT.num_seq++;
    // printf("The elements When temp.num_seq==1 (%d)\n",CPT.num_seq);
             return CPT;

         }
  else {
         //temp=Remove_Repeated_sequence(temp,model); 
  
         for(i=0;i<temp.num_seq;i++)
     {
       if(temp.tseq[i].size==RM)
         {
           if(i==temp.num_seq-1)
              {
                 temp.num_seq--;
                
              }
             else{
                  for(j=0;j<temp.tseq[temp.num_seq-1].size;j++)
                     temp.tseq[i].t_seq[j]=temp.tseq[temp.num_seq-1].t_seq[j];
                     temp.tseq[i].size=temp.tseq[temp.num_seq-1].size;
                }
         
         }
     
     
     } 
        for(i=0;i<temp.num_seq;i++)
          temp1*=temp.tseq[i].size;
          CPT.num_seq=temp1;
        for(i=0;i<CPT.num_seq;i++)
          CPT.tseq[i].size=temp.num_seq;
   
      //  printf(" ****T.num_seq-->%d\n",CPT.num_seq);
        int a[temp.num_seq];
        CrossProduct(a,0,temp.num_seq,temp,CPT,&count);
       // printf("\n CrossProduct() is ended \n");
        //CPT=Remove_Repeated_sequence(CPT,model);
        
        for(i=0;i<CPT.num_seq;i++)
     {
       if(CPT.tseq[i].size==RM)
         {
           if(i==CPT.num_seq-1)
              {
                 CPT.num_seq--;
                
              }
             else{
                  for(j=0;j<CPT.tseq[CPT.num_seq-1].size;j++)
                     CPT.tseq[i].t_seq[j]=CPT.tseq[CPT.num_seq-1].t_seq[j];
                     CPT.tseq[i].size=CPT.tseq[CPT.num_seq-1].size;
                }
         
         }
     
     
     } 
        for(i=0;i<CPT.num_seq;i++){
           CPT.tseq[i]=Remove_duplicate(CPT.tseq[i]);
           }
     /*printf("\n######### The concurrent transitions are ####################################\n");
        for(i=0;i<CPT.num_seq;i++){
           printf(" The elements of set %d are \n",i+1);
           for(j=0;j<CPT.tseq[i].size;j++)
              printf("%d\t",CPT.tseq[i].t_seq[j]);
           }
       printf("\n#########################################################################\n");*/
       return CPT;
  
     }

}
// It is used to find all the paths of the given Pres+ model
SET_OF_PATHS ConstructPaths(SET_TRANS Tsh,MARK_H Mh,CONCURRENT_TR_SET Te,PRESPLUS *model,SET_OF_PATHS Q,CUTPOINT cp )
{
           //printf("\nConstructPaths() is called ..................\n");
         int i,j,k,l,temp=0;
         MARK_H Mnew,Cset,Mh1;
    	 Mnew.num_of_marking=0;
    	 Mh1.num_of_marking=0;
    	 SET_TRANS path,T,Te1;
    	 path.num_seq=0;
    	 path=Initialize_trans(path);
   	 T=Initialize_trans(T);
    	 Te1=Initialize_trans(Te1);
       /*printf("\n ########### The elements of TSh are #####################\n");
           for(i=0;i<Tsh.num_seq;i++){
  
           for(j=0;j<Tsh.tseq[i].size;j++)
             printf("%s\t",model->transitions[Tsh.tseq[i].t_seq[j]].name);
             printf("  \n");
             }
       printf("\n##########################################################\n");
       printf("\n ####################### The elements of Te are ################## \n ");
          for(i=0;i<Te.size;i++)
            printf("%s ",model->transitions[Te.t_seq[i]].name);
            printf("\n ################################################################## \n");  
            printf("\n ######################## The elements of MH At Beginning  ################### \n");          
                  for(j=0;j<Mh.num_of_marking;j++)
                  printf("%s \t",model->places[Mh.mp[j]].name);
                  printf("\n ############################################################### \n");*/
           //printf("\nThe size of Tsh is %d\n",Tsh.num_seq);
    	 if(Te.size==0)
      	    return Q;
    	 
           mark(Te,model);
      
           if(Tsh.num_seq==0)
             {
              Tsh.num_seq=1;
              //  printf("Tsh->num_seq=%d\n",Tsh.num_seq);
              Tsh.tseq[0].size=Te.size;
              for(j=0;j<Te.size;j++)
                 {
               Tsh.tseq[0].t_seq[j]=Te.t_seq[j];
                 }
              }
           else {
              //Tsh.num_seq+=Te.num_seq;
       
              Tsh.tseq[Tsh.num_seq].size=Te.size;
              for(j=0;j<Te.size;j++)
                 Tsh.tseq[Tsh.num_seq].t_seq[j]=Te.t_seq[j];
              Tsh.num_seq+=1;
              } 

     
            /*printf("\n ########### The elements of TSh are after appending Te #####################\n");
           for(i=0;i<Tsh.num_seq;i++){
  
           for(j=0;j<Tsh.tseq[i].size;j++)
             printf("%s\t",model->transitions[Tsh.tseq[i].t_seq[j]].name);
             printf("  \n");
             }
       printf("\n##########################################################\n");*/

          //Mnew.mp=(int*)malloc(MAXNOPLACES*sizeof(int));
          Mnew.num_of_marking=0;
          for(i=0;i<Te.size;i++)
            {
          for(j=0;j<model->transitions[Te.t_seq[i]].no_of_postset;j++)
            {
                //printf("%d",model->places[model->transitions[Te.t_seq[i]].postset[j]].label);
                 if(!Search_Mh(Mnew,model->transitions[Te.t_seq[i]].postset[j])&& model->places[model->transitions[Te.t_seq[i]].postset[j]].label!=1){
                  
                 //if(){//&&model->places[model->transitions[Te.t_seq[i]].postset[j]].no_of_postset==0){
                    Mnew.mp[Mnew.num_of_marking++]=model->transitions[Te.t_seq[i]].postset[j];
                    model->places[Mnew.mp[Mnew.num_of_marking-1]].copy=0;
                     }
            }
          }
          /*printf("\n ################## The elements of Mnew i.e Te POSTSET side for are  ############### \n");          
          for(i=0;i<Mnew.num_of_marking;i++)
          printf("%s \t",model->places[Mnew.mp[i]].name);
          printf("\n ########################################################### \n");*/
 
          for(i=0;i<Mnew.num_of_marking;i++)
             {
           //printf("%s cut =%d\n",model->places[Mnew.mp[i]].name,model->places[Mnew.mp[i]].cut);
              if( model->places[Mnew.mp[i]].cut==1&& model->places[Mnew.mp[i]].copy!=1)
                {
                  
                  //printf("\n Cutpoint in Mnew is........ %s\n",model->places[Mnew.mp[i]].name);
                 /* printf("\nThe preset of this cut point is \n");
                  for(j=0;j<model->places[Mnew.mp[i]].no_of_preset;j++)
                  printf(" %s \t",model->transitions[model->places[Mnew.mp[i]].preset[j]].name);
                  printf("\n\n");*/
                  MARK_H Cset;
                  Cset.num_of_marking=0;
                  path=Initialize_trans(path);
                  path.num_seq=0;
                  //Cset.mp=(int*)malloc(MAXNOPLACES*sizeof(int));
                  Cset.mp[Cset.num_of_marking++]=Mnew.mp[i];

                  /*printf("\n ######################## The elements of MH Before path  ################### \n");          
                  for(j=0;j<Mh.num_of_marking;j++)
                  printf("%s \t",model->places[Mh.mp[j]].name);
                  printf("\n ############################################################### \n");*/
                  path=ConstOnePath(Cset,Tsh,model,path);
                  /* printf("\n ######################## The elements of MH after path  ######################## \n");          
                  for(j=0;j<Mh.num_of_marking;j++)
                    printf("%s \t",model->places[Mh.mp[j]].name);
                  printf("\n ############################################################### \n");*/
         
                  //printf("\nThe size of Tsh is %d\n",Tsh.num_seq);
                  if(path.num_seq>0){
                  Q=Update_Q(Q,path,model);
                 for(k=0;k<path.tseq[0].size;k++)
                 {
                   for(l=0;l<model->transitions[path.tseq[0].t_seq[k]].no_of_postset;l++)
                     {
                      if((model->transitions[path.tseq[0].t_seq[k]].postset[l]!=Mnew.mp[i])&& Search_Mh(Mnew,model->transitions[path.tseq[0].t_seq[k]].postset[l])){
                           //printf("%s",model->places[model->transitions[path.tseq[0].t_seq[k]].postset[l]].name);
                            //      printf("\nI in creament%d\n",i);
                                    //i++;
                                    model->places[model->transitions[path.tseq[0].t_seq[k]].postset[l]].copy=1;
                                    
                                    //printf("%d",model->places[model->transitions[path.tseq[0].t_seq[k]].postset[l]].copy);
                           }
                     
                     }
                 
                 
                 }
                 
                 }
                   //free(path);
                  //quicksort ( Mnew.mp, 0, Mnew.num_of_marking-1 );
                  //for(j=0;j<model->places[Mnew.mp[i]].no_of_preset;j++)
                   //{
                    /* for(k=1;k<model->transitions[model->places[Mnew.mp[i]].preset[0]].no_of_postset;k++)
                       {

                          if((model->transitions[model->places[Mnew.mp[i]].preset[0]].postset[k]!=Mnew.mp[i]))
                            {
                              if(model->places[model->transitions[model->places[Mnew.mp[i]].preset[0]].postset[k]].cut==1)
                                {
                                printf("%s",model->places[model->transitions[model->places[Mnew.mp[i]].preset[0]].postset[k]].name);
                                  printf("\nI in creament%d\n",i);
                                    i++;
                                // Mnew=Difference(Mnew,model->transitions[model->places[Mnew.mp[i]].preset[j]].postset[k]);qsort(Mnew.mp,Mnew.num_of_marking,sizeof(int),compare);
                                }
                            }
                         }*/
                     //}
                     
                     
                
           /*printf("\n ##################### The elements of Mnew after Mnew-S  ############# \n");          
          for(j=0;j<Mnew.num_of_marking;j++)
          printf("%s \t",model->places[Mnew.mp[j]].name);
         printf("\n ##################################################################### \n");*/
                      
                  //quicksort ( Mnew.mp, 0, Mnew.num_of_marking-1 );
                 if(model->places[Mnew.mp[i]].no_of_postset==0 ){
                    model->places[Mnew.mp[i]].label=1;
                    Mnew=Difference(Mnew,Mnew.mp[i]);
                    i--;
                    
                    //printf("It is deleted");
                    qsort(Mnew.mp,Mnew.num_of_marking,sizeof(int),compare);}
                 //printf("check is=%d\n",Check_marking(Mnew.mp[i],model));
                 // quicksort ( Mnew.mp, 0, Mnew.num_of_marking-1 );
                // if(Check_marking(Mnew.mp[i],model))
                  // Mnew=Difference(Mnew,Mnew.mp[i]);   
             }
          /* printf("\n ##################### The elements of Mnew at the end of for are ############# \n");          
          for(j=0;j<Mnew.num_of_marking;j++)
          printf("%s \t",model->places[Mnew.mp[j]].name);
         printf("\n ##################################################################### \n");*/
        }
        // quicksort (Mnew.mp, 0, Mnew.num_of_marking-1);
   
       /*printf("\n ####################### The elements of Te are ################## \n ");
       for(i=0;i<Te.size;i++)
       printf("%s ",model->transitions[Te.t_seq[i]].name);
       printf("\n ################################################################## \n");
       //quicksort ( Mh.mp, 0, Mh.num_of_marking-1 ); 
       printf("\n ######################## The elements of MH Before MH-Te PRESET  ######################## \n");          
       for(i=0;i<Mh.num_of_marking;i++)
       printf("%s \t",model->places[Mh.mp[i]].name);
       printf("\n ############################################################### \n");*/

       for(i=0;i<Te.size;i++){
          for(j=0;j<model->transitions[Te.t_seq[i]].no_of_preset;j++)
            Mh=Difference(Mh,model->transitions[Te.t_seq[i]].preset[j]);
            qsort(Mh.mp,Mh.num_of_marking,sizeof(int),compare);
         }
       /* printf("\n ######################## The elements of MH After Mh-Te PRESET ######################## \n");          
       for(i=0;i<Mh.num_of_marking;i++)
       printf("%s \t",model->places[Mh.mp[i]].name);
       printf("\n ############################################################### \n");
       printf("\n ################## The elements of Mnew before append to Mh ############### \n");          
       for(i=0;i<Mnew.num_of_marking;i++)
       printf("%s \t",model->places[Mnew.mp[i]].name);
       printf("\n ########################################################### \n");*/

       for(i=0;i<Mnew.num_of_marking;i++){ 
             // printf("%s is inserted in Mh\n",model->places[Mnew.mp[i]].name);
            Mh=Union(Mh,Mnew.mp[i]);
          }
         //quicksort (Mh.mp, 0, Mh.num_of_marking-1);
       /* printf("\n ######################## The elements of MH are After Mh Uion MNew ######################## \n");          
       for(i=0;i<Mh.num_of_marking;i++)
       printf("%s \t",model->places[Mh.mp[i]].name);
       printf("\n ############################################################### \n");*/
  
       T=ComputeAllSetsOfConcurrentTransitions(Mh,model);
   
       /* printf("\n ########### The elements of CPT are #####################\n");
       //printf("%d",T.num_seq);
       for(i=0;i<T.num_seq;i++){
  
       for(j=0;j<T.tseq[i].size;j++)
       //mark(T .tseq[i],&model);
       printf("%s\t",model->transitions[T.tseq[i].t_seq[j]].name);
       printf("  \n");
      }
     printf("\n##########################################################\n");*/
     if(T.num_seq==0){
       for(i=0;i<Mh.num_of_marking;i++){
           printf("Invalid:"); 
           }
       // printf("\nUNMARK is OVER\n");
        // Mh.num_of_marking=0; 
       }
 
     else{
           for(i=0;i<T.num_seq;i++)
                 {
                
                j=0;
                while(j<T.tseq[i].size)
                    {
                   // printf("transition=%s\n",model->transitions[T.tseq[i].t_seq[j]].name);
                     if(model->transitions[T.tseq[i].t_seq[j]].mark==1)
                       {
                       //printf("DIFFERENCE\n");
                        if(T.tseq[i].size>1)
                          {
                           T.tseq[i].t_seq[j]=T.tseq[i].t_seq[T.tseq[i].size-1];
                           T.tseq[i].size--;
                          }
                        else if(T.tseq[i].size==1)
                         {
                            T.tseq[i].size=0;
                          }
                       }
                     else {j++;}
                    }
                    //printf("\n,.....................,...............,,,, size%d\n",T.tseq[i].size);
               if(T.tseq[i].size>0){
               
        /*printf("\n ######################## The elements of MH  Before Call ######################## Mh.num_of_marking=%d\n",Mh.num_of_marking);          
          // Mh1.num_of_marking=Mh.num_of_marking;
            for(j=0;j<Mh.num_of_marking;j++)
                //  Mh1.mp[j]=Mh.mp[]
              printf("%s \t",model->places[Mh.mp[j]].name);
        printf("\n ############################################################### \n");*/
                 
                    Q=ConstructPaths(Tsh,Mh,T.tseq[i],model,Q,cp);
                    
                   /* printf("\n ######################## The elements of MH After Call ######################## Mh.num_of_marking=%d\n",Mh.num_of_marking);          
           for(j=0;j<Mh.num_of_marking;j++)
              printf("%s \t",model->places[Mh.mp[j]].name);

       printf("\n ############################################################### \n");*/
                    }
             }
          
       }
//printf("\nConstructPaths() is ended/////////////////////////////\n");
return Q; 
}

//It is used to construct all paths of the given model

SET_OF_PATHS ConstructAllPaths(PRESPLUS *model)

{
  //printf("\n/////////////// IN FUNCTION ConstructAllPaths()//////////////////////////////\n");
  int i;
  Initial(model);
  CUTPOINT cp = findcutpoints(model, cp);
  
  
  printf("ConstructAllPaths: Point 1\n");
  
  
  MARK_H Mh;
  SET_TRANS Tsh,T;
  Tsh=Initialize_trans(Tsh);
  T=Initialize_trans(T);
  Mh.num_of_marking=0;
  SET_OF_PATHS Q;
  Q.num_of_path=0;
  struct timeval tv1;
	struct timezone tz1;
	struct timeval tv2;
	struct timezone tz2;
        gettimeofday(&tv1, &tz1);

    //Mh.mp=(int*)malloc(MAXNOPLACES*sizeof(int));
    for(i=0;i<model->no_of_places_initially_marked;i++){
       //printf("%d \t",model->initial_marking[i]);
        Mh.mp[i]=model->initial_marking[i];
        Mh.num_of_marking++;
       }
  /*printf("\n################### Initial Mh is \n");
  for(i=0;i<Mh.num_of_marking;i++)
    printf("{%s \t",model->places[Mh.mp[i]].name);
     printf("}\n");
  printf("\n ################################ \n");*/
// for(i=0;i<Mh.num_of_marking;i++)
   //printf("%d\t",Mh.mp[i]);
   T=ComputeAllSetsOfConcurrentTransitions(Mh,model);
   //printf("T->num_seq=%d\n",T.num_seq);
 for(i=0;i<T.num_seq;i++)
      Q=ConstructPaths(Tsh,Mh,T.tseq[i],model,Q,cp);
//free(model);

 //printf("\n \n No. of places : %d \n \n ", model->no_of_places);
//printf("\n \n No. of transitions : %d \n \n ", model->no_of_transitions);
printf("\n *************PATH CONSTRUCTION TIME***************** \n \n ");
 printf("\n No. of paths: %d \n", 
        Q.num_of_path);
  
gettimeofday(&tv2, &tz2);
  printf("\n \n get_cpu_time execution time is %ld sec and %ld microsecs\n \n ", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
 // printf("\n \n gettimeofday execution time is %ld sec and %ld microsecs\n \n ", (tv2.tv_sec - tv1.tv_sec)*23, (tv2.tv_usec - tv1.tv_usec)*68);
   printf("\n \n########################################################################## \n \n");
return Q;
//printf("\n//////////////////////// END OF findpaths()////////////////////////////////////\n");
}


