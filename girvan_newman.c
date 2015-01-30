#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph.h"
#include "graph_entry.h"
#include "graph_stats.h"
#include "database.h"
#include "girvan_newman.h"
#include "linked_list.h"
#include "prejob.h"

#define min_(A, B) ( ( (A) <= (B) ) ? (A) : (B) )

struct edge_measuring {
    int from;
    int to;
    int measure;
};

char *edge_type = "person_knows_person.csv";

double calculate_modularity( ptr_graph g, size_t size );
int detect_edge( const void *a, const void *b );
int compare_edges( const void *a, const void *b );
list_ptr construct_communities( ptr_graph g );
struct community *create_community( ptr_graph g, list_ptr left );
ptr_graph copy_graph( ptr_graph g, char *edge_type );
double edge_betweenness( ptr_graph g, int from, int to );
void edge_shortest_paths_rec_( ptr_graph g, int start, int end, int from, int to, int previous, int *found, int *shortest_in, int *shortest_out, int dist, list_ptr checked );

void communities_print( struct Communities *this );
void community_print( struct community *this );

struct Communities *girvan_newman_method( double limit, ptr_graph g )
{
    ptr_graph cg;
    list_ptr initial, edges;
    HT_iter_ptr node_it;
    LL_iter_ptr edge_it;
    ptr_entry node;
    ptr_edge edge;
    struct edge_measuring **assignment, **new_assignment, *edge_info, temp;
    struct edge sample;
    int i, j;
    size_t size;
    double modularity;
    struct Communities *retval;

/* Construct the initial values */

    /* Get a copy of the graph */
    cg = copy_graph( g, "person_knows_person.csv" );
    print_graph( cg );

    node_it = HT_iter_create( Graph_nodes(cg) );
    initial = LL_create( detect_edge );

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        edges = type_list( node, edge_type );
        if ( ( ! edges ) || ( ! LL_size( edges ) ) ) {
            continue;
        }

        edge_it = LL_iter_create( edges );

