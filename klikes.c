#include "klikes.h"


struct community
{
	int id_com;
	ptr_graph graph_com;
};


ptr_community Com_create(int id,ptr_graph graph)
{
	ptr_community com = malloc(sizeof(struct community));

	com->graph_com = graph;
	com->id_com = id;

	return com;
}

void Com_destroy(ptr_community com)
{
	if(com != NULL)
	{
		destroyGraph(com->graph_com);
		free(com);
	}
}


int Com_match(const void* a,const void* key)
{
	int key_f = *key;
	ptr_community com = ((ptr_community) a);

	if(com->id_com == key_f)
	{
		return 0;
	}
	else return 1;

}

int Com_get_id_and_graph(ptr_community com,ptr_graph graph)
{
	int id;
	if(com != NULL)
	{
		id = com->id_com;
		graph = com->graph_com;
	}
	return id;
}



/*********** klikes struct ***************/

struct klika
{
	int max;
	int current;
	int *pinakas;
};

ptr_klika KL_create(int CliqueSize)
{
	ptr_klika klika = malloc(sizeof(struct klika));

	klika->current = 0;
	klika->max = CliqueSize;
	klika->pinakas = malloc(CliqueSize * sizeof(int));

	return klika;
}

void KL_destroy(ptr_klika klika)
{
	if(klika != NULL)
	{
		free(klika->pinakas);
		free(klika);
	}
}

void KL_insert(ptr_klika klika,int id)
{
	int current = klika->current;

	if(current < klika->max)
	{
		klika->pinakas[current] = id;
		(klika->current)++;
	}
	else
	{
		printf("Error in KL_insert\n");
		exit(0);
	}
}


int KL_match(const void* a,const void* b)
{
	ptr_klika A = ((ptr_klika) a);
	ptr_klika B = ((ptr_klika) b);

	int i,j;
	int count = 0;
	int max = A->max;

	for(i=0;i<max;i++)
	{
		for(j=0;j<max;j++)
		{
			if(A->pinakas[i] == B->pinakas[j])
			{
				count++;
			}
		}
	}
	if(count == max)
	{
		return 0;
	}
	else return 1;

}


int KL_isFull(ptr_klika klika)
{
	int ret; // 0 full 1 ena diathesimo 2 duo diathesima

	ret = klika->max - klika->current;

	return ret;
}


void KL_print(ptr_klika klika)
{
	int i,current = klika->current;

	printf("klika : ");
	for(i=0;i<current;i++)
	{
		printf("%d ",klika->pinakas[i]);
	}
	printf("\n");
}


/************ klika struct finish ***********/


list_ptr computeCPMResults(ptr_graph graph,int cliqueSize)
{
	list_ptr communities = LL_create(Com_match);
	ptr_community community;
	list_ptr klikes = LL_create(KL_match);

	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;

	int bhmata;
	ptr_klika klika;

	iter = HT_iter_create(nodes);
	ptr_klika* export;
	int size,j;

	for(i=0;i<graph_size;i++)
	{
		bhmata = cliqueSize;
		data = ((ptr_entry)HT_iter_data(iter));

		find_clique(graph,data,&bhmata,data->id,klikes,cliqueSize,klika);

		size = LL_size(klikes);
		if(size != 0)
		{
			export = LL_export(klikes);
			for(j=0;j<size;j++)
			{
				KL_print(export[j]);
			}
		}


		HT_iter_next(iter);
	}
	HT_iter_destroy(iter);


}


int find_clique(ptr_graph graph,ptr_entry node,int* bhmata,int start,list_ptr klikes,int max_bhmata,ptr_klika klika)
{

	list_ptr friends = type_list(node,"person_knows_person.csv");
	LL_iter_ptr iter;
	int size;
	int i;
	ptr_edge edge;
	ptr_entry data;
	int flag = 1; //flag == 0 brike kuklo == 1 teleiosan ta bhmata alla den brike
	int full;
	ptr_klika new_one;

	if(friends != NULL)
	{
		iter = LL_iter_create(friends);
		size = LL_size(friends);

		for(i=0;i<size;i++)
		{
			edge = LL_iter_data(iter);
			if(*bhmata == 0)
			{
				if(start == edge->target_id)
				{
					KL_create(max_bhmata);
					KL_insert(klika,edge->target_id);
					return 0;
				}
				else return 1;
			}
			data = lookupNode(graph,edge->target_id);

			(*bhmata)--;
			flag = find_clique(graph,data,bhmata,start,klikes,max_bhmata,klika);

			if(flag == 0)
			{
				KL_insert(klika,data->id);
				full = KL_isFull(klika);
				if(full != 0) return 0;
				else if(full == 0)
				{
					new_one = LL_search(klikes,((void *) klika));
					if(new_one == NULL)
					{
						LL_insert(klikes,((void *) klika));
					}
					else KL_destroy(klika);
				}
			}

			LL_iter_next(iter);
		}
		LL_iter_destroy(iter);
	}

	return flag;

}
