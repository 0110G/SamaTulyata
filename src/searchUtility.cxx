#include <string.h>

#include "Utility.h"


int searchForVariable(PRESPLUS model, char* name)
{
	int idx ;
	for(idx=0;idx < model.no_of_variables ; idx++)
	{
		if(strcmp(name,model.var_table[idx].name)==0)
		{
			return idx ;
		}
	}
	return -1 ;
}

int searchForPlace(PRESPLUS model, char* name)
{
	int idx;
	for(idx=0; idx < model.no_of_places ; idx++)
	{
		if( strcmp(name,model.places[idx].name) == 0 )
		{
			return idx ;
		}
	}
	return -1 ;
}

int searchForTransition(PRESPLUS model, char* name)
{
	int idx;
	for(idx=0; idx < model.no_of_transitions; idx++)
	{
		if(0 == strcmp(name,model.transitions[idx].name) )
		{
			return idx ;
		}
	} 
	return -1 ;
}