        /* For each edge of the node */
        do {
            edge = (ptr_edge) LL_iter_data( edge_it );
            temp.from = node->id;
            temp.to = edge->target_id;
            if ( LL_search( initial, &temp )  || ( ! lookupNode( cg, edge->target_id  ) ) ) {
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
        edge_info->measure = edge_betweenness( cg, edge_info->from, edge_info->to );
    } while ( LL_iter_next( edge_it ) );
    assignment = (struct edge_measuring**) LL_export( initial );
    size = LL_size( initial );
    LL_destroy( initial, NULL );

/* Start the loop */
    sample.target_type = PERSON;
    sample.weight = 0;
    strcpy( sample.edge_type, "person_knows_person.csv" );
    //for ( j = 0; j < 14; ++j ) {
    do {
        /* Remove the most central edge */
        qsort( assignment, size, sizeof(struct edge_measuring*), compare_edges );
        edge_info = assignment[size - 1];
        --size;

        /* Make the new assignment */
        new_assignment = malloc( size * sizeof(struct edge_measuring*) );
        memcpy( new_assignment, assignment, size * sizeof(struct edge_measuring*) );
        free( assignment );
        assignment = new_assignment;

        /* Edge 1 */
        sample.target_id = edge_info->to;
        node = lookupNode( cg, edge_info->from );
        edges = type_list( node, edge_type );
        assert( LL_delete( edges, &sample ) );

        /* Edge 2 */
        sample.target_id = edge_info->from;
        node = lookupNode( cg, edge_info->to );
        edges = type_list( node, edge_type );
        assert( LL_delete( edges, &sample ) );

        /* Calculate again the betweenness for each other edge */
        for ( i = 0; i < size; ++i ) {
            assignment[i]->measure = edge_betweenness( cg, assignment[i]->from, assignment[i]->to );
        }

        /* Calculate the new modularity */
        modularity = calculate_modularity( cg, size );

        if ( size == 1 ) {  /* shouldnt reach that / debug */
            break;
        }
     } while ( modularity < limit );
     //}

    /* Construct the new Communities */
    retval = construct_communities( cg );

    return retval;
}

double calculate_modularity( ptr_graph g, size_t size )
{
    ht_ptr checked = HT_create( 8, 8, hash );
    ptr_entry node, node2;
    HT_iter_ptr node_it, node_it2;
    list_ptr edges, edges2;
    int dist;
    double sum = 0.0;

    node_it = HT_iter_create( Graph_nodes(g) );
    node_it2 = HT_iter_create( Graph_nodes(g) );

    do {
        node = HT_iter_data( node_it );
        do {
            node2 = HT_iter_data( node_it2 );
            if ( node->id == node2->id || HT_search( checked, node2->id ) ) {
                continue;
            }
            edges = type_list( node, edge_type );
            edges2 = type_list( node2, edge_type );
            if ( ( dist = reachNode1( g, node->id, node2->id ) ) > 0 ) {
                sum += ( ( dist == 1 ) - ( ( LL_size( edges ) * LL_size( edges2 ) ) / (double) ( 2 * size ) ) );
            }
        } while ( HT_iter_next( node_it2 ) );
        HT_insert( checked, (void*) node->id, node->id );
        HT_iter_reset( node_it2 );
    } while ( HT_iter_next( node_it ) );

    HT_destroy( checked, NULL );
    HT_iter_destroy( node_it );
    HT_iter_destroy( node_it2 );

    return sum / ( 2 * size );
}

int detect_edge( const void *a, const void *b )
{
    return ! ( ( ( ( (struct edge_measuring*) a )->from == ( (struct edge_measuring*) b )->from )
                && ( ( ( (struct edge_measuring*) a )->to == ( (struct edge_measuring*) b )->to ) ) )
            ||  ( ( ( (struct edge_measuring*) a )->from == ( (struct edge_measuring*) b )->to )
                && ( ( ( (struct edge_measuring*) a )->to == ( (struct edge_measuring*) b )->from ) ) )
            );
}

int compare_edges( const void *a, const void *b )
{
    return (*((struct edge_measuring**) a) )->measure - ( *((struct edge_measuring**) b) )->measure;
}

list_ptr construct_communities( ptr_graph g )
{
    struct Communities *ret;
    struct community *init_assign[ Graph_size( g ) ];
    ptr_entry node;
    list_ptr left;
    HT_iter_ptr node_it = HT_iter_create( Graph_nodes( g ) );
    int count = 0;

    /* Construct the initial list of node ids */
    left = LL_create( compare_ints );
    do {
        node = HT_iter_data( node_it );
        LL_insert( left, (void*) node->id );
    } while ( HT_iter_next( node_it ) );
    HT_iter_destroy( node_it );

    /* Create communities while there are still nodes left */
    /*do {
        init_assign[ count++ ] = create_community( g, left );
    } while ( LL_size( left ) );
    */

    /*ret = malloc( sizeof(struct Communities) );
    ret->size = count;
    ret->array = malloc( count * sizeof(struct community*) );
    memcpy( ret->array, init_assign, count * sizeof(struct community*) );
    LL_destroy( left, NULL );
    */
    list_ptr communities;
    communities = GI_numberOfCCs(g);

    return communities;
}

struct community *create_community( ptr_graph g, list_ptr left )
{
    list_ptr gathered = LL_create( compare_ints ), old_left = LL_copy( left );
    ptr_edge edge;
    LL_iter_ptr node_it, node_it2;
    struct community *retval;
    int node_id, node2_id, previous_id;
    static int id;


    node_it = LL_iter_create( old_left );
    node_it2 = LL_iter_create( old_left );

    /* First get a random first node to begin with */
    node_id = (int) LL_iter_data( node_it );
    LL_insert( gathered, (void*) node_id );
    LL_delete( left, (void*) node_id );

    /* First graph traverse */
    while ( LL_iter_next( node_it ) ) {
        previous_id = node_id;
        node_id = (int) LL_iter_data( node_it );
        if ( reachNode1( g, previous_id, node_id ) < 0 ) {
            node_id = previous_id;
            continue;
        }
        if ( ! LL_search( gathered, (void*) node_id ) ) {
            LL_insert( gathered, (void*) node_id );
            LL_delete( left, (void*) node_id );
        }

        /* Second graph traverse */
        do {
            node2_id = (int) LL_iter_data( node_it2 );
            if ( node_id == node2_id || LL_search( gathered, (void*) node2_id ) ) {
                continue;
            }
            if ( reachNode1( g, node_id, node2_id ) > 0 ) {
                LL_insert( gathered, (void*) node2_id );
                LL_delete( left, (void*) node2_id );
            }
        } while ( LL_iter_next( node_it2 ) );
        LL_iter_reset( node_it2 );
    }

