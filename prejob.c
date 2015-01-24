#include "prejob.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>

struct findTopN
{
	ptr_array_matches array;
	ptr_entry data;
};

ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N)
{
	int i, graph_size = Graph_size(forum_graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(forum_graph);
	ptr_entry data;
	ptr_findTopN topNdata;

	ptr_array_matches array = create_array_match(N);

	iter = HT_iter_create(nodes);
	//printf("In matchSuggestion\n");

	pthread_t* thread_w = (pthread_t *)malloc(graph_size * sizeof(pthread_t));
	int err;


	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));
		topNdata = malloc(sizeof(struct findTopN));
		topNdata->array = array;
		topNdata->data = data;

		err = pthread_create(&thread_w[i],NULL,array_thread_insert,((void *) topNdata)) ;
		if(err)
		{
			perror2("pthread_create_find_TopN_forums",err);
		}

		HT_iter_next(iter);
	}
	HT_iter_destroy(iter);

	for(i=0;i<graph_size;i++)
	{
		pthread_join(thread_w[i],NULL);
	}

	return array;
}


void* array_thread_insert(void* topNdata)
{
	ptr_array_matches array = ((ptr_findTopN) topNdata)->array;
	ptr_entry data = ((ptr_findTopN) topNdata)->data;
	int ret = 22;
	list_ptr member_list;
	double members;
	ptr_matches match;

	member_list = type_list(data,"forum_hasMember_person.csv");

	if(member_list != NULL)
	{
		members = LL_size(member_list);
		match = create_match(data->id,members);

		pthread_mutex_lock(&mutex_find_top_n);
		insert_match(array,match);
		pthread_mutex_unlock(&mutex_find_top_n);
	}

	free(topNdata);
	pthread_exit(&ret);
}
