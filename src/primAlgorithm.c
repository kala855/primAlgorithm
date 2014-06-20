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
#include <string.h>

#define NUMNODOS 6
int numNodos = 0;

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

int initIgraph(int *m){

	igraph_t g;
	igraph_matrix_t mat;
	long int i, j;

	igraph_matrix_init(&mat, NUMNODOS, NUMNODOS);
	for (i=0; i<NUMNODOS; i++) for (j=0; j<NUMNODOS; j++) MATRIX(mat, i, j) = m[i+numNodos*j];
	igraph_i_set_attribute_table(&igraph_cattribute_table);


	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_UPPER, 0, /*loops=*/ 1);
	print(&g);

	FILE *stp;
	stp = fopen("/home/john/git/primAlgorithm/grafo2.gml", "w");
	if (stp==0) {
		printf("Problema abriendo archivo de grafo\n");
		return EXIT_FAILURE;
	}

	igraph_write_graph_gml(&g, stp, 0, "gml Test");
	fclose(stp);

	igraph_destroy(&g);
	return EXIT_SUCCESS;
}

int stpPrim(int *grafo){


	int *stp;
	stp = malloc(numNodos*numNodos*sizeof(int));
	int min = INT_MAX;
	int i=0,j,fila,columna;
	for (i = 0; i < numNodos; ++i)
		for (j = 0; j < numNodos; ++j)
			stp[i+numNodos*j] = INT_MAX;


	bool *vector;
	vector = malloc(numNodos*sizeof(bool));
	for (i = 0; i < numNodos; ++i) {
		vector[i] = false;
	}
	vector[0] = true;

	i = 0;
	j = 0;
	int y = 0;
	while(true){
		if(vector[i]){
			for (j = 0; j < numNodos; ++j) {
				if(!vector[j]){
					if(grafo[i+numNodos*j] < min){
						min = grafo[i+numNodos*j];
						fila = i;
						columna = j;
					}
				}
			}
		}
		i++;
		if(i == NUMNODOS){
			vector[columna] = true;
			stp[fila+numNodos*columna] = grafo[fila+numNodos*columna];
			stp[columna+numNodos*fila] = grafo[fila+numNodos*columna];
			i = 0;
			y++;
			if (y == NUMNODOS - 1)
				break;
			min = INT_MAX;
		}
	}
	for (i = 0; i < NUMNODOS; ++i) {
			for (j = 0; j < NUMNODOS; ++j) {

				if(stp[i+numNodos*j]==INT_MAX){
					printf("0,");
					stp[i+numNodos*j] = 0;
				}
				else{
					printf("%d,",stp[i+numNodos*j]);
				}
			}
			printf("\n");
		}

	initIgraph(stp);
	return EXIT_SUCCESS;
}

int readGrafo(int *grafo){

	igraph_matrix_t gMatrix;
	igraph_t g;
	igraph_i_set_attribute_table(&igraph_cattribute_table);
	FILE *ifile;
	int i,j;
	ifile=fopen("/home/john/git/primAlgorithm/grafo.gml", "r");
	if (ifile==0) {
		printf("Problema abriendo archivo de grafo\n");
		return EXIT_FAILURE;
	}
	igraph_read_graph_gml(&g, ifile);

	fclose(ifile);
	numNodos = igraph_vcount(&g);
	grafo = malloc(numNodos*numNodos*sizeof(int));
	igraph_matrix_init(&gMatrix,numNodos,numNodos);

	igraph_get_adjacency(&g,&gMatrix,IGRAPH_GET_ADJACENCY_BOTH,1);

	igraph_vector_t el;
	long int ii, jj, n;
	igraph_vector_init(&el, 0);
	igraph_get_edgelist(&g, &el, 0);
	n = igraph_ecount(&g);
	char ch = igraph_is_directed(&g) ? '>' : '-';

	memset(grafo,0,numNodos*numNodos*sizeof(int));

	  for (ii=0, jj=0; ii<n; ii++, jj+=2) {
	    printf("%ld --%c %ld: %ld\n",
	      (long)VECTOR(el)[jj], ch, (long)VECTOR(el)[jj+1], (long)EAN(&g, "weight", ii));
	    grafo[((long)VECTOR(el)[jj])+numNodos*((long)VECTOR(el)[jj+1])] = (int)EAN(&g, "weight", ii);
	    grafo[((long)VECTOR(el)[jj+1])+numNodos*((long)VECTOR(el)[jj])] =  (int)EAN(&g, "weight", ii);
	  }

	for (i = 0; i < numNodos; ++i) {
		for (j = 0; j < numNodos; ++j) {
			printf("%d ",grafo[i+numNodos*j]);
		}
		printf("\n");
	}

	printf("Numero de nodos %d",numNodos);
	igraph_vector_destroy(&el);
	return EXIT_SUCCESS;

}

int main(void) {

	int *grafo;
	grafo = NULL;

	//int grafo[6][6] = {0,3,1,6,INT_MAX,INT_MAX,3,0,5,INT_MAX,3,INT_MAX,1,5,0,5,6,4,6,INT_MAX,5,0,INT_MAX,2,INT_MAX,3,6,INT_MAX,0,6,INT_MAX,INT_MAX,4,2,6,0};
	//int grafo[NUMNODOS][NUMNODOS] = {{INT_MAX,2,INT_MAX,INT_MAX,INT_MAX},
						//{2,INT_MAX,3,2,10},
						//{INT_MAX,3,INT_MAX,4,INT_MAX},
						//{INT_MAX,2,4,INT_MAX,2},
						//{INT_MAX,10,INT_MAX,2,INT_MAX}};


	readGrafo(grafo);
	stpPrim(grafo);

	//grafo = {0,3,1,6,INT_MAX,INT_MAX,3,0,5,INT_MAX,3,INT_MAX,1,5,0,5,6,4,6,INT_MAX,5,0,INT_MAX,2,INT_MAX,3,6,INT_MAX,0,6,INT_MAX,INT_MAX,4,2,6,0};


	//int grafo2[6][6] = {0,3,1,6,0,0,3,0,5,0,3,0,1,5,0,5,6,4,6,0,5,0,0,2,0,3,6,0,0,6,0,0,4,2,6,0};


	//initIgraph(grafo2);
	//stpPrim(grafo);

	free(grafo);
	return EXIT_SUCCESS;
}
