#ifndef _ENTRIES_
#define _ENTRIES_

#include "linked_list.h"

struct entry
{
	int id;
	void* properties;
	void* friends;    //deiktis se lista filon
};

struct edge
{
	int id;    //id filou
	int weight;
};

typedef struct entry *ptr_entry;


typedef struct edge *ptr_edge;


ptr_entry create_entry(int id,void* properties);    //create node

void destroy_entry(void* this);

ptr_edge create_edge(int id, void* lista_idiotiton);

void destroy_edge(void* this);

int match_edge(const void *a, const void *key);

int match_entry(void *a,void *key);


//Iterators

void* Entry_take_list(ptr_entry this);

int size_of_friend_list(ptr_entry this);

#endif /* _ENTRIES_ */
