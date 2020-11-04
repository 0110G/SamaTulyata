#include <stdio.h>

#include "Utility.h"

int drivemenu()
{
    int choice;
    /*PRESPLUS model;*/
      
    printf("\n1. Read PRES+ from Keyboard \n");
    printf("2. Read pres+ from a disk resident file \n");
    printf("3. Write pres+ on screen \n");
    printf("4. Write pres+ onto a disk file \n");
    printf("5. ConstructAllPaths using cutpoints\n");
    printf("6. Find Path Equivalence\n");
    printf("7. Put the PRESPLUS model in a DOT file for visualizing\n");
    printf("8. Quit \n");
    printf ("Enter your choice.... : ");
    scanf ("%d", &choice); 
     
    return choice ;
}
 
