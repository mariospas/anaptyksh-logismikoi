#include "prejob.h"
#define perror2(s, e) fprintf(stderr, "%s: %s\n",s,strerror(e))


pthread_mutex_t mutex_forum = PTHREAD_MUTEX_INITIALIZER;

struct data_forum
{
	ptr_database database;
	int forumID;
	ptr_forum_database forums_database;
};


ptr_data_forum DF_create(ptr_database database,int forum_id,ptr_forum_database forums_database)
{
	ptr_data_forum data = malloc(sizeof(struct data_forum));

	data->database = database;
	data->forumID = forum_id;
	data->forums_database = forums_database;

	return data;
}

void DF_destroy(ptr_data_forum data)
{
	free(data);
}




ptr_forum_database computeTopNForums(ptr_database database,int limit)
{
	ptr_graph forum_graph = DB_get_entity(database,FORUM);
	ptr_forum_database forums_database = DB_forum_create(limit);


	ptr_array_matches array = find_topN_forums(forum_graph,limit);

	pthread_t* thread_w = (pthread_t *)malloc(limit * sizeof(pthread_t));;

	int i;
	int g1;
	int forum_id;
	double score;
	int err;
	ptr_data_forum forum_data;

	for(g1=0;g1<(limit);g1++)
	{
		forum_id = get_match(g1,array,&score);
		printf("\nForum id = %d and Members = %f\n",forum_id,score);

		forum_data = DF_create(database,forum_id,forums_database);

		err = pthread_create(&thread_w[g1],NULL,Forum_graph_create_insert,((void *) forum_data));
		if( err )
		{
			perror2("pthread_create_computeTopNForums",err);
			exit(8);
		}

	}

	for(i=0;i<limit;i++)
	{
		printf("waiting\n");
		pthread_join(thread_w[i],NULL);
	}


	return forums_database;
}




ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N)
{
	int i, graph_size = Graph_size(forum_graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(forum_graph);
	ptr_entry data;

	ptr_array_matches array = create_array_match(N);
	ptr_matches match;
	double similarity = 0.0;

	iter = HT_iter_create(nodes);
	//printf("In matchSuggestion\n");
	list_ptr member_list;
	double members;

	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));
		member_list = type_list(data,"forum_hasMember_person.csv");

		if(member_list == NULL) continue;

		members = LL_size(member_list);
		match = create_match(data->id,members);
		insert_match(array,match);


		HT_iter_next(iter);
	}
	HT_iter_destroy(iter);

	return array;
}




void* Forum_graph_create_insert(void *data)
{
	ptr_database database = ((ptr_data_forum) data)->database;
	int forumID = ((ptr_data_forum) data)->forumID;
	ptr_forum_database forums_database = ((ptr_data_forum) data)->forums_database;

	ptr_graph graph = DB_get_entity(database,PERSON);
	ptr_graph forum_graph = DB_get_entity(database,FORUM);

	ptr_graph new_forum = createGraph(PERSON,TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE);

	ptr_entry forum_entry = lookupNode(forum_graph,forumID);
	list_ptr list_of_members = type_list(forum_entry,"forum_hasMember_person.csv");

	LL_iter_ptr iter;
	int list_size;
	int i;
	ptr_edge edge;
	ptr_entry person_entry;
	int ret = 22;

	if(list_of_members != NULL)
	{
		iter = LL_iter_create(list_of_members);
		list_size = LL_size(list_of_members);
		for(i=0;i<list_size;i++)
		{
			edge = LL_iter_data(iter);

			person_entry = lookupNode(graph,edge->target_id);
			//printf("TRY TO COPY forum ID = %d person_entry = %d\n",forum_entry->id,edge->target_id);
			if(person_entry != NULL)
			{
				//printf("COPY forum ID = %d person_entry = %d\n",forum_entry->id,person_entry->id);
				person_entry = copy_entry_person_knows_person(new_forum,person_entry);
			}

			LL_iter_next(iter);
		}
		LL_iter_destroy(iter);
	}

	pthread_mutex_lock(&mutex_forum);
	DB_forum_insert_graph(forums_database,forumID,new_forum);
	pthread_mutex_unlock(&mutex_forum);

	free(data);


	pthread_exit(&ret);

}















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
	int key_f = *((int *)key);
	ptr_community com = ((ptr_community) a);

	if(com->id_com == key_f)
	{
		return 0;
	}
	else return 1;

}

int Com_get_id(ptr_community com)
{
	int id;
	if(com != NULL)
	{
		id = com->id_com;
	}
	return id;
}

