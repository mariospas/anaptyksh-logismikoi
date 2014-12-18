#include <math.h>

#include "hash_table.h"
#include "entries.h"
#include "linked_list.h"

#define HASH_CONSTANT ((sqrt(5) - 1) / 2)

typedef struct graph *ptr_graph;

typedef struct
{
	int *array_id;
	int *array_dist;
} ResultSet;

typedef ResultSet *Result_ptr;


/* Interface */
ptr_graph createGraph(int id,int m,int c);

int destroyGraph(ptr_graph graph);

int insertNode(ptr_graph graph,ptr_entry entry,hash_f h);

int insertEdge(ptr_graph graph,int id,ptr_edge filos,hash_f hash);

ptr_entry lookupNode(ptr_graph graph,int id,hash_f hash);

int reachNode1( ptr_graph this, int start, int end );

Result_ptr reachNodeN(ptr_graph graph, int start);

void ResultSet_next(Result_ptr result, int *id, int *distance);

/* Helpers */
int rec_search( ptr_graph this, int start, int end, int level );

int hash(int value, int size);

Properties createProperties(int number) ;

void setStringProperty(char* property, int index, Properties p);

void setIntegerProperty(int property, int index, Properties p);
