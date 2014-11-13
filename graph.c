#include "graph.h"
#include <stdio.h>
#include <stdlib.h>


struct graph
{
	int id;          //panepistimio,atoma,douleia,etc
	ht_ptr table;
};


ptr_graph createGraph(int id,int m,int c)
{
	ptr_graph graph;
	ht_ptr table;

	graph = malloc(sizeof(struct graph));
	table = HT_create(m,c);              //duo int gia orismata
	graph->table = table;
	graph->id = id;

	return graph;

}


int destroyGraph(ptr_graph graph)
{
	int error;

	error = HT_destroy(graph->table,destroy_entry);  //int epistrofh h HT_destroy
	free(graph);

	return error;
}


int insertNode(ptr_graph graph,ptr_entry entry,hash_f h)
{
	int error;

	int key;

	error = HT_insert_node(graph->table,h,(void*) entry,key);

	return error;
}


int insertEdge(ptr_graph graph,int id,ptr_edge filos,hash_f hash)
{
	ptr_entry node;
	void *temp;

	temp = HT_search(graph->table,id,hash);
	node = (ptr_entry) temp;

	LL_insert((list_ptr)node->friends,(void*)filos);

	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id,hash_f hash)
{
	ptr_entry node;
	void *temp;

	temp = HT_search(graph->table,id,hash);
	node = (ptr_entry) temp;

	return node;
}




