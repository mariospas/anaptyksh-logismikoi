#include "hash_table.h"


typedef struct graph *ptr_graph;



ptr_graph createGraph(int id,int m,int c);

int destroyGraph(ptr_graph graph);

int insertNode(ptr_graph graph,ptr_entry entry,hash_f h);

int insertEdge(ptr_graph graph,int id,ptr_edge filos,hash_f hash);

ptr_entry lookupNode(ptr_graph graph,int id,hash_f hash);
