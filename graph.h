#include "hash_table.h"
#include "entries.h"
#include "linked_list.h"


typedef struct graph *ptr_graph;

typedef struct
{
	int *pinakas_id;
	int *pinakas_apost;
} ResultSet;



ptr_graph createGraph(int id,int m,int c);

int destroyGraph(ptr_graph graph);

int insertNode(ptr_graph graph,ptr_entry entry,hash_f h);

int insertEdge(ptr_graph graph,int id,ptr_edge filos,hash_f hash);

ptr_entry lookupNode(ptr_graph graph,int id,hash_f hash);

int reachNode1( ptr_graph this, int start, int end,hash_f hash );

int IDS( ptr_graph this, int start, int end, int level,hash_f hash );

ResultSet reachNodeN(ptr_graph graph, int start);

void NEXT(ResultSet result, int *id, int *apostasi);
