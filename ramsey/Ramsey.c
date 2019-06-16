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

int check_stamp( int value ) {
  if( edge_stamps[ value ] == edge_stamp ) return 0;
  edge_stamps[ value ] = edge_stamp;
  return value;
}

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

void printClause (int size) {
  int i;

  for (i = 0; i < size; i++)
    printf("%i ", clause[i]);
  printf("0\n");
}

void genClauseRec (int pos, int neg, int size, int max, int from, int to) {
  if (pos ==  red) return;
  if (neg == blue) return;
  if (size == max) {
    printClause (max);
    return;
  }

  int edge = var(from, to);
  clause[ size ] = edge;
  genClauseRec (pos + 1, neg, size + 1, max, from + 1, to);

  clause[ size ] = -edge - 1;
  genClauseRec (pos, neg + 1, size + 1, max, from +1, to);
}

int edge (int a, int b ) {
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

  return check_stamp (tmp + max - min);
}

int fact ( int base, int tmp, int stop ) {
  if( base == stop ) return tmp;
  return fact( base - 1, tmp * base, stop );
}

int main (int argc , char ** argv ) {
	int i, j, h, k, x, tmp, next;
	int vars, edges, clauses;

        if (argc <= 3) {
          printf("c run using ./Ramsey REDCLIQUE BLUECLIQUE NODES\n");
          exit (0);
        }

        red   = atoi(argv[1]);
 	blue  = atoi(argv[2]);
	nodes = atoi(argv[3]);

	edges = (nodes * (nodes - 1)) / 2;
        vars  = edges;
#ifdef SBP
//        vars += (nodes - 1) * (nodes - blue) * (blue - 1);
#endif
#ifdef MAXRED
       vars += (nodes - 1) * (nodes - 2) * (nodes - 2) / 2;
#endif
	clauses  = fact(nodes, 1, nodes -  red) / fact( red, 1, 1);
	clauses += fact(nodes, 1, nodes - blue) / fact(blue, 1, 1);

#ifdef SBP
        clauses += (nodes - 1) * (nodes - 2) / 2;
        clauses += (nodes - 2) * (nodes - 3) / 2;
#endif
	printf("p cnf %i %i\n", vars, clauses);


#ifdef SBP

/*********** FIX A CLIQUE OF MAX DEGREE ***********/

  next = edges;
#ifdef MAXRED
  for (i = 0; i < nodes; i++)
    for (j = i + 1; j < nodes; j++) {
      tmp = next;
      for (k = 0; k < nodes; k++)
        if (k != i && k != j) {
          next++;
          printf("-%i %i %i 0\n", next, var(i,k), var(j,k));
          printf("%i -%i 0\n", next, var(i,k));
          printf("%i -%i 0\n", next, var(j,k));
        }

      for (k = 1; k < nodes - 1; k++)
        printf("-%i ", tmp + k);
      printf("-%i 0\n", var (i,j));
  }
#endif
#ifdef RESTRICTOVERLAP
  for (i = 0; i < nodes; i++)
    for (j = i + 1; j < nodes; j++) {
      tmp = next;
      for (k = 0; k < nodes; k++)
        if (k != i && k != j) {
          next++;
          printf("%i %i 0\n", next, var(i,k));
          printf("%i -%i 0\n", next, var(j,k));
          printf("-%i -%i %i 0\n", next, var(i,k), var(j,k));

//          printf("%i  %i  %i 0\n", next, var(i,k), var(j,k));
//          printf("%i -%i -%i 0\n", next, var(i,k), var(j,k));
//          printf("-%i -%i %i 0\n", next, var(i,k), var(j,k));
//          printf("-%i %i -%i 0\n", next, var(i,k), var(j,k));
        }
/*
      next++;
      printf("-%i %i 0\n", next, next + nodes - 4);
      printf("-%i %i 0\n", tmp + 1, next);
      for (k = 2; k < nodes - 3; k++) {
        printf("-%i -%i %i 0\n", tmp + k, next, next + 1);
        next++;
        printf("-%i %i 0\n", next, next + nodes - 4);
      }
      printf("-%i -%i 0\n", tmp + nodes - 3, next);

      next++;
      printf("-%i %i 0\n", tmp + 2, next);
      for (k = 3; k < nodes - 2; k++) {
        printf("-%i -%i %i 0\n", tmp + k, next, next + 1);
        next++; }
      printf("-%i -%i 0\n", tmp + nodes - 2, next);
*/
      for (k = 1; k < nodes - 1; k++)
        printf("-%i ", tmp + k);
      printf("0\n");

  }
#endif
  for (i = 0; i < blue - 1; i++)
    printf("-%i 0\n", var(i, nodes - 1));

  for (i = 1; i < blue - 2; i++)
    printf("-%i %i 0\n", var(i, nodes - 2), var(i + 1, nodes - 2));

  for (i = 2; i < blue - 2; i++)
    printf("%i -%i %i 0\n", var(i + 1, nodes - 2), var(i, nodes - 3), var(i + 1, nodes - 3));

  for (i = 2; i < blue - 2; i++)
    printf("-%i -%i %i 0\n", var(i, nodes - 2), var(i, nodes - 3), var(i + 1, nodes - 3));

  for (i = 3; i < blue - 2; i++)
    printf("%i %i -%i %i 0\n", var(i + 1, nodes - 2), var(i + 1, nodes - 3), var(i, nodes - 4), var(i + 1, nodes - 4));

  for (i = 3; i < blue - 2; i++)
    printf("-%i %i -%i %i 0\n", var(i, nodes - 2), var(i + 1, nodes - 3), var(i, nodes - 4), var(i + 1, nodes - 4));

  for (i = 3; i < blue - 2; i++)
    printf("%i -%i -%i %i 0\n", var(i + 1, nodes - 2), var(i, nodes - 3), var(i, nodes - 4), var(i + 1, nodes - 4));

  for (i = 3; i < blue - 2; i++)
    printf("-%i -%i -%i %i 0\n", var(i, nodes - 2), var(i, nodes - 3), var(i, nodes - 4), var(i + 1, nodes - 4));


/******* CLIQUE OLD-STYLE *************

  for (i = 0; i < blue - 2; i++)
    printf("%i 0\n", var(i, blue - 1));

  for (i = 0; i < blue - 1; i++)
    for (j = i + 1; j < blue - 1; j++)
      printf("%i %i 0\n", var(i, blue), var (j, blue));
*/

#ifdef MAXDEGREE
        tmp = next;
        for (i = 0; i < nodes; i++) {
          for (k = 0; k < blue - 1; k++)
            for (j = 1; j < nodes - blue; j++)
              printf("-%i %i 0\n", tmp + k * (nodes - blue) + j, tmp + k * (nodes - blue) + j + 1);

          for (j = 0; j < nodes - blue; j++) {
            x = j + (j >= i);
            printf("%i %i 0\n", var(i, x), tmp + j + 1);
            if (j != 0) printf("%i ", tmp + j);
            printf("-%i -%i 0\n", var(i, x), tmp + j + 1);
          }

          for (j = 0; j < nodes - blue; j++)
            for (k = 1; k < blue - 1; k++) {
               x = j + k + ((j + k) >= i);
               printf("%i -%i %i 0\n", var(i, x), tmp + (k-1) * (nodes - blue) + j + 1, tmp + k * (nodes - blue) + j + 1);
               if (j != 0) printf("%i ", tmp + k * (nodes - blue) + j);
               printf("-%i -%i 0\n", var(i, x), tmp + k * (nodes - blue) + j + 1);
               printf("%i -%i 0\n", tmp + (k-1) * (nodes - blue) + j + 1, tmp + k * (nodes - blue) + j + 1);
            }

          for (j = 0; j < nodes - blue; j++)
            for (k = 1; k < blue - 1; k++) {
               x = j + k + ((j + k) >= i);
               printf("%i -%i %i 0\n", var(i, x), tmp + (k-1) * (nodes - blue) + j + 1, tmp + k * (nodes - blue) + j + 1);
               if (j != 0) printf("%i ", tmp + k * (nodes - blue) + j);
               printf("-%i -%i 0\n", var(i, x), tmp + k * (nodes - blue) + j + 1);
               printf("%i -%i 0\n", tmp + (k-1) * (nodes - blue) + j + 1, tmp + k * (nodes - blue) + j + 1);
            }

          for (j = 0; j < nodes - blue; j++) {
            x = j + blue - 1 + ((j + blue - 1) >= i);
            printf("%i -%i 0\n", var(i, x), tmp + (nodes - blue) * (blue - 2) + j + 1);
          }

          tmp += (nodes - blue) * (blue - 1);
        }
#endif
/*
        tmp = next;
        int diff = blue - 1;
        for (k = 0; k < blue - 1; k++)
          for (j = 1; j < nodes - k - 1; j++)
            diff++;

        for (i = 1; i < nodes; i++) {
          next = tmp;
          for (k = 0; k < blue - 1; k++) {
            for (j = 1; j < nodes - k - 1; j++) {
              next++;
              printf("-%i %i 0\n", next, next + 1 );
            }
            next++;
            if (i == 1) {
              if (k != 0) printf("%i ", nodes - k);
              printf("-%i -%i 0\n", nodes - k - 1, next);
            }
            else if (i < blue - 1)
              printf("%i -%i 0\n", next - diff, next);
          }

          int a = tmp;
          for (j = 0; j < nodes - 1; j++) {
            a++;
            x = j + (j >= i);
            printf("%i %i 0\n", var(i, x), a );
            if (j != 0) printf("%i ", a - 1);
            printf("-%i -%i 0\n", var(i, x), a);
          }

          for (k = 1; k < blue - 1; k++)
            for (j = 0; j < nodes - k - 1; j++) {
               a++;
               x = j + k + ((j + k) >= i);
               printf("%i -%i %i 0\n", var(i, x), a - nodes + k, a);

               if (j != 0) printf("%i ", a - 1);
               printf("-%i -%i 0\n", var(i, x), a);
               printf("%i -%i 0\n", a - nodes + k, a);
            }

          a -= nodes - blue;
          for (j = 0; j < nodes - blue; j++) {
            x = j + blue - 1 + ((j + blue - 1) >= i);
            printf("%i -%i 0\n", var(i, x), a++);
          }

          tmp = next;
        }
*/

  for (i = 0; i < red + blue - 1; i++)
    for (j = i + 1; j < nodes - 1; j++) {
       clause[ 0 ] =  var(i, j    );
       clause[ 1 ] = -var(i, j + 1);
       genClauseRec (0, 0, 2, i + 2, 0, j);
    }

#endif
	edge_stamps = (int*) malloc( sizeof(int) * (edges+1) );
	for( i = 1; i <= edges; i++ )
	   edge_stamps[ i ] = 0;

	int red_c[ red ];

	for( i = 0; i < red; i++ )
	    red_c[ i ] = i;

	do {
	    edge_stamp++;

	    for( i = 0; i < red; i++ )
	        for( j = i+1; j < red; j++ ) {
		    h = edge(red_c[ i ], red_c[ j ]);
		    if( h > 0 ) printf("%i ", h );
		}

	    printf("0\n");

	    for( i = red - 1; i >= 0; i-- ) {
		if( red_c[ i ] < nodes + i - red )
		{
		    red_c[ i ]++;

		    for( j = i + 1; j < red; j++ )
			red_c[ j ] = red_c[j - 1] + 1;

		    break;
		}

  	    }
	}
	while( i >= 0 );

#ifdef BLUECLAUSES
	int blue_c[ blue ];
	for( i = 0; i < blue; i++ )
	    blue_c[ i ] = i;

	do {
	  edge_stamp++;

	  for( i = 0; i < blue; i++ )
	    for( j = i+1; j < blue; j++ ) {
              h = edge(blue_c[ i ], blue_c[ j ]);
              if( h > 0 ) printf("-%i ", h );
	    }

	  printf("0\n");

	  for (i = blue - 1; i >= 0; i--)
	    if (blue_c[ i ] < nodes + i - blue) {
	      blue_c[ i ]++;

	      for (j = i + 1; j < blue; j++)
		blue_c[ j ] = blue_c[ j - 1] + 1;

	      break;
	    }
	}
	while (i >= 0);
#endif
}
