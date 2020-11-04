#include "Utility.h"

char* retformula(EXPR exp) {
	char* temp;
	int val;
	if(exp->type==1) {
		temp=(char*)calloc(1,sizeof(char));
		temp[0]=exp->value;
	}
	else if(exp->type==2) {
		char temp1[50];
		int n=sprintf(temp1,"%d",exp->value);
		temp=(char*)calloc(n,sizeof(char));
		strcpy(temp,temp1);
	}
	else {
		temp=(char*)calloc(1,sizeof(char));
		val=exp->value;
		switch(val) {
			case 0: {
				temp=(char*)calloc(2,sizeof(char));
				temp[0]='='; temp[1]='=';
			}
				break;
			case 1: temp[0]='&';
				break;
			case 2: {
				temp=(char*)calloc(2,sizeof(char));
				temp[0]='\\'; temp[1]='>';
			}
				break;
			case 3: {
				temp=(char*)calloc(2,sizeof(char));
				temp[0]='\\'; temp[1]='<';
			}
				break;
			case 4: temp[0]='|';
				break;
			case 5: {
				temp=(char*)calloc(3,sizeof(char));
				temp[0]='\\'; temp[1]='>'; temp[2]='=';
			}
				break;
			case 6: {
				temp=(char*)calloc(3,sizeof(char));
				temp[0]='\\'; temp[1]='<'; temp[2]='=';
			}
				break;
			case 7: temp[0]='=';
				break;
			case 8: {
				temp=(char*)calloc(3,sizeof(char));
				temp[0]='\\'; temp[1]='!'; temp[2]='=';
			}
				break;
			case 9: temp[0]='+';
				break;
			case 10: temp[0]='-';
				break;
			case 11: temp[0]='*';
				break;
			case 12: temp[0]='/';
				break;
			case 13: {
				temp=(char*)calloc(3,sizeof(char));
				temp[0]='n'; temp[1]='_'; temp[2]='g';
			}
				break;
			default:
				break;
		}
		if(val!=13) {
			if(exp->left!=NULL)
				temp=strcat(retformula(exp->left),temp);
			if(exp->right!=NULL)
				temp=strcat(temp,retformula(exp->right));
		}
	}
	return temp;
}

void visualize(PRESPLUS model) {
	FILE *fp;
 	char name[100],name1[100];
 	printf(".....Enter filename to save visual representation.......... :\n");
 	scanf("%s", name);
 	strcpy(name1,name);
 	strcat(name,".dot");
	fp = fopen(name, "w");
	fprintf(fp, "digraph %s {\n", name1);
	fprintf(fp, "\tfontname = \"Times-Roman\"; fontsize = 14;\n");
	fprintf(fp, "\tnode [ height = 0.25, width = 0.4, shape = box ];\n\tedge [ color = red ];\n");
	fprintf(fp, "\tnodesep = 0.1; ranksep = 0.5;\n");
	int i,j,k;
	fprintf(fp,"\n");
	for(i=0;i<model.no_of_places;i++) {
		fprintf(fp, "\tnode%s [color = chocolate shape = ellipse, label = \" %s : variable := %s\"];\n",\
			model.places[i].name, model.places[i].name, model.var_table[model.places[i].var_index].name);
	}
	fprintf(fp,"\n");
	for(i=0;i<model.no_of_transitions;i++) {
		fprintf(fp, "\tnode%s [color = blue shape = record, label = \"{ %s | expression := ",\
			model.transitions[i].name, model.transitions[i].name);
		fprintf(fp, "%s", retformula(model.transitions[i].expr));
		if(strcmp("n_g",retformula(model.transitions[i].guard))!=0) {
			fprintf(fp," | condition := %s",retformula(model.transitions[i].guard));
		}
		fprintf(fp, " }\"];\n");
	}
	fprintf(fp,"\n");
	for(i=0;i<model.no_of_transitions;i++) {
		for(j=0;j<model.transitions[i].no_of_preset;j++) {
			fprintf(fp,"\tnode%s -> node%s [label = \" \"];\n", model.places[model.transitions[i].preset[j]].name, model.transitions[i].name);
		}
		for(j=0;j<model.transitions[i].no_of_postset;j++) {
			fprintf(fp,"\tnode%s -> node%s [label = \" \"];\n", model.transitions[i].name, model.places[model.transitions[i].postset[j]].name);
		}
	}
	fprintf(fp, "\tnodestart [color = black shape = ellipse, label = \"Initial Mark\"];\n");
	for(i=0;i<model.no_of_places_initially_marked;i++) {
		fprintf(fp, "\tnodestart -> node%s [color = black label = \" \"];\n", model.places[model.initial_marking[i]].name);
	}
	fprintf(fp,"}\n");
	fclose(fp);
	printf("The PRESPLUS model has been saved in file %s.\nUse \"dotty %s\" for rendering\n",name,name);
}
