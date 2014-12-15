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

struct properties {
    int num_of_prop;
    char* name;
    char* surname;
    int age;
    char* type;
    int weight;
};
typedef struct properties *Properties;


/* Interface */
ptr_graph createGraph(int id,int m,int c);

int destroyGraph(ptr_graph graph);

int insertNode(ptr_graph graph,ptr_entry entry);

int insertEdge(ptr_graph graph,int id,ptr_edge filos);

ptr_entry lookupNode(ptr_graph graph,int id);

int reachNode1( ptr_graph this, int start, int end );

ResultSet reachNodeN(ptr_graph graph, int start);

void ResultSet_next(ResultSet result, int *id, int *distance);

double closeness_centrality( ptr_entry n, ptr_graph g );

double betweenness_centrality( ptr_entry n, ptr_graph g );

/* Helpers */
int rec_search( ptr_graph this, int start, int end, int level );

size_t hash(int value, size_t size);

Properties createProperties(int number) ;

void setStringProperty(char* property, int index, Properties p);

void setIntegerProperty(int property, int index, Properties p);

