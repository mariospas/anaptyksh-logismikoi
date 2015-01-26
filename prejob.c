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
