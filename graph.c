#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "entries.h"
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

struct katanomh
{
	int size;
	int arithmos_filon;
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





//########katanomh#######################



int match_friend(const void *a, const void *key)
{
	if( ((ptr_katanomh)a)->arithmos_filon == *((int*)key) )
	{
		(((ptr_katanomh)a)->size)++;
		return 0;
	}
	else return 1;
}


ptr_katanomh create_data(int arithmos_filon)
{
	ptr_katanomh node;
	node = malloc(sizeof(struct katanomh));

	node->arithmos_filon = arithmos_filon;
	node->size = 1;

	return node;
}

void manage_list(list_ptr list,int arithmos_filon)
{
	void* data = NULL;
	ptr_katanomh new_data;

	if(list != NULL)
	{
		data = LL_search(list,&arithmos_filon);
		if(data == NULL)
		{
			new_data = create_data(arithmos_filon);
			LL_insert(list,((void *)new_data));
		}

	}
}


void degreeDistribution(ptr_graph g)
{
	list_ptr list = LL_create(match_friend);
	int i;
    ht_ptr nodes = Graph_nodes(g);
	HT_iter_ptr iter;
	LL_iter_ptr iterList;
	ptr_entry data;
	list_ptr friend_list;
	int list_size, graph_size = Graph_size(g);
	FILE* fp;
	ptr_katanomh dataKat;
	FILE* gnuplotPipe;
	char ** commandsForGnuplot;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i < graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			friend_list = person_knows_person_list(data);
			list_size = LL_size(friend_list); //theoroume oti oloi einai knows ara to megethos tis listas
			//einai kai to zhtoumeno mas
			manage_list(list,list_size);
			HT_iter_next(iter);
		}
		fp = fopen("dedomena_katanomhs.temp","w+");
		iterList = LL_iter_create(list);

		for(i=0;i<LL_size(list);i++)
		{
			dataKat = (ptr_katanomh)LL_iter_data(iterList);
			fprintf(fp,"%d %f\n",dataKat->arithmos_filon,( (double) dataKat->size / graph_size ) );
			LL_iter_next(iterList);
		}

		LL_iter_destroy(iterList);
		HT_iter_destroy(iter);

		/*********GNUPLOT******************/

		gnuplotPipe = popen("gnuplot -persistent","w");
		commandsForGnuplot = malloc(2*sizeof(char*));
		for(i=0; i<2; i++)
		{
			commandsForGnuplot[i] = malloc(50*sizeof(char));
		}

		strcpy(commandsForGnuplot[0], "set title \"Κατανομή Βαθμού\"");
		strcpy(commandsForGnuplot[1], "plot \"dedomena_katanomhs.temp\"");

		for(i=0; i<2; i++)
		{
			fprintf(gnuplotPipe,"%s \n",commandsForGnuplot[i]);
		}

		for(i=0; i<2; i++) free(commandsForGnuplot[i]);

		free(commandsForGnuplot);

		fclose(fp);

		//remove("dedomena_katanomhs.temp");
	}
}


//#################diametros#########################

void return_maxN(Result_ptr result,int size,int* max)
{
	int i;
	int id;
	int distance;

	for(i=0;i<size;i++)
	{
		ResultSet_next(result,&id,&distance);
		if(*max < distance)
		{
			*max = distance;
		}
	}

}


int diameter(ptr_graph g)
{
	int i, graph_size = Graph_size(g);
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry data;
	Result_ptr result;
	int max = -7;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			result = reachNodeN(g,data->id);
			return_maxN(result,graph_size,&max);

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);
	}

	return max;
}



//############mesh timh monopation########


void sum_from_result(Result_ptr result,int size,double *sum)
{
	int i;
	int id;
	int distance;

	for(i=0;i<size;i++)
	{
		ResultSet_next(result,&id,&distance);
		if(distance < 0)
		{
			continue;
		}
		*sum = *sum + distance;
	}
}


double averagePathLength(ptr_graph g)
{

	int i, graph_size = Graph_size(g);
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry data;
	Result_ptr result;
	double apotel = 0.0;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			result = reachNodeN(g,data->id);
			sum_from_result(result,graph_size,&apotel);

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);

		apotel = (apotel*2)/((graph_size)*((graph_size)-1));

	}

	return apotel;

}


//#########plithos sunektikon grafimaton######


int match_id( const void *a, const void *key)
{
	if( *((int*)a) == *((int*)key)  )
	{
		return 0;
	}
	else return 1;
}


int node_exist(list_ptr list,ptr_entry node)     //0 an den to brei kai 1 an to brei
{
	int id = node->id;
	void* data;

	data = LL_search(list,((void*) &id));
	if(data == NULL) return 0;
	return 1;
}


void rec_search_dfs(ptr_graph g,list_ptr list,ptr_entry node,int *size)
{
	int id = node->id;
	list_ptr friends_list;
	LL_iter_ptr iterList;
	ptr_edge data;
	int size_friends_list;
	int i;
	ptr_entry next;


	if(LL_size(list) == 0)
	{
		LL_insert(list,((void*) &id));
		*size = *size + 1;
	}
	else if(LL_size(list) > 0 && (!node_exist(list,node)) )
	{
		LL_insert(list,((void*) &id));
		*size = *size + 1;
	}
	else
	{
		return;
	}

	friends_list = person_knows_person_list(node);
	iterList = LL_iter_create(friends_list);

	size_friends_list = LL_size(friends_list);

	for(i=0;i<size_friends_list;i++)
	{
		data = ((ptr_edge) LL_iter_data(iterList));
		id = data->id;

		next = lookupNode(g,id);             //hash lookupNode
		if(next == NULL) return;
		rec_search_dfs(g,list,next,size);


		LL_iter_next(iterList);
	}

	LL_iter_destroy(iterList);


	return;




}



int numberOfCCs(ptr_graph g)
{
	int i, graph_size = Graph_size(g);
	list_ptr fringe;
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry node;
	int size = 0;
	int num_of_graphs = 0;
	fringe = LL_create(match_id);

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));
			if(!(node_exist(fringe,node)) )
			{
				size = 0;
				num_of_graphs++;
				rec_search_dfs(g,fringe,node,&size);
			}


			HT_iter_next(iter);
		}


		HT_iter_destroy(iter);
	}

	return num_of_graphs;

}


int maxCC(ptr_graph g)
{
	int i, graph_size = Graph_size(g);
	list_ptr fringe;
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry node;
	int size = 0;
	int max_size = 0;
	int num_of_graphs = 0;
	fringe = LL_create(match_id);

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));

			if(!(node_exist(fringe,node)) )
			{
				size = 0;
				num_of_graphs++;
				rec_search_dfs(g,fringe,node,&size);
				if(max_size < size) max_size = size;
			}

			HT_iter_next(iter);
		}
		if(max_size < size) max_size = size;

		HT_iter_destroy(iter);
	}

	return max_size;
}



double density(ptr_graph g)
{
	int i, graph_size = Graph_size(g);
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry node;
	long sizeEdges = 0;
	double d = 1;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));
			if(node != NULL)
			{
				sizeEdges = sizeEdges + size_of_friend_list(node);
			}

			HT_iter_next(iter);
		}


		HT_iter_destroy(iter);

		d = (2 * sizeEdges) / ((graph_size) * (graph_size - 1));

	}

	return d;
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
