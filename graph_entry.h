#ifndef _GRAPH_ENTRY_
#define _GRAPH_ENTRY_

#include "linked_list.h"
#include "dataset_handlers.h"

#define EDGE_TYPE_BUF 100

struct data_of_list
{
	char *special_id;
	list_ptr list;
};


typedef struct data_of_list *ptr_data_list;

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
	double weight;
	void* extra_data;
};

typedef struct entry *ptr_entry;

typedef struct edge *ptr_edge;


ptr_data_list data_list_create(char *special_id);

void destroy_data_of_list(void *data);

int match_data_list(const void* a,const void* special_key);

ptr_entry create_entry(int id, void* properties, deallocator_f destroy_properties );      //create node

void destroy_entry(void *entry);

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, int weight, void *extra );

void destroy_edge(void *edge);

int match_edge(const void *a, const void *key);

int match_entry(const void *a,const void *key);

void edge_change_weight(ptr_edge edge,double weight);


void* Entry_take_list(ptr_entry this);

int size_of_friend_list(ptr_entry this);

int location_of_entry(ptr_entry node);

int last_work_or_study_of_entry(ptr_entry node,char *target_type);

int work_or_study_in_same_place(ptr_entry node1,ptr_entry node2,char *target_type);

void* type_list(ptr_entry node,char *type);

int common_interests_two_entries(ptr_entry node1,ptr_entry node2,int *interest1,int *interest2);

int generation_gap(ptr_entry node1,ptr_entry node2);

int same_gender(ptr_entry node1,ptr_entry node2);

int creator_of_post(ptr_entry node);

int there_is_in_forum_member_list(ptr_entry node,int id_person);

int there_is_in_tag_list(ptr_entry node,int tag_id);

void print_list_of_edges(ptr_entry node);

void print_list(list_ptr list);

#endif /* _GRAPH_ENTRY_ */