    LL_iter_destroy( node_it );
    LL_iter_destroy( node_it2 );

    retval = malloc( sizeof(struct community) );
    retval->id = id++ + 1;
    retval->size = LL_size( gathered );
    retval->members = (int*) LL_export( gathered );
    LL_destroy( gathered, NULL );
    LL_destroy( old_left, NULL );

    return retval;
}

ptr_graph copy_graph( ptr_graph g, char *edge_type )
{
    ptr_graph retval;
    HT_iter_ptr node_it;
    ptr_entry addition, node;
    ptr_data_list edge_list;
    LL_iter_ptr edge_it;
    ptr_edge edge;

    retval = createGraph( PERSON, 8, 8 );
    node_it = HT_iter_create( Graph_nodes(g) );
    do {
        node = HT_iter_data( node_it );
        addition = create_entry( node->id, node->properties, NULL );
        edge_list = malloc( sizeof(struct data_of_list) );
        edge_list->special_id = strdup( edge_type );
        edge_list->list = LL_create( match_edge );
        if ( type_list( node, edge_type ) != NULL ) {
            /* Copy the edges */
            edge_it = LL_iter_create( type_list( node, edge_type ) );
            do {
                edge = LL_iter_data( edge_it );
                if ( lookupNode( g, edge->target_id ) ) {
                    LL_insert( edge_list->list, edge );
                }
            } while ( LL_iter_next( edge_it ) );
        }
        LL_insert( addition->edges, edge_list );
        insertNode( retval, addition );
    } while ( HT_iter_next( node_it ) );

    return retval;
}

double edge_betweenness( ptr_graph g, int from, int to )
{
    int size = Graph_size(g);
    int id, dist, next, found, shortest_in, shortest_out;
    double ret = 0.0;
    ht_ptr checked_nodes = HT_create( 8, 8, hash );
    list_ptr checked_paths;
    HT_iter_ptr node_it = HT_iter_create( Graph_nodes(g) );
    ptr_entry node;
    ResultSet *set;

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        set = reachNodeN( g, node->id );
        checked_paths = LL_create( compare_ints );

        /* For each possible pair of nodes */
        while ( ( next = ResultSet_next( set, &id, &dist ) ) || 1 ) {
            do {
                if ( node->id == id ) {
                    break;
                }
                if ( dist < 0 ) {
                    break;
                }
                if ( HT_search( checked_nodes, id ) != NULL ) {
                    break;
                }
                found = 0;
                shortest_in = 0;
                shortest_out = 0;
                edge_shortest_paths_rec_( g, node->id, id, from, to, -1, &found, &shortest_in, &shortest_out, dist, checked_paths );
                if ( shortest_in || shortest_out ) {
                    ret += ( (double) shortest_in ) / ( shortest_in + shortest_out );
                }
            } while(0);
            if ( next == 0 ) {
                break;
            }
        }
        HT_insert( checked_nodes, node, node->id );
        LL_destroy( checked_paths, NULL );
    } while ( HT_iter_next( node_it ) );
    HT_destroy( checked_nodes, NULL );
    HT_iter_destroy( node_it );

    return ret;
}

void edge_shortest_paths_rec_( ptr_graph g, int start, int end, int from, int to, int previous, int *found, int *shortest_in, int *shortest_out, int dist, list_ptr checked )
{
    int temp, old_found, node_id;
    ptr_entry starting_node;
    ptr_edge edge;
    list_ptr edges;
    LL_iter_ptr edge_it;
    double sum;

    if ( ( previous == from && start == to ) || ( previous == to && start == from ) ) {
        *found = 1;
    }
    if ( dist == 0 ) {
        if ( start == end ) {
            if ( *found ) {
                ++*shortest_in;
            } else {
                ++*shortest_out;
            }
        }
        return;
    }
    starting_node = lookupNode( g, start );
    edges = (list_ptr) type_list( starting_node, "person_knows_person.csv" );
    assert( edges != NULL );

    edge_it = LL_iter_create( edges );
    do {
        edge = LL_iter_data( edge_it );
        node_id = edge->target_id;
        if ( LL_search( checked, (void*) node_id ) ) {
            continue;
        }
        old_found = *found;
        LL_insert( checked, (void*) node_id );
        edge_shortest_paths_rec_( g, node_id, end, from, to, start, found, shortest_in, shortest_out, dist - 1, checked );
        LL_delete( checked, (void*) node_id );
        *found = old_found;
    } while ( LL_iter_next( edge_it ) );
    LL_iter_destroy( edge_it );
}

