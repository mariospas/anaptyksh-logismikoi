#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph.h"
#include "graph_entry.h"
#include "database.h"
#include "girvan_newman.h"

struct edge_measuring {
    int from;
    int to;
    int measure;
};

static int edge_betweenness( ptr_graph g, int from, int to );
static int calculate_modularity( ptr_graph g );
static int detect_edge( const void *a, const void *b );
static int compare_edges( const void *a, const void *b );
static struct Communities *construct_communities( ptr_graph g );

struct Communities *girvan_newman_method( double limit, ptr_graph g )
{
    struct edge_measuring **assignment, *edge_info, temp;
    list_ptr initial, edges;
    HT_iter_ptr node_it;
    LL_iter_ptr edge_it;
    ptr_entry node;
    ptr_edge edge;
    struct edge sample;
    int i;
    size_t size;
    double modularity;
    struct Communities *retval;

/* Construct the initial values */
    node_it = HT_iter_create( Graph_nodes(g) );
    initial = LL_create( detect_edge );

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        edges = type_list( node, "person_knoes_person.csv" );
        edge_it = LL_iter_create( edges );

        /* For each edge of the node */
        do {
            edge = (ptr_edge) LL_iter_data( edge_it );
            temp.from = node->id;
            temp.to = edge->target_id;
            if ( LL_search( initial, &temp ) ) {
                /* Ignore duplicate edges ( assuming graph is not oriented ) */
                continue;
            }
            edge_info = malloc( sizeof(struct edge_measuring) );
            memcpy( edge_info, &temp, sizeof(struct edge_measuring) );
            LL_insert( initial, edge_info );
        } while ( LL_iter_next( edge_it ) );
        LL_iter_destroy( edge_it );
    } while ( HT_iter_next( node_it ) );
    HT_iter_destroy( node_it );

/* Do an initial calculation of the betweenness for each edge */
    edge_it = LL_iter_create( initial );
    do {
        edge_info = LL_iter_data( edge_it );
        edge_info->measure = edge_betweenness( g, edge_info->from, edge_info->to );
    } while ( LL_iter_next( edge_it ) );
    assignment = (struct edge_measuring**) LL_export( initial );
    size = LL_size( initial );
    LL_destroy( initial, NULL );

/* Start the loop */
    sample.target_type = PERSON;
    sample.weight = 0;
    strcpy( sample.edge_type, "person_knows_person.csv" );
    do {
        /* Remove the most central edge */
        qsort( assignment, size, sizeof(struct edge_measuring*), compare_edges );
        edge_info = *assignment;
        sample.target_id = edge_info->to;
        node = lookupNode( g, edge_info->from );
        edges = type_list( node, "person_knows_person.csv" );
        edge = LL_search( edges, &sample );
        edge->weight = -1;  /* a non painful way to mark it removed */

        /* Calculate again the betweenness for each other edge */
        for ( i = 0; i < size; ++i ) {
            assignment[i]->measure = edge_betweenness( g, assignment[i]->from, assignment[i]->to );
        }

        /* Construct the new Communities */
        retval = construct_communities( g );

        /* Calculate the new modularity */
        modularity = calculate_modularity( g );
    } while ( modularity < limit );

    return retval;
}

static int edge_betweenness( ptr_graph g, int from, int to )
{
}

static int calculate_modularity( ptr_graph g )
{
}

static int detect_edge( const void *a, const void *b )
{
    return ( ( (struct edge_measuring*) a )->from != ( (struct edge_measuring*) b )->from
        && ( ( (struct edge_measuring*) a )->to != ( (struct edge_measuring*) b )->to );
}

static int compare_edges( const void *a, const void *b )
{
    return ( (struct edge_measuring*) b )->measure - ( (struct edge_measuring*) a )->measure;
}

static struct Communities *construct_communities( ptr_graph g )
{
}
