#ifndef _GRAPH_
#define _GRAPH_

#include <math.h>

#include "hash_table.h"
#include "linked_list.h"
#include "graph_entry.h"

#define GRAPH_ID_SIZE 50
#define HASH_CONSTANT ((sqrt(5) - 1) / 2)

typedef struct graph *ptr_graph;

typedef struct
{
	int *array_id;
	int *array_dist;
} ResultSet;
typedef ResultSet *Result_ptr;

/* Interface */
ptr_graph createGraph(char id[GRAPH_ID_SIZE], int num_of_buckets, int size_of_bucket);

int destroyGraph(ptr_graph graph);

int insertNode(ptr_graph graph,ptr_entry entry);

int insertEdge(ptr_graph graph,int id,ptr_edge filos);

ptr_entry lookupNode(ptr_graph graph,int id);

int reachNode1( ptr_graph this, int start, int end );

Result_ptr reachNodeN(ptr_graph graph, int start);

void ResultSet_next(Result_ptr result, int *id, int *distance);

size_t Graph_size( ptr_graph graph );

ht_ptr Graph_nodes( ptr_graph graph );

double closeness_centrality( ptr_entry n, ptr_graph g );

double betweenness_centrality( ptr_entry n, ptr_graph g );

/* Helpers */
static int rec_search( ptr_graph this, int start, int end, int level );

static size_t hash(int value, size_t size);

#endif /* _GRAPH_ */
