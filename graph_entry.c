#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph_entry.h"
#include "linked_list.h"

ptr_entry create_entry(int id,void* properties, deallocator_f properties_destroy )      //create node
{
	ptr_entry node;

	node = malloc(sizeof(struct entry));

	node->id = id;
	node->properties = properties;
	node->edges = LL_create(match_edge);
	node->properties_destroy = properties_destroy;

	return node;
}

void destroy_entry(void *entry )
{
    ptr_entry entry1 = (ptr_entry) entry;
	if(entry1->properties != NULL)
	{
        entry1->properties_destroy( entry1->properties );
	}
	free(entry1);
}

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, int weight )
{
	ptr_edge new_edge = malloc(sizeof(struct edge));

    strcpy( new_edge->edge_type, edge_type );
	new_edge->target_id = target_id;
	new_edge->target_type = target_type;
	new_edge->weight = 0;

	return new_edge;
}


void destroy_edge(void *edge)
{
	free(edge);
}


int match_edge(const void *a, const void *b)
{
    ptr_edge a1 = (ptr_edge) a;
    ptr_edge b1 = (ptr_edge) b;
    assert( a1 != NULL && b1 != NULL );
    if ( a1->target_type == b1->target_type
      && a1->target_id == b1->target_id
      && a1->weight == b1->weight
      && ( ! strcmp( a1->edge_type, b1->edge_type ) ) ) {
       return 0;
    }
    return -1;
}


int match_entry(void *a,void *key)
{
	if( ((ptr_entry)a)->id == ((ptr_entry)key)->id  )
	{
		return 0;
	}
	else return 1;
}






//Iterators


void* Entry_take_list(ptr_entry this)
{
	return this->edges;
}

int size_of_friend_list(ptr_entry this)
{
	int size = 0;

	size = LL_size(((list_ptr)(this->edges)));

	return size;
}

void print_list_of_edges(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = ((list_ptr) Entry_take_list(node));

	iterList = LL_iter_create(list);

	data = LL_iter_data(iterList);
	printf("Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		printf("Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	}
}
