#include <assert.h>

#include "graph.h"
#include "graph_stats.h"
#include "linked_list.h"


//########katanomh#######################

struct katanomh
{
	int size;
	int arithmos_filon;
};


int katanomh_get_size(ptr_katanomh data)
{
	return (data->size);
}

int katanomh_get_id(ptr_katanomh data)
{
	return (data->arithmos_filon);
}


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

void destroy_data(void* data)
{
	free(data);
}

void manage_list(list_ptr list,int arithmos_filon)
{
	void* data;
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
			friend_list =((list_ptr)Entry_take_list(data));
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
		LL_destroy(list,destroy_data);

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
		//printf("In Diameter\n");

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			//printf("Check for id = %d\n",data->id);
			result = reachNodeN(g,data->id);
			return_maxN(result,graph_size,&max);
			//free(result);
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
		//printf("sum = %f\n",*sum);
	}
}


int averagePathLength(ptr_graph g, double *apotel)
{

	int i, graph_size = Graph_size(g);
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry data;
	Result_ptr result;
	//double apotel = 0.0;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);
		//printf("In averagePathLenght\n");

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			//printf("Check for id = %d\n",data->id);
			result = reachNodeN(g,data->id);
			sum_from_result(result,graph_size,apotel);
			//free(result);
			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);

		*apotel = *apotel/2.0;

		*apotel = ((*apotel)*2.0)/((double)((graph_size)*((graph_size)-1)));

		//printf("apotel = %f\n",*apotel);

	}

	return 1;

}


//#########plithos sunektikon grafimaton######

struct dataCC
{
	int id;
};

int dataCC_get_id(dataCC_ptr data)
{
	return data->id;
}

dataCC_ptr create_dataCC(int id)
{
	dataCC_ptr data = malloc(sizeof(struct dataCC));
	data->id = id;
	return data;
}

void destroy_dataCC(void* data)
{
	free(data);
}

int match_id( const void *a, const void *key)
{
	dataCC_ptr A = ((dataCC_ptr) a);
	//printf("tsekarisma a = %d b = %d\n",A->id,*((int*)key));
	if( A->id == *((int*)key)  )
	{
		return 0;
	}
	else return 1;
}


int node_exist(list_ptr list,ptr_entry node)     //0 an den to brei kai 1 an to brei
{
	int id = node->id;
	void* data;

	//printf("Size of list frienge = %d and id = %d\n",LL_size(list),id);
	data = LL_search(list,((void*) &id));
	if(data == NULL) return 0;
	return 1;
}


void rec_search_dfs(ptr_graph g,list_ptr list,ptr_entry node,int *size)
{

	Result_ptr result;
	int id = -3;
	int distance = -3;
	dataCC_ptr data;
	int i;
	int size_g = Graph_size(g);

	result = reachNodeN(g,node->id);

	for(i=0;i<size_g;i++)
	{
		ResultSet_next(result,&id,&distance);
		if(distance != -1)
		{
			//printf("^^^^^^ id = %d before i inserted in frienge\n",id);
			data = create_dataCC(id);
			LL_insert(list,((void*) data));
			*size = *size + 1;
		}
	}
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

	LL_iter_ptr iterList;
	dataCC_ptr data_CC;

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


		LL_destroy(fringe,destroy_dataCC);
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



double density(ptr_graph g,double *d)
{
	int i, graph_size = Graph_size(g);
	HT_iter_ptr iter;
    ht_ptr nodes = Graph_nodes(g);
	ptr_entry node;
	long sizeEdges = 0;
	//double d = 1;

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));
			if(node != NULL)
			{
				sizeEdges = sizeEdges + size_of_friend_list(node);
				//printf("sizeEdges = %ld\n",sizeEdges);
			}

			HT_iter_next(iter);
		}


		HT_iter_destroy(iter);

		*d = (2 * ((double)sizeEdges)) / (((double)graph_size) * ((double)graph_size - 1));
		//printf("D = %f\n",*d);
	}

	//printf("D = %f\n",*d);
	return 1;
}