void communities_print( struct Communities *this )
{
    int i, size = this->size;
    for ( i = 0; i < size; ++i ) {
        community_print( this->array[i] );
    }
}

void community_print( struct community *this )
{
    int i, size = this->size;

    printf( "{ " );
    for ( i = 0; i < size; ++i ) {
        printf( "%d ", this->members[i] );
    }
    puts( "}" );
}

void array_print( int *array, int size )
{
    int i;
    for ( i = 0; i < size; ++i ) {
        printf( "%d ", array[i] );
    }
    fputc( '\n', stdout );
}




list_ptr GI_numberOfCCs(ptr_graph g)
{
	int i, graph_size = Graph_size(g);
	list_ptr fringe;
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry node,test = NULL;
	int size = 0;
	int num_of_graphs = 0;
	fringe = LL_create(match_id);

	LL_iter_ptr iterList;
	dataCC_ptr data_CC;
	ptr_community community;
	ptr_entry entry;
	ptr_graph new_graph;
	list_ptr communities = LL_create(Com_match);

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));
			//printf("node id = %d\n",node->id);

			if(!(node_exist(fringe,node)) )
			{
				//printf("node id = %d insert\n\n",node->id);
				size = 0;
				if(LL_size(fringe) != 0)
				{
					new_graph = createGraph(PERSON,TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE);

					iterList = LL_iter_create(fringe);

					data_CC = LL_iter_data(iterList);
					printf("In fringe id = %d\n",dataCC_get_id(data_CC));
					entry = lookupNode(g,dataCC_get_id(data_CC));
					copy_entry_person_knows_person(new_graph,entry);

					while(LL_iter_next(iterList))
					{
						data_CC = LL_iter_data(iterList);
						printf("In fringe id = %d\n",dataCC_get_id(data_CC));
						entry = lookupNode(g,dataCC_get_id(data_CC));
						copy_entry_person_knows_person(new_graph,entry);
					}

					community = Com_create(num_of_graphs,new_graph);
					LL_insert(communities,((void *) community));

					LL_destroy(fringe,destroy_dataCC);
					fringe = LL_create(match_id);
				}
				num_of_graphs++;
				rec_search_dfs(g,fringe,node,&size);
			}


			HT_iter_next(iter);
		}


		HT_iter_destroy(iter);

		/*
		 * iterList = LL_iter_create(fringe);

		data_CC = LL_iter_data(iterList);
		printf("In fringe id = %d\n",data_CC->id);
		while(LL_iter_next(iterList))
		{
			data_CC = LL_iter_data(iterList);
			printf("In fringe id = %d\n",data_CC->id);
		}
		LL_iter_destroy(iterList);
		*/


		if(LL_size(fringe) != 0)
		{

			iterList = LL_iter_create(fringe);
			data_CC = LL_iter_data(iterList);
			printf("In fringe id = %d\n",dataCC_get_id(data_CC));
			test = Com_test_if_exist(communities,dataCC_get_id(data_CC));
			if(test == NULL)
			{
				new_graph = createGraph(PERSON,TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE);
				entry = lookupNode(g,dataCC_get_id(data_CC));
				copy_entry_person_knows_person(new_graph,entry);

				while(LL_iter_next(iterList))
				{
					data_CC = LL_iter_data(iterList);
					printf("In fringe id = %d\n",dataCC_get_id(data_CC));
					entry = lookupNode(g,dataCC_get_id(data_CC));
					copy_entry_person_knows_person(new_graph,entry);
				}

				community = Com_create(num_of_graphs,new_graph);
				LL_insert(communities,((void *) community));

				LL_destroy(fringe,destroy_dataCC);
				fringe = LL_create(match_id);
			}
		}


		LL_destroy(fringe,destroy_dataCC);
	}

	return communities;

}
