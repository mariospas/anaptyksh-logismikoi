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
	void* lista_idiotiton;    //knows,respect,etc
};

typedef struct entry *ptr_entry;


typedef struct edge *ptr_edge;


ptr_entry create_entry(int id,void* properties,int (*match)( const void *a, const void *b));    //create node

void destroy_entry(void* this);

ptr_edge create_edge(int id, void* lista_idiotiton);

void destroy_edge(void* this);


#endif /* _ENTRIES_ */