ptr_graph Com_get_graph(ptr_community com)
{
	ptr_graph graph;
	if(com != NULL)
	{
		graph = com->graph_com;
	}
	return graph;
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
	printf("!!!!!!!!!! klika_max = %d, klika_cur = %d\n",klika->max,klika->current);
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

	printf("Insert KL current = %d , max = %d\n",klika->current,klika->max);
	if(current < klika->max)
	{
		printf("pinakas[%d] = %d\n",current,id);
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


int KL_match_1(ptr_klika A,ptr_klika B)
{
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
	return count;   //posa koina epistefei
}


void KL_print_graph(ptr_graph graph)
{
	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;
	Result_ptr result;
	int max = -7;
	ptr_klika p;

	if(graph != NULL)
	{
		iter = HT_iter_create(nodes);
		//printf("In Diameter\n");
		printf("****Graph id = %d and Size = %d\n",Graph_id(graph),((int)Graph_size(graph)) );

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			p = (ptr_klika) (data->properties);
			printf("Entry ID = %d ",data->id);
			KL_print(p);

			KL_print_list_of_edges(data);
			printf("\n");

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);
	}
}


ptr_edge KL_copy_edge(ptr_edge data)
{
	ptr_klika date;
	ptr_klika date1 = NULL;
	int i;
	if((data->extra_data) != NULL)
	{
		date = ((ptr_klika) (data->extra_data));
		date1 = KL_create(date->max);
		date1->current = date->current;
		for(i=0;i<(date->max);i++)
		{
			date1->pinakas[i] = date->pinakas[i];
		}
	}
	ptr_edge edge = create_edge(data->edge_type,data->target_id,data->target_type,data->weight,(void *)date1);
	return edge;
}


ptr_entry KL_copy_entry_person_knows_person(ptr_graph graph,ptr_entry data)
{
	ptr_klika info = ((ptr_klika) (data->properties));

	int i;
	ptr_klika info1 = KL_create(info->max);
	info1->current = info->current;
	for(i=0;i<(info->max);i++)
	{
		info1->pinakas[i] = info->pinakas[i];
	}

	ptr_entry entry = create_entry(data->id,info1,data->properties_destroy);

	insertNode(graph,entry);

	list_ptr list_of_person = type_list(data,"person_knows_person.csv");

	ptr_data_list data_list;
	char edge_type[] = "person_knows_person.csv";


	if(list_of_person != NULL)
	{
		ptr_edge* result = ((ptr_edge*)LL_export(list_of_person));

		int size = LL_size(list_of_person);
		int i;

		ptr_edge edge;

		/*data_list = LL_search(entry->edges,((void *) (edge_type)));
		//printf("Normal SEARCH\n");
		if(data_list == NULL)
		{
			//printf("First Insert\n");
			data_list = data_list_create((edge_type));
			LL_insert(entry->edges,(void *) data_list);
		}
		*/

		for(i=0;i<size;i++)
		{
			edge = KL_copy_edge(result[i]);
			//LL_insert(data_list->list,edge);
			insertEdge(graph,entry->id,edge);
		}

		free(result);
	}

	return entry;
}



void KL_print_list_of_edges(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;
	ptr_data_list data_list;
	LL_iter_ptr iter;
	ptr_klika klika;

	list = ((list_ptr) Entry_take_list(node));
	if(list != NULL && LL_size(list) != 0)
	{

		iterList = LL_iter_create(list);

		data_list = LL_iter_data(iterList);

		if(data_list->list != NULL && LL_size(data_list->list) != 0)
		{
			iter = LL_iter_create(data_list->list);
			data = LL_iter_data(iter);
			printf("Edge ID = %d ",data->target_id);
			klika = ((ptr_klika) data->extra_data);
			KL_print(klika);
			while(LL_iter_next(iter))
			{
				data = LL_iter_data(iter);
				printf("Edge ID = %d ",data->target_id);
				klika = ((ptr_klika) data->extra_data);
				KL_print(klika);
			}
			LL_iter_destroy(iter);

			while(LL_iter_next(iterList))
			{
				data_list = LL_iter_data(iterList);
				iter = LL_iter_create(data_list->list);
				data = LL_iter_data(iter);
				printf("Edge ID = %d ",data->target_id);
				klika = ((ptr_klika) data->extra_data);
				KL_print(klika);
				while(LL_iter_next(iter))
				{
					data = LL_iter_data(iter);
					printf("Edge ID = %d ",data->target_id);
					klika = ((ptr_klika) data->extra_data);
					KL_print(klika);
				}
				LL_iter_destroy(iter);
			}

			LL_iter_destroy(iterList);
		}
	}
}

ptr_entry Com_test_if_exist(list_ptr communities,int id)
{
	int com_size,w;
	ptr_community com;
	int id_com=80;
	ptr_graph com_graph;
	LL_iter_ptr iter;
	ptr_entry entry = NULL;

	com_size = LL_size(communities);
	if(com_size != 0)
	{
		iter = LL_iter_create(communities);

		for(w=0;w<com_size;w++)
		{
			com = (ptr_community) LL_iter_data(iter);
			id_com = Com_get_id(com);
			com_graph = Com_get_graph(com);
			entry = lookupNode(com_graph,id);
			if(entry != NULL) return entry;
			LL_iter_next(iter);
		}
		LL_iter_destroy(iter);
	}

	return entry;
}

list_ptr KL_numberOfCCs(ptr_graph g)
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
					KL_copy_entry_person_knows_person(new_graph,entry);

					while(LL_iter_next(iterList))
					{
						data_CC = LL_iter_data(iterList);
						printf("In fringe id = %d\n",dataCC_get_id(data_CC));
						entry = lookupNode(g,dataCC_get_id(data_CC));
						KL_copy_entry_person_knows_person(new_graph,entry);
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
				KL_copy_entry_person_knows_person(new_graph,entry);

				while(LL_iter_next(iterList))
				{
					data_CC = LL_iter_data(iterList);
					printf("In fringe id = %d\n",dataCC_get_id(data_CC));
					entry = lookupNode(g,dataCC_get_id(data_CC));
					KL_copy_entry_person_knows_person(new_graph,entry);
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



/************ klika struct finish ***********/


list_ptr computeCPMResults(ptr_graph graph,int cliqueSize)
{
	printf("### ComputeCPMResults ###\n");
	list_ptr communities;
	ptr_community community;
	list_ptr klikes = LL_create(KL_match);

	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;
	ptr_graph super_graph;
	ptr_edge super_edge;
	int k;

	int bhmata;
	ptr_klika klika = NULL;

	iter = HT_iter_create(nodes);
	ptr_klika* export;
	int size,j,connect;

	for(i=0;i<graph_size;i++)
	{
		bhmata = cliqueSize;
		data = ((ptr_entry)HT_iter_data(iter));

		find_clique(graph,data,&bhmata,data->id,klikes,cliqueSize);


		HT_iter_next(iter);
	}
	HT_iter_destroy(iter);

	size = LL_size(klikes);
	if(size != 0)
	{
		export = (ptr_klika*) LL_export(klikes);
		/*for(j=0;j<size;j++)
		{
			KL_print(export[j]);
		}*/
	}
	/********** exo brei klikes poio pano *****************/

	int koina;
	int max_koina = cliqueSize;
	if(size != 0)
	{

		for(j=0;j<size;j++)
		{
			if(j == 0)
			{
				super_graph = createGraph(PERSON,TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE);
			}
			data = create_entry(j,((void *) export[j]),KL_destroy);
			insertNode(super_graph,data);
			for(k=0;k<size;k++)
			{
				koina = KL_match_1(export[j],export[k]);
				if(koina == (max_koina - 1) )
				{
					super_edge = create_edge("person_knows_person.csv",k,PERSON,0,((void *) export[k]));
					insertEdge(super_graph,j,super_edge);
				}
			}

		}
	}

	KL_print_graph(super_graph);

	/****************** ftiaxnoume sunektikous grafous ***************/

	communities = KL_numberOfCCs(super_graph);
	destroyGraph(super_graph);
	//printf("CONNECT = %d\n",connect);

	printf("### finish ComputeCPMResults ###\n");
	return communities;
}


ptr_klika find_clique(ptr_graph graph,ptr_entry node,int* bhmata,int start,list_ptr klikes,int max_bhmata)
{
	printf("### find_clique ###\n");
	list_ptr friends = type_list(node,"person_knows_person.csv");
	LL_iter_ptr iter;
	int size;
	int i;
	ptr_edge edge;
	ptr_entry data;
	int flag = 1; //flag == 0 brike kuklo == 1 teleiosan ta bhmata alla den brike
	int full;
	ptr_klika new_one;
	ptr_klika klika = NULL;

	if(friends != NULL)
	{
		iter = LL_iter_create(friends);
		size = LL_size(friends);

		for(i=0;i<size;i++)
		{
			edge = LL_iter_data(iter);
			printf("bhma = %d , start = %d and edge->target_id = %d\n",*bhmata,start,edge->target_id);
			if(*bhmata == 1)
			{
				if(start == edge->target_id)
				{
					klika = KL_create(max_bhmata);
					KL_insert(klika,edge->target_id);
					return klika;
				}
				else
				{
					LL_iter_next(iter);
					continue;
				}
			}

			data = lookupNode(graph,edge->target_id);
			if(data != NULL)
			{
				(*bhmata)--;
				klika = find_clique(graph,data,bhmata,start,klikes,max_bhmata);
				//full = KL_isFull(klika);
				//printf("FULL = %d\n",full);
				if(klika != NULL)
				{
					KL_insert(klika,data->id);
					full = KL_isFull(klika);
					if(full != 0) return klika;
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

			}

			LL_iter_next(iter);
		}
		LL_iter_destroy(iter);

		(*bhmata)++;
		return NULL;

	}
	printf("### finish find_clique ###\n");

	return klika;

}
