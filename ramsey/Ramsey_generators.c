#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

//#define SBP

#define BLUECLAUSES
//#define MAXRED
//#define MAXDEGREE
//#define RESTRICTOVERLAP

int nodes;
int edge_stamp = 0;
int *edge_stamps;
int clause[100];
int red, blue;

int var  (int a, int b) {
  int i, tmp, min, max;

  if( a > b ) {
    min = b;
    max = a;
  }
  else {
    max = b;
    min = a;
  }

  tmp = 0;
  for (i = 0; i < min; i++)
    tmp += nodes - i - 1;

  return tmp + max - min;
}

int getImage(int in, int swapfrom, int swapto){
    if(in==swapfrom){
        return swapto;
    }else if(in==swapto){
        return swapfrom;
    }else{
        return in;
    }
}

void printGeneratorForDoubleNodeSwap(int i, int j, int k, int l){
    int s; int t;
    for(s=0; s<nodes; ++s){ // loop over all variables
        for(t=s+1; t<nodes; ++t){
            int varfrom = var(s,t);
            int varto = var(getImage(getImage(s,i,j),k,l),getImage(getImage(t,i,j),k,l));
            if(varfrom!=varto){
                printf("( %i %i ) ( %i %i ) ",varfrom,varto,-varfrom,-varto);
            }
        }
    }
    printf("\n");
}

void printGeneratorForNodeSwap(int i, int j){
    printGeneratorForDoubleNodeSwap(i,j,0,0);
}

void printLinearGenerators(){
    int i;
    for(i=0; i<nodes-1; ++i){
        printGeneratorForNodeSwap(i,i+1);
    }
}

void printQuadraticGenerators(){
    int i, j;
    for(i=0; i<nodes; ++i){
        for(j=i+1; j<nodes; ++j){
            printGeneratorForNodeSwap(i,j);
        }
    }
}

void printDoublyQuadraticGenerators(){
    int i, j, k, l;
    for(i=0; i<nodes; ++i){
        for(j=i+1; j<nodes; ++j){
            for(k=i+1; k<nodes; ++k){
                for(l=k+1; l<nodes; ++l){
                    if(k!=j && l!=j){
                        printGeneratorForDoubleNodeSwap(i,j,k,l);
                    }
                }
            }
        }
    }
}

int main (int argc , char ** argv ) {
	int i, j, h, k, x, tmp, next;
	int vars, edges, clauses;

        if (argc <= 4) {
          printf("c run using ./Ramsey_generators REDCLIQUE BLUECLIQUE NODES AMOUNT\n");
          exit (0);
        }

    // AMOUNT == 0: only color symmetry
    // AMOUNT == 1: linear
    // AMOUNT == 2: quadratic
    // AMOUNT == 3: doubly quadratic

    red   = atoi(argv[1]);
 	blue  = atoi(argv[2]);
	nodes = atoi(argv[3]);
    int quadratic = atoi(argv[4]);

	edges = (nodes * (nodes - 1)) / 2;
    vars  = edges;  

    if(quadratic==1){
        printLinearGenerators();
    }else if(quadratic>1){
        printQuadraticGenerators();
        if(quadratic>2){
            printDoublyQuadraticGenerators();
        }
    }

// print color generator    
    for(h=0; h<nodes; ++h){ // loop over all variables
        for(k=h+1; k<nodes; ++k){
            int varfrom = var(h,k);
            int varto = -varfrom;
            printf("( %i %i ) ",varfrom,varto);
        }
    }
    printf("\n");  
    
}
