#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "graph.h"
#include "hash_table.h"
#include "graph_entry.h"
#include "linked_list.h"

struct graph
{
    /* Type of graph */
	int id;

    /* Assignment */
	ht_ptr table;

    /* Number of nodes */
	int size;
};

ptr_graph createGraph(int id, int num_of_buckets, int size_of_bucket )
{
	ptr_graph graph;

	graph = malloc( sizeof( struct graph ) );
	graph->table = HT_create( num_of_buckets, size_of_bucket, hash );
	graph->id = id;
	graph->size = 0;

	return graph;

}

int destroyGraph(ptr_graph graph )
{
	HT_destroy( graph->table, destroy_entry );
	free( graph );

	return 0;
}

int insertNode( ptr_graph graph, ptr_entry entry )
{
	int key = entry->id;

	HT_insert( graph->table, (void*) entry, key );
	++graph->size;

	return 0;
}


int insertEdge( ptr_graph graph, int id, ptr_edge edge )
{
	printf("Search for id = %d \n",id);
	ptr_entry node = HT_search( graph->table, id );
	printf("in insert edge ID = %d\n",node->id);
	if(node == NULL)
	{
		printf("Entry not Found\n\n");
		return 1;
	}
	else if(node != NULL)
	{
		printf("Normal Insert\n");
		LL_insert( node->edges, (void*) edge );
	}


	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id)
{
	ptr_entry node = NULL;
	node = HT_search(graph->table,id);
	if(node == NULL) printf("NULL in lookupNode\n");
	return node;
}

int reachNode1( ptr_graph this, int start, int end )
{
    int i, result;
    for ( i = 0; i < this->size; ++i )
    {
    	//printf("In reachNode i=%d\n",i);
        if ( rec_search( this, start, end, i ) != 0 )
        {
            return i;
        }
    }
    return -1;
}

int rec_search( ptr_graph this, int start, int end, int level )
{
	//printf("insert in rec_search\n");
    int i, size,list_size, result = 0;
    list_ptr edges;
    ptr_edge edge;
    LL_iter_ptr edge_it;
    if ( level == 0 )
    {
        return start == end;
    }

    edges = ( (ptr_entry) HT_search( this->table, start ) )->edges;

    list_size = LL_size(edges);
    if(list_size == 0) return result;

    edge_it = LL_iter_create( edges );
    for ( i = 0; i < this->size - level; ++i )
    {
    	//printf("rec_search  i = %d start = %d end = %d\n",i,start,end);
        do
        {
            edge = LL_iter_data( edge_it );
            if ( edge->target_type == this->id
              && rec_search( this, edge->target_id, end, level - 1 ) != 0 )
            {
                result = 1;
                break;
            }
        }
        while ( LL_iter_next( edge_it ) );
        LL_iter_reset( edge_it );
    }
    LL_iter_destroy( edge_it );
    return result;
}


ResultSet *reachNodeN( ptr_graph graph, int start )
{
	ResultSet *result = malloc(sizeof(ResultSet));

    /* Initialize only */
    result->graph = graph;
    result->current = HT_iter_create( graph->table );
    result->from = start;

	return result;
}


int ResultSet_next(ResultSet *result, int *id, int *distance )
{
    assert( result != NULL );
    ptr_entry entry = HT_iter_data( result->current );
    *id = entry->id;
    *distance = reachNode1( result->graph, result->from, entry->id );

    /* End condition */
    if ( ! HT_iter_next( result->current ) ) {
        HT_iter_destroy( result->current );
        free( result );
        return 0;
    }
    return 1;
}

size_t hash(int value, size_t size)
{
    return ( size * fmod( ( value * HASH_CONSTANT ), 1 ) );
}

size_t Graph_size( ptr_graph graph )
{
    return graph->size;
}

ht_ptr Graph_nodes( ptr_graph graph )
{
    return graph->table;
}