double closeness_centrality( ptr_entry n, ptr_graph g )
{
    double sumdist = 0, size = Graph_size(g);
    ptr_entry entry;
    HT_iter_ptr inode = HT_iter_create( Graph_nodes(g) );
    int k;

    Result_ptr result;
    int i;
    int id = -3;
    int distance = -3;

    result = reachNodeN(g,n->id);

	for(i=0;i<size;i++)
	{
		ResultSet_next(result,&id,&distance);
		if(distance != -1 && distance != 0)
		{
			//printf("******k = %f\n",((double)distance));
			sumdist += 1.0/((double)distance);
			//printf("******sumdist = %f\n",sumdist);
		}
	}

    printf("^^^^^^result = %f and size = %f^^^^^^^\n",(sumdist / ( (double) (size - 1) )),size);
    return sumdist / ( (double) (size - 1) );
}

#if 0
double betweenness_centrality( ptr_entry n, ptr_graph g )
{
    int size = Graph_size(g), alldist, betweendist;
    int id, dist, dist1, dist2, i, next;
    double ret = 0.0;
    ht_ptr checked_nodes = HT_create( 8, 8, hash );
    HT_iter_ptr node_it = HT_iter_create( Graph_nodes(g) );
    ptr_entry node;
    ResultSet *set;

    HT_insert( checked_nodes, (void*) 1, n->id );

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        if ( node->id == n->id ) {
            continue;
        }

        set = reachNodeN( g, node->id );
        alldist = 0;
        betweendist = 0;

        /* Check every shortest path */
        while ( ( next = ResultSet_next( set, &id, &dist ) ) >= 0 ) {
            do {
                if ( node->id == id ) {
                    break;
                }
                if ( HT_search( checked_nodes, id ) ) {
                    break;
                }
                if ( ( dist1 = reachNode1( g, node->id, n->id ) ) < 0 ) {
                    break;
                }
                if ( ( dist2 = reachNode1( g, n->id, id ) ) < 0 ) {
                    break;
                }
                ++alldist;
                if ( dist == ( dist1 + dist2 ) ) {
                    ++betweendist;
                }
            } while (0);
            if ( next == 0 ) {
                break;
            }
        }
        if ( alldist > 0 ) {
            ret += ( ( (double) betweendist ) / alldist );
            printf( "%d ret: %lf\n", n->id, ( ( (double) betweendist ) / alldist ) );
        }
        HT_insert( checked_nodes, (void*) 1, node->id );
    } while ( HT_iter_next( node_it ) );
    HT_destroy( checked_nodes, NULL );
    HT_iter_destroy( node_it );

    //fprintf( stderr, "%lf\n", ( 2 * ret ) / ( (doouble) ( size - 1 ) * ( size - 2 ) ) );
    return ( 2 * ret ) / ( (double) ( size - 1 ) * ( size - 2 ) );
}
#endif

double betweenness_centrality( ptr_entry n, ptr_graph g )
{
    int size = Graph_size(g);
    int id, dist, next, found, shortest_in, shortest_out;
    double ret = 0.0;
    ht_ptr checked_nodes = HT_create( 8, 8, hash );
    list_ptr checked_paths;
    HT_iter_ptr node_it = HT_iter_create( Graph_nodes(g) );
    ptr_entry node;
    ResultSet *set;

    HT_insert( checked_nodes, n, n->id );

    /* For each node in the graph */
    do {
        node = HT_iter_data( node_it );
        if ( node->id == n->id ) {
            continue;
        }
        set = reachNodeN( g, node->id );
        checked_paths = LL_create( compare_ints );

        /* For each possible pair of nodes */
        while ( ( next = ResultSet_next( set, &id, &dist ) ) || 1 ) {
            do {
                if ( n->id == id || node->id == id ) {
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
                shortest_paths_rec_( g, node->id, id, n->id, &found, &shortest_in, &shortest_out, dist, checked_paths );
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

    return ( 2 * ret ) / ( ( size - 1 ) * ( size - 2 ) );
}

void shortest_paths_rec_( ptr_graph g, int start, int end, int through, int *found, int *shortest_in, int *shortest_out, int dist, list_ptr checked )
{
    int temp, old_found, node_id;
    ptr_entry starting_node;
    ptr_edge edge;
    list_ptr edges;
    LL_iter_ptr edge_it;
    double sum;

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
    if ( start == through ) {
        *found = 1;
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
        shortest_paths_rec_( g, node_id, end, through, found, shortest_in, shortest_out, dist - 1, checked );
        LL_delete( checked, (void*) node_id );
        *found = old_found;
    } while ( LL_iter_next( edge_it ) );
    LL_iter_destroy( edge_it );
}

int compare_ints( const void *a, const void *b )
{
    return ! ( a == b );
}
