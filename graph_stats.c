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
		printf("In Diameter\n");

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			printf("Check for id = %d\n",data->id);
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
		printf("In averagePathLenght\n");

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			printf("Check for id = %d\n",data->id);
			result = reachNodeN(g,data->id);
			sum_from_result(result,graph_size,apotel);

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);

		*apotel = ((*apotel)*2)/((graph_size)*((graph_size)-1));

		//printf("apotel = %f\n",*apotel);

	}

	return 1;

}


//#########plithos sunektikon grafimaton######

struct dataCC
{
	int id;
};

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

	result = reachNodeN(g,node->id);

	while(ResultSet_next(result,&id,&distance))
	{
		if(distance != -1)
		{
			//printf("id = %d before i inserted in frienge\n",id);
			data = create_dataCC(id);
			LL_insert(list,((void*) data));
			*size = *size + 1;
		}
	}
	if(distance != -1)
	{
		//printf("id = %d before i inserted in frienge\n",id);
		data = create_dataCC(id);
		LL_insert(list,((void*) data));
		*size = *size + 1;
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

	if(g != NULL)
	{
		iter = HT_iter_create(nodes);

		for(i=0;i<graph_size;i++)
		{
			node = ((ptr_entry)HT_iter_data(iter));
			//printf("node id = %d\n",node->id);

			if(!(node_exist(fringe,node)) )
			{
				//printf("node id = %d insert\n",node->id);
				size = 0;
				num_of_graphs++;
				rec_search_dfs(g,fringe,node,&size);
			}


			HT_iter_next(iter);
		}


		HT_iter_destroy(iter);
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
    int sumdist = 0, size = Graph_size(g);
    ptr_entry entry;
    HT_iter_ptr inode = HT_iter_create( Graph_nodes(g) );

    do {
        entry = HT_iter_data( inode );
        sumdist += reachNode1( g, n->id, entry->id );
    } while ( HT_iter_next( inode ) );
    HT_iter_destroy( inode );

    printf("result = %f\n",(sumdist / ( (double) size )));
    return sumdist / ( (double) size );
}

double betweenness_centrality( ptr_entry n, ptr_graph g )
{
    int size = Graph_size(g), alldist, betweendist;
    int id, dist, i;
    double ret = 0.0;
    HT_iter_ptr node_it = HT_iter_create( Graph_nodes(g) );
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
           // assert( ResultSet_next( set, &id, &dist ) );
            if ( dist <= 0 || node->id == id ) {
                continue;
            }

            ++alldist;
            if ( dist == ( reachNode1( g, node->id, n->id ) + reachNode1( g, n->id, id ) ) ) {
                ++betweendist;
            }
        }

        ret += (double) betweendist / alldist;
        //free( set );
    } while ( HT_iter_next( node_it ) );
    HT_iter_destroy( node_it );

    printf("result = %f\n",(( 2 * ret ) / ( (double) ( size - 1 ) * ( size - 2 ) )));
    return ( 2 * ret ) / ( (double) ( size - 1 ) * ( size - 2 ) );
}
