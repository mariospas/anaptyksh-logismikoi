#include "graph.h"
#include <stdio.h>
#include <stdlib.h>


struct graph
{
	int id;          //panepistimio,atoma,douleia,etc
	ptr_hash table;
};


ptr_graph createGraph(int id,int m,int c)
{
	ptr_graph graph;
	ptr_hash table;

	graph = malloc(sizeof(struct graph));
	table = HT_create(m,c);              //duo int gia orismata
	graph->table = table;
	graph->id = id;

	return graph;

}


int destroyGraph(ptr_graph graph)
{
	int error;

	error = HT_destroy(graph->table);  //int epistrofh h HT_destroy
	free(graph);

	return error;
}


int insertNode(ptr_graph graph,ptr_entry entry)
{
	int error;

	int key;
	hash_f h;
	//na ftiaksoume thn hash function

	error = HT_insert_node(graph->table,h,(void*) entry,key);

	return error;
}


int insertEdge(ptr_graph graph,int id,ptr_edge filos)
{

}
