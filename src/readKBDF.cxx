#include "Utility.h"

PRESPLUS readKBDF()
{
  
  PRESPLUS model;

  model = readplace (model); 
  // reads places and the variables associated with them
	printf("Hello World\n");  
	model = readtransition (model);
  model = readInitialMarking (model) ;
  
  model = connectSetPlaces (model) ;

  return model;
} 
