
#include "database.h"
#include "graph.h"


struct entity {
	int id;
	ptr_graph assignment;
};

struct database {
	struct entity entities[NUMBER_OF_ENTITIES];
};


ptr_database DB_create(void)
{
	//printf("insert DB_create\n");
    int i;
    ptr_database retval = malloc( sizeof(struct database) );
    for ( i = 0; i < NUMBER_OF_ENTITIES; ++i ) {
        retval->entities[i].id = i;
        retval->entities[i].assignment = createGraph( i, TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE );
    }
    //printf("exiting DB_create\n");
    return retval;
}

void DB_destroy( ptr_database this )
{
    int i;
    for ( i = 0; i < NUMBER_OF_ENTITIES; ++i ) {
        destroyGraph( this->entities[i].assignment );
    }
    free( this );
}

ptr_graph DB_get_entity( ptr_database this, entity_type id )
{
	printf("this->entities[id].id = %d   and id = %d\n",this->entities[id].id,id);
    return this->entities[id].assignment;
}



void isMemberInThisForum(ptr_graph graph,ptr_graph stalker_graph,int person_id)
{
	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;
	int flag = -3;
	int err;
	ptr_edge stalker_edge;


	iter = HT_iter_create(nodes);


	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));

		flag = there_is_in_forum_member_list(data,person_id);

		if(flag == 1)
		{
			stalker_edge = create_edge("isMemberInForum",data->id,PERSON,0,NULL);
			err = insertEdge(stalker_graph,person_id,stalker_edge);
		}

		HT_iter_next(iter);
	}

	return;

}



void personHasInterestTag(ptr_graph graph_person,ptr_graph women,ptr_graph men,int tag_id)
{
	int i, graph_size = Graph_size(graph_person);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph_person);
	ptr_entry data;
	int flag = -3;
	int err;
	ptr_entry person_entry;
	ptr_person_info person_info;
	int gender;


	iter = HT_iter_create(nodes);


	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));

		flag = there_is_in_tag_list(data,tag_id);

		if(flag == 1)
		{

			//printf("TRY to Insert stalker %d\n",data->id);
			person_info = data->properties;
			gender = get_gender(person_info);
			if(gender == MALE)
			{
				person_entry = copy_entry_person_knows_person(men,data);
				//insertNode(men,person_entry);
			}
			else if(gender == FEMALE)
			{
				person_entry = copy_entry_person_knows_person(women,data);
				//insertNode(women,person_entry);
			}
		}

		HT_iter_next(iter);
	}

	return;

}







list_ptr list_of_post_from_person_node(ptr_entry data,ptr_graph post_graph)
{
	list_ptr list = LL_create(match_entry);

	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(post_graph);
	ptr_entry data_post;
	int i, graph_size = Graph_size(post_graph);
	int j;

	int post_creator = -3;

	iter = HT_iter_create(nodes);
	//printf("In getTopStalkers\n");

	for(i=0;i<graph_size;i++)
	{
		data_post = ((ptr_entry)HT_iter_data(iter));
		//printf("Data id = %d\n",data_tag->id);

		post_creator = creator_of_post(data_post);

		if(post_creator == data->id)
		{
			LL_insert(list,((void *) data_post));
		}


		HT_iter_next(iter);
	}

	return list;
}




list_ptr list_of_comments_from_person_node(ptr_entry data,ptr_graph comment_graph)
{
	list_ptr list = LL_create(match_entry);

	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(comment_graph);
	ptr_entry data_comment;
	int i, graph_size = Graph_size(comment_graph);
	int j;

	int comment_creator = -3;

	iter = HT_iter_create(nodes);
	//printf("In getTopStalkers\n");

	for(i=0;i<graph_size;i++)
	{
		data_comment = ((ptr_entry)HT_iter_data(iter));
		//printf("Data id = %d\n",data_tag->id);

		comment_creator = creator_of_comments(data_comment);

		if(comment_creator == data->id)
		{
			LL_insert(list,((void *) data_comment));
		}


		HT_iter_next(iter);
	}

	return list;
}







/********** forums ****************/



struct forum_g {
	int id;
	ptr_graph assignment;
};


struct forum_database {
	int size;
	int limit;
	forum_g_ptr forums;
};


ptr_forum_database DB_forum_create(int limit)
{
	//printf("insert DB_create\n");
    int i;
    ptr_forum_database retval = malloc( sizeof(struct forum_database) );
    retval->size = 0;
    retval->limit = limit;
    retval->forums = malloc(limit * sizeof(struct forum_g));
    //printf("exiting DB_create\n");
    return retval;
}

void DB_forum_insert_graph(ptr_forum_database forum_database,int forum_id,ptr_graph forum_graph)
{
	int size = forum_database->size;
	(forum_database->forums[size]).id = forum_id;
	(forum_database->forums[size]).assignment = forum_graph;

	(forum_database->size)++;
}

void DB_forum_destroy( ptr_forum_database this )
{
    int i;
    int limit = this->size;
    for ( i = 0; i < limit; ++i ) {
        destroyGraph( this->forums[i].assignment );
    }
    free(this->forums);
    free( this );
}

ptr_graph DB_forum_get_entity( ptr_forum_database this, int id )
{
    int i = 0;
    int limit = this->limit;
    for(i=0;i<limit;i++)
    {
    	printf("this->forums[i].id = %d   and id = %d\n",this->forums[id].id,id);
    	if(this->forums[i].id == id)
    	{
    		return this->forums[i].assignment;
    	}
    }

    return NULL;
}


int DB_forum_get_size(ptr_forum_database database)
{
	return database->size;
}

int DB_forum_get_limit(ptr_forum_database database)
{
	return database->limit;
}







