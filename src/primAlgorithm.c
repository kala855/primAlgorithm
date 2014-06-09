/*
 ============================================================================
 Name        : primAlgorithm.c
 Author      : John H. Osorio Ríos - Yensy H. Gómez Villegas
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <igraph.h>
#include <stdarg.h>

#define NUMNODOS 6

int printMatrixAdj(int grafo[NUMNODOS][NUMNODOS]){
	int i,j;
	for (i = 0; i < NUMNODOS; ++i) {
		for (j = 0; j < NUMNODOS; ++j) {

			if(grafo[i][j]==INT_MAX){
				printf("0,");
				grafo[i][j] = 0;
			}
			else{
				printf("%d,",grafo[i][j]);
			}
		}
		printf("\n");
	}

	return EXIT_SUCCESS;
}

void print(igraph_t *g) {
  igraph_vector_t el;
  long int i, j, n;
  char ch = igraph_is_directed(g) ? '>' : '-';

  igraph_vector_init(&el, 0);
  igraph_get_edgelist(g, &el, 0);
  n = igraph_ecount(g);

  for (i=0, j=0; i<n; i++, j+=2) {
    printf("%ld --%c %ld: %ld\n",
      (long)VECTOR(el)[j], ch, (long)VECTOR(el)[j+1], (long)EAN(g, "weight", i));
  }
  printf("\n");

  igraph_vector_destroy(&el);
}

int initIgraph(int m[NUMNODOS][NUMNODOS]){

	igraph_t g;
	igraph_matrix_t mat;
	long int i, j;

	igraph_matrix_init(&mat, NUMNODOS, NUMNODOS);
	for (i=0; i<NUMNODOS; i++) for (j=0; j<NUMNODOS; j++) MATRIX(mat, i, j) = m[i][j];
	igraph_i_set_attribute_table(&igraph_cattribute_table);


	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_UPPER, 0, /*loops=*/ 1);
	print(&g);

	FILE *stp;
	stp = fopen("stp.gml", "w");
	if (stp==0) {
	   return EXIT_FAILURE;
	}

	igraph_write_graph_gml(&g, stp, 0, "gml Test");
	fclose(stp);

	igraph_destroy(&g);
	return EXIT_SUCCESS;
}

int stpPrim(int grafo[NUMNODOS][NUMNODOS]){
	int stp[NUMNODOS][NUMNODOS];
	int min = INT_MAX;
	int i=0,j,fila,columna;
	for (i = 0; i < NUMNODOS; ++i)
		for (j = 0; j < NUMNODOS; ++j)
			stp[i][j] = INT_MAX;

	bool vector[NUMNODOS];
	for (i = 0; i < NUMNODOS; ++i) {
		vector[i] = false;
	}
	vector[0] = true;

	i = 0;
	j = 0;
	int y = 0;
	while(true){
		if(vector[i]){
			for (j = 0; j < NUMNODOS; ++j) {
				if(!vector[j]){
					if(grafo[i][j] < min){
						min = grafo[i][j];
						fila = i;
						columna = j;
					}
				}
			}
		}
		i++;
		if(i == NUMNODOS){
			vector[columna] = true;
			stp[fila][columna] = grafo[fila][columna];
			stp[columna][fila] = grafo[fila][columna];
			i = 0;
			y++;
			if (y == NUMNODOS - 1)
				break;
			min = INT_MAX;
		}
	}
	for (i = 0; i < NUMNODOS; ++i) {
			for (j = 0; j < NUMNODOS; ++j) {

				if(stp[i][j]==INT_MAX){
					printf("0,");
					stp[i][j] = 0;
				}
				else{
					printf("%d,",stp[i][j]);
				}
			}
			printf("\n");
		}

	initIgraph(stp);
	return EXIT_SUCCESS;
}

int main(void) {
	int grafo[6][6] = {0,3,1,6,INT_MAX,INT_MAX,3,0,5,INT_MAX,3,INT_MAX,1,5,0,5,6,4,6,INT_MAX,5,0,INT_MAX,2,INT_MAX,3,6,INT_MAX,0,6,INT_MAX,INT_MAX,4,2,6,0};
	//int grafo[NUMNODOS][NUMNODOS] = {{INT_MAX,2,INT_MAX,INT_MAX,INT_MAX},
						//{2,INT_MAX,3,2,10},
						//{INT_MAX,3,INT_MAX,4,INT_MAX},
						//{INT_MAX,2,4,INT_MAX,2},
						//{INT_MAX,10,INT_MAX,2,INT_MAX}};

	//int grafo2[6][6] = {0,3,1,6,0,0,3,0,5,0,3,0,1,5,0,5,6,4,6,0,5,0,0,2,0,3,6,0,0,6,0,0,4,2,6,0};


	//initIgraph(grafo2);
	stpPrim(grafo);

	return EXIT_SUCCESS;
}
