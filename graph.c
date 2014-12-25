#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "hash_table.h"
#include "graph_entry.h"
#include "linked_list.h"

struct graph
{
    /* Type of graph */
	char id[GRAPH_ID_SIZE];

    /* Assignment */
	ht_ptr table;

    /* Number of nodes */
	int size;
};

ptr_graph createGraph(char id[GRAPH_ID_SIZE], int num_of_buckets, int size_of_bucket)
{
	ptr_graph graph;

	graph = malloc( sizeof( struct graph ) );
	graph->table = HT_create( num_of_buckets, size_of_bucket, hash );
	strcpy(graph->id,id);
	//graph->id = id;
	graph->size = 0;

	return graph;

}

int destroyGraph( ptr_graph graph )
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


int insertEdge( ptr_graph graph, int id, ptr_edge friend )
{
	printf("*****InsertEdge %d to Entry %d\n",friend->id,id);
	ptr_entry node = HT_search( graph->table, id );
	if(node == NULL)
	{
		printf("Entry not Found\n");
		return 1;
	}
	else if(node != NULL)
	{
		LL_insert( node->friends, (void*) friend );
	}


	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id)
{
	ptr_entry node = HT_search(graph->table,id);
	return node;
}

int reachNode1( ptr_graph this, int start, int end )
{
    int i, result;
    for ( i = 0; i < this->size; ++i )
    {
        if ( rec_search( this, start, end, i ) != 0 )
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

    edges = ( (ptr_entry) HT_search( this->table, start ) )->friends;
    edge_it = LL_iter_create( edges );
    for ( i = 0; i < this->size - level; ++i )
    {
        do
        {
            edge = LL_iter_data( edge_it );
            if ( rec_search( this, edge->id, end, level - 1 ) != 0 )
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
	int dist, count = 0;
    HT_iter_ptr ht_it = HT_iter_create( graph->table );
    ptr_entry entry;

	result->array_id = malloc(graph->size * sizeof(int));
	result->array_dist = malloc(graph->size * sizeof(int));
    do
    {
        entry = HT_iter_data( ht_it );
        dist = reachNode1( graph, start, ( (ptr_entry) entry )->id );
        result->array_dist[count] = dist;
        result->array_id[count] = ( (ptr_entry) entry )->id;
        count++;
    }
    while ( HT_iter_next( ht_it ) );
    HT_iter_destroy( ht_it );
	return result;
}


void ResultSet_next(ResultSet *result, int *id, int *distance )
{
    /* Initialized to 0 */
	static int i;

    /* Reset condition */
    if ( result == NULL ) {
        i = 0;
        return;
    }

	*id = result->array_id[i];
	*distance = result->array_dist[i];
	i++;
}

size_t hash(int value, size_t size)
{
    return ( size * fmod( ( value * HASH_CONSTANT ), 1 ) );
}

double closeness_centrality( ptr_entry n, ptr_graph g )
{
    int sumdist = 0;
    ptr_entry entry;
    HT_iter_ptr inode = HT_iter_create( g->table );

    do {
        entry = HT_iter_data( inode );
        sumdist += reachNode1( g, n->id, entry->id );
    } while ( HT_iter_next( inode ) );
    HT_iter_destroy( inode );

    return sumdist / ( (double) g->size );
}

double betweenness_centrality( ptr_entry n, ptr_graph g )
{
    int size = g->size, alldist, betweendist;
    int id, dist, i;
    double ret = 0.0;
    HT_iter_ptr node_it = HT_iter_create( g->table );
    ptr_entry node;
    ResultSet *set;

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        set = reachNodeN( g, node->id );
        alldist = 0;
        betweendist = 0;

        /* Check every shortest path */
        for ( i = 0; i < size; ++i ) {
            ResultSet_next( set, &id, &dist );
            if ( dist <= 0 ) {
                continue;
            }

            ++alldist;
            if ( dist == ( reachNode1( g, node->id, n->id ) + reachNode1( g, n->id, id ) ) ) {
                ++betweendist;
            }
        }

        ret += (double) betweendist / alldist;
        free( set );
    } while ( HT_iter_next( node_it ) );
    HT_iter_destroy( node_it );

    return ( 2 * ret ) / ( (double) ( g->size - 1 ) * ( g->size - 2 ) );
}

size_t Graph_size( ptr_graph graph )
{
    return graph->size;
}

ht_ptr Graph_nodes( ptr_graph graph )
{
    return graph->table;
}
