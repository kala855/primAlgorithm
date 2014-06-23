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

#define INT_MAX2 100
int numNodos = 0;


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

int writeGraphToFile(igraph_t *g, char *fileName){

	FILE *stp;
	stp = fopen(fileName, "w");
	if (stp==0) {
		printf("Problema abriendo archivo de grafo writeGraphToFile\n");
		return EXIT_FAILURE;
	}

	igraph_write_graph_gml(g, stp, 0, "gml Test");
	fclose(stp);

	return EXIT_SUCCESS;

}

int initIgraph(int *m){

	igraph_t g;
	igraph_matrix_t mat;
	long int i, j;

	igraph_matrix_init(&mat, numNodos, numNodos);
	for (i=0; i<numNodos; i++) for (j=0; j<numNodos; j++) MATRIX(mat, i, j) = m[i+numNodos*j];
	igraph_i_set_attribute_table(&igraph_cattribute_table);


	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_UPPER, 0, /*loops=*/ 1);
	//print(&g);

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
	int min = INT_MAX2;
	int i=0,j,fila,columna;
	for (i = 0; i < numNodos; ++i)
		for (j = 0; j < numNodos; ++j)
			stp[i+numNodos*j] = INT_MAX2;

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
		if(i == numNodos){
			vector[columna] = true;
			stp[fila+numNodos*columna] = grafo[fila+numNodos*columna];
			stp[columna+numNodos*fila] = grafo[fila+numNodos*columna];
			i = 0;
			y++;
			if (y == numNodos - 1)
				break;
			min = INT_MAX2;
		}
	}
	for (i = 0; i < numNodos; ++i) {
			for (j = 0; j < numNodos; ++j) {

				if(stp[i+numNodos*j]==INT_MAX2){
					//printf("0,");
					stp[i+numNodos*j] = 0;
				}
				else{
					//printf("%d,",stp[i+numNodos*j]);
				}
			}
			//printf("\n");
		}

	initIgraph(stp);
	free(stp);
	return EXIT_SUCCESS;
}

int* readGrafo(){

	int *grafo;
	igraph_matrix_t gMatrix;
	igraph_t g;
	igraph_i_set_attribute_table(&igraph_cattribute_table);
	FILE *ifile;
	int i,j;
	ifile=fopen("/home/john/Documents/celegansneural.gml"/*"/home/john/git/primAlgorithm/grafo.gml"*/, "r");
	if (ifile==0) {
		printf("Problema abriendo archivo de grafo\n");
		return NULL;
	}
	igraph_read_graph_gml(&g, ifile);

	fclose(ifile);
	numNodos = igraph_vcount(&g);
	grafo = malloc(numNodos*numNodos*sizeof(int));
	igraph_matrix_init(&gMatrix,numNodos,numNodos);
	igraph_get_adjacency(&g,&gMatrix,IGRAPH_GET_ADJACENCY_BOTH,1);

	igraph_vector_t el;
	int ii, jj, n;
	igraph_vector_init(&el, 0);
	igraph_get_edgelist(&g, &el, 0);
	n = igraph_ecount(&g);
	char ch = igraph_is_directed(&g) ? '>' : '-';

	memset(grafo,INT_MAX2,numNodos*numNodos*sizeof(int));

	  for (ii=0, jj=0; ii<n; ii++, jj+=2) {
//	    printf("%ld --%c %ld: %ld\n",
//	      (long)VECTOR(el)[jj], ch, (long)VECTOR(el)[jj+1], (long)EAN(&g, "weight", ii));
	    grafo[((long)VECTOR(el)[jj])+numNodos*((long)VECTOR(el)[jj+1])] = (int)EAN(&g, "weight", ii);
	    grafo[((long)VECTOR(el)[jj+1])+numNodos*((long)VECTOR(el)[jj])] =  (int)EAN(&g, "weight", ii);
	  }

	printf("\nNumero de nodos %d",numNodos);

	igraph_vector_destroy(&el);
	igraph_destroy(&g);
	return grafo;

}

int main(void) {
	int *grafo;
	grafo = readGrafo();
	stpPrim(grafo);
	free(grafo);
	return EXIT_SUCCESS;
}
