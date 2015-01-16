
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
