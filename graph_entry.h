#ifndef _GRAPH_ENTRY_
#define _GRAPH_ENTRY_

#include "linked_list.h"

#define EDGE_TYPE_BUF 20

struct entry
{
	int id;

    /* Various info about the entity */
	void* properties;

	list_ptr edges;    //deiktis se lista filon
};

struct edge
{
    char edge_type[EDGE_TYPE_BUF];
    int target_type;  /* This should be enum type cast */
	int target_id;
	int weight;
};

typedef struct entry *ptr_entry;

typedef struct edge *ptr_edge;


ptr_entry create_entry(int id,void* properties);    //create node

void destroy_entry(void *entry);

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, int weight );

void destroy_edge(void *edge);

int match_edge(const void *a, const void *key);

int match_entry(void *a,void *key);


void* Entry_take_list(ptr_entry this);

int size_of_friend_list(ptr_entry this);

#endif /* _GRAPH_ENTRY_ */
