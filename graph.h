#ifndef _GRAPH_
#define _GRAPH_

#include <math.h>

#include "hash_table.h"
#include "linked_list.h"
#include "graph_entry.h"
#include "dataset_handlers.h"
#include <stdio.h>

#define GRAPH_ID_SIZE 50
#define HASH_CONSTANT ((sqrt(5) - 1) / 2)
#define MAX_STR_LEN 1024
#define MAX_NAME 50
#define LOCATION_IP 25
#define BROWSER 25

typedef struct graph *ptr_graph;

typedef struct
{
    ptr_graph graph;
    HT_iter_ptr current;
    int from;
} ResultSet;
typedef ResultSet *Result_ptr;

/* Interface */
ptr_graph createGraph(int id, int num_of_buckets, int size_of_bucket);

int destroyGraph(ptr_graph graph );

int insertNode(ptr_graph graph,ptr_entry entry);

int insertEdge( ptr_graph graph, int id, ptr_edge edge);

ptr_entry lookupNode(ptr_graph graph,int id);

int reachNode1( ptr_graph this, long start, long end );

Result_ptr reachNodeN(ptr_graph graph, int start);

int ResultSet_next(Result_ptr result, int *id, int *distance);

size_t Graph_size( ptr_graph graph );

ht_ptr Graph_nodes( ptr_graph graph );

void load_graph(ptr_graph graph);

void load_2ids(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type);

void load_2ids_and_extra(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type,int choice);

ptr_date load_date(char* buf,int i);

/* Helpers */
//static int rec_search( ptr_graph this, int start, int end, int level );

static int expand_( ptr_graph this, ht_ptr *frontier, ht_ptr visited, int *level );

static int expand_reversed_( ptr_graph this, ht_ptr *frontier, ht_ptr visited, int *level );

static int match_found_( ht_ptr frontier1, ht_ptr frontier2 );


static size_t hash(int value, size_t size);

void print_graph(ptr_graph graph);

#endif /* _GRAPH_ */
