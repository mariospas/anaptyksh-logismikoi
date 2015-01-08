#ifndef _GRAPH_ENTRY_
#define _GRAPH_ENTRY_

#include "linked_list.h"

#define EDGE_TYPE_BUF 100

typedef void (*deallocator_f)(void *obj);

struct entry
{
	int id;

    /* Various info about the entity */
	void* properties;

	list_ptr edges;    //deiktis se lista filon

    /* Function to handle properties destruction */
    deallocator_f properties_destroy;
};

struct edge
{
    char edge_type[EDGE_TYPE_BUF];
    int target_type;  /* This should be enum type cast */
	int target_id;
	int weight;
	void* extra_data;
};

typedef struct entry *ptr_entry;

typedef struct edge *ptr_edge;


ptr_entry create_entry(int id, void* properties, deallocator_f destroy_properties );      //create node

void destroy_entry(void *entry);

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, int weight, void *extra );

void destroy_edge(void *edge);

int match_edge(const void *a, const void *key);

int match_entry(void *a,void *key);


void* Entry_take_list(ptr_entry this);

int size_of_friend_list(ptr_entry this);

int location_of_entry(ptr_entry node);

int last_work_or_study_of_entry(ptr_entry node,char *target_type);

list_ptr interest_list(ptr_entry node);

int common_interests_two_entries(ptr_entry node1,ptr_entry node2);

void print_list_of_edges(ptr_entry node);

#endif /* _GRAPH_ENTRY_ */
