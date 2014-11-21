#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hash_table.h"
#include "entries.h"
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

ptr_graph createGraph(int id, int num_of_buckets, int size_of_bucket)
{
	ptr_graph graph;

	graph = malloc( sizeof( struct graph ) );
	graph->table = HT_create( num_of_buckets, size_of_bucket );
	graph->id = id;
	graph->size = 0;

	return graph;

}

int destroyGraph( ptr_graph graph )
{
	HT_destroy( graph->table, destroy_entry );
	free( graph );

	return 0;
}

int insertNode( ptr_graph graph, ptr_entry entry, hash_f h )
{
	int key;

	HT_insert( graph->table, h, (void*) entry, key );
	++graph->size;

	return 0;
}


int insertEdge( ptr_graph graph, int id, ptr_edge friend, hash_f hash )
{
	ptr_entry node = HT_search( graph->table, id, hash );
	LL_insert( node->friends, (void*) friend );

	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id, hash_f hash)
{
	ptr_entry node = HT_search(graph->table,id,hash);
	return node;
}

int reachNode1( ptr_graph this, int start, int end )
{
    int i, result;
    for ( i = 0; i < this->size; ++i )
    {
        if ( IDS( this, start, end, i ) != 0 )
        {
            return i;
        }
    }
    return -1;
}

int rec_search( ptr_graph this, int start, int end, int level )
{
    int i, size, result = 0;
    list_ptr edges;
    ptr_edge edge;
    LL_iter_ptr edge_it;
    if ( level == 0 )
    {
        return start == end;
    }

    edges = ( (ptr_entry) HT_search( this->table, start, hash ) )->friends;
    edge_it = LL_iter_create( edges );
    for ( i = 0; i < this->size - level; ++i )
    {
        do
        {
            edge = LL_iter_data( edge_it );
            if ( IDS( this, edge->id, end, level - 1 ) != 0 )
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


ResultSet reachNodeN( ptr_graph graph, int start )
{
	ResultSet result;
	int dist, count = 0;
    HT_iter_ptr ht_it = HT_iter_create( graph->table );
    ptr_entry entry;

	result.array_id = malloc(graph->size * sizeof(int));
	result.array_dist = malloc(graph->size * sizeof(int));
    do
    {
        entry = HT_iter_data( ht_it );
        dist = reachNode1( graph, start, ( (ptr_entry) entry )->id );
        result.array_dist[count] = dist;
        result.array_id[count] = ( (ptr_entry) entry )->id;
        count++;
    }
    while ( HT_iter_next( ht_it ) );
    HT_iter_destroy( ht_it );
	return result;
}


void ResultSet_next(ResultSet result, int *id, int *distance)
{
    /* Initialized to 0 */
	static int i;

	*id = result.array_id[i];
	*distance = result.array_dist[i];
	i++;
}

int hash(int value, int size)
{
    return ( size * fmod( ( value * HASH_CONSTANT ), 1 ) );
}
