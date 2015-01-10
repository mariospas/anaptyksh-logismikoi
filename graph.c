#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "graph.h"
#include "hash_table.h"
#include "graph_entry.h"
#include "linked_list.h"
#include "dataset_handlers.h"
#include "database.h"

struct graph
{
    /* Type of graph */
	int id;

    /* Assignment */
	ht_ptr table;

    /* Number of nodes */
	int size;
};

ptr_graph createGraph(int id, int num_of_buckets, int size_of_bucket )
{
	ptr_graph graph;

	graph = malloc( sizeof( struct graph ) );
	graph->table = HT_create( num_of_buckets, size_of_bucket, hash );
	graph->id = id;
	graph->size = 0;

	return graph;

}

int destroyGraph(ptr_graph graph )
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


int insertEdge( ptr_graph graph, int id, ptr_edge edge )
{
	//printf("Search for id = %d \n",id);
	ptr_entry node = HT_search( graph->table, id );
	//printf("in insert edge ID = %d\n",node->id);
	if(node == NULL)
	{
		printf("Entry not Found\n\n");
		return 1;
	}
	else if(node != NULL)
	{
		//printf("Normal Insert\n");
		LL_insert( node->edges, (void*) edge );
	}


	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id)
{
	ptr_entry node = NULL;
	node = HT_search(graph->table,id);
	if(node == NULL) printf("NULL in lookupNode\n");
	return node;
}

int reachNode1( ptr_graph this, int start, int end )
{
    int i, result;
    for ( i = 0; i < this->size; ++i )
    {
    	//printf("In reachNode i=%d\n",i);
        if ( rec_search( this, start, end, i ) != 0 )
        {
            return i;
        }
    }
    return -1;
}

int rec_search( ptr_graph this, int start, int end, int level )
{
	//printf("insert in rec_search\n");
    int i, size,list_size, result = 0;
    list_ptr edges;
    ptr_edge edge;
    LL_iter_ptr edge_it;
    ptr_entry entry;
    if ( level == 0 )
    {
        return start == end;
    }

    entry = ( (ptr_entry) HT_search( this->table, start ) );
    edges = type_list(entry,"person_knows_person.csv");       //( (ptr_entry) HT_search( this->table, start ) )->edges;

    list_size = LL_size(edges);
    if(list_size == 0) return result;

    edge_it = LL_iter_create( edges );
    for ( i = 0; i < this->size - level; ++i )
    {
    	//printf("rec_search  i = %d start = %d end = %d\n",i,start,end);
        do
        {
            edge = LL_iter_data( edge_it );
            if ( edge->target_type == this->id
              && rec_search( this, edge->target_id, end, level - 1 ) != 0
              && strcmp(edge->edge_type,"person_knows_person.csv") == 0)
            {
                result = 1;
                break;
            }
        }
        while ( LL_iter_next( edge_it ) );
        LL_iter_reset( edge_it );
    }
    LL_iter_destroy( edge_it );
    LL_destroy(edges,destroy_edge);
    return result;
}


ResultSet *reachNodeN( ptr_graph graph, int start )
{
	ResultSet *result = malloc(sizeof(ResultSet));

    /* Initialize only */
    result->graph = graph;
    result->current = HT_iter_create( graph->table );
    result->from = start;

	return result;
}


int ResultSet_next(ResultSet *result, int *id, int *distance )
{
    assert( result != NULL );
    ptr_entry entry = HT_iter_data( result->current );
    *id = entry->id;
    *distance = reachNode1( result->graph, result->from, entry->id );

    /* End condition */
    if ( ! HT_iter_next( result->current ) ) {
        HT_iter_destroy( result->current );
        free( result );
        return 0;
    }
    return 1;
}


void load_graph(ptr_graph graph)
{
	FILE *fp;
	char *buf = malloc(MAX_STR_LEN);
	char *tmp;
	char *tempB;
	char *tempCreat;
	ptr_entry entry;
	ptr_edge edge;

	char *edge_type;
	int target_type;
	char *filename;

	int id;
	ptr_date creationDate;
	char *location_ip;
	char *browser;

	if (buf == NULL)
	{
		printf ("No memory\n");
		return;
	}

	printf("here!\n");
	if(graph->id == PERSON)
	{
		printf("here!!!\n");

		char *firstname;
		char *lastname;
		gender_t gender;
		ptr_date birthday;



		int id1;
		int id2;
		ptr_date extra;

		/************** person.cvs *************/
		if ( ( fp = fopen( "dataset/person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		fgets(buf, 1023, fp);   //first line

		while (fgets(buf, 1023, fp) != NULL)
		{
			if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
				buf[strlen (buf) - 1] = '\0';


			tmp = strtok(buf, "|");
			id = atoi(tmp);
			printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			firstname = strdup(tmp);
			printf("firstname = %s ",firstname);

			tmp = strtok(NULL, "|");
			lastname = strdup(tmp);
			printf("lastname = %s ",lastname);

			tmp = strtok(NULL, "|");
			if(strcmp(tmp,"male") == 0) gender = MALE;
			if(strcmp(tmp,"female") == 0) gender = FEMALE;
			printf("gender = %d \n",gender);

			tmp = strtok(NULL, "|");
			tempB = strdup(tmp);
			printf("Birthday = %s ",tmp);
			//birthday = load_date(temp,4);

			tmp = strtok(NULL, "|");
			printf("Creation Date = %s \n",tmp);
			tempCreat = strdup(tmp);
			//creationDate = load_date(temp,5);

			tmp = strtok(NULL, "|");
			location_ip = strdup(tmp);
			printf("location_ip = %s ",tmp);

			tmp = strtok(NULL, "|");
			browser = strdup(tmp);
			printf("browser = %s\n\n",tmp);

			birthday = load_date(tempB,4);
			creationDate = load_date(tempCreat,5);

			ptr_person_info person = person_create(id,firstname,lastname,gender,birthday,creationDate,location_ip,browser);

			entry = create_entry(id,((void *)person),person_delete);

			insertNode(graph,entry);

			free(tempB);
			free(tempCreat);
		}

		/********* person_knows_person.cvs **************/
		if ( ( fp = fopen( "dataset/person_knows_person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_knows_person.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);

		/********* person_hasInterest_tag.csv *************/
		if ( ( fp = fopen( "dataset/person_hasInterest_tag.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_hasInterest_tag.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);

		/********** person_isLocatedIn_place.csv ***********/
		if ( ( fp = fopen( "dataset/person_isLocatedIn_place.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_isLocatedIn_place.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);

		/********** person_workAt_organisation.csv ************/
		if ( ( fp = fopen( "dataset/person_workAt_organisation.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_workAt_organisation.csv");

		load_2ids_and_extra(graph,buf,fp,filename,PERSON);

		free(filename);



		/*********** person_studyAt_organisation.csv ************/
		if ( ( fp = fopen( "dataset/person_studyAt_organisation.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_studyAt_organisation.csv");

		load_2ids_and_extra(graph,buf,fp,filename,PERSON);

		free(filename);


		/********** person_likes_post.csv ************/
		if ( ( fp = fopen( "dataset/person_likes_post.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}


		fgets(buf, 1023, fp);   //first line

		while (fgets(buf, 1023, fp) != NULL)
		{
			if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
				buf[strlen (buf) - 1] = '\0';


			tmp = strtok(buf, "|");
			id1 = atoi(tmp);
			printf(" id1 = %d ",id1);

			tmp = strtok(NULL, "|");
			id2 = atoi(tmp);
			printf("id2 = %d ",id2);

			tmp = strtok(NULL, "|");
			tempB = strdup(tmp);
			printf("extra = %s \n",tempB);

			extra = load_date(tempB,5);


			edge_type = strdup("person_likes_post.csv");
			target_type = PERSON;

			edge = create_edge(edge_type,id2,target_type,0,((void *) extra));

			insertEdge(graph,id1,edge);

			free(tempB);
			free(edge_type);
		}

	}
	else if(graph->id == POST)
	{
		char *imageFile;
		char *language;
		char *content;
		int flag = 0;

		/************** post.csv *************/
		if ( ( fp = fopen( "dataset/post.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		fgets(buf, 1023, fp);   //first line

		while (fgets(buf, 1023, fp) != NULL)
		{
			if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
				buf[strlen (buf) - 1] = '\0';


			tmp = strtok(buf, "|");
			id = atoi(tmp);
			printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			imageFile = strdup(tmp);
			//printf("imageFile = %s ",imageFile);


			tmp = strtok(NULL, "|");
			//printf("Creation Date = %s \n",tmp);
			tempCreat = strdup(tmp);
			if(imageFile[0] >= '0' && imageFile[0] <= '9')
			{
				free(tempCreat);
				tempCreat = strdup(imageFile);
				free(imageFile);
				imageFile = NULL;
				flag = 1;

			}
			else printf("imageFile = %s ",imageFile);
			printf("Creation Date = %s \n",tempCreat);


			if(flag == 0) tmp = strtok(NULL, "|");
			else flag = 0;
			location_ip = strdup(tmp);
			printf("location_ip = %s ",tmp);

			tmp = strtok(NULL, "|");
			browser = strdup(tmp);
			printf("browser = %s\n",tmp);

			tmp = strtok(NULL, "|");
			//printf("tmp = %s\n",tmp);
			if(tmp != NULL && tmp[0] != '\0')
			{
				language = strdup(tmp);
				printf("language = %s ",language);
			}
			else language = NULL;

			tmp = strtok(NULL, "|");
			//printf("tmp = %s\n",tmp);
			if(tmp != NULL && tmp[0] != '\0')
			{
				content = strdup(tmp);
				printf("content = %s ",content);
			}
			else content = NULL;

			printf("\n");
			//printf("before creation date\n");

			creationDate = load_date(tempCreat,5);

			//printf("before create post\n");

			ptr_post_info post = post_create(id,imageFile,creationDate,location_ip,browser,language,content);

			//printf("before create entry\n");

			entry = create_entry(id,((void *)post),post_delete);

			//printf("before insert node\n");

			insertNode(graph,entry);

			//printf("before free\n");

			free(tempCreat);
			//printf("finish POST\n");
		}


		/*********** post_hasCreator_person.csv ************/
		if ( ( fp = fopen( "dataset/post_hasCreator_person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("post_hasCreator_person.csv");

		//printf("start load_2ids_and_extra\n");
		load_2ids(graph,buf,fp,filename,POST);
		//printf("finish load_2ids_and_extra\n");

		free(filename);

	}
	else if(graph->id == FORUM)
	{
		char *title;

		/************** forum.csv *************/
		if ( ( fp = fopen( "dataset/forum.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		fgets(buf, 1023, fp);   //first line

		while (fgets(buf, 1023, fp) != NULL)
		{
			if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
				buf[strlen (buf) - 1] = '\0';


			tmp = strtok(buf, "|");
			id = atoi(tmp);
			printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			title = strdup(tmp);
			printf("title = %s ",title);


			tmp = strtok(NULL, "|");
			//printf("Creation Date = %s \n",tmp);
			tempCreat = strdup(tmp);

			printf("\n");
			//printf("before creation date\n");

			creationDate = load_date(tempCreat,5);

			//printf("before create post\n");

			ptr_forum_info forum = forum_create(id,title,creationDate);

			//printf("before create entry\n");

			entry = create_entry(id,((void *)forum),forum_delete);

			//printf("before insert node\n");

			insertNode(graph,entry);

			//printf("before free\n");

			free(tempCreat);
			//printf("finish POST\n");
		}

		/*********** forum_containerOf_post.csv ************/
		if ( ( fp = fopen( "dataset/forum_containerOf_post.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("forum_containerOf_post.csv");

		//printf("start load_2ids_and_extra\n");
		load_2ids(graph,buf,fp,filename,FORUM);
		//printf("finish load_2ids_and_extra\n");

		free(filename);

		/*********** forum_hasMember_person.csv ************/
		if ( ( fp = fopen( "dataset/forum_hasMember_person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("forum_hasMember_person.csv");

		load_2ids_and_extra(graph,buf,fp,filename,FORUM);

		free(filename);

	}
}


void load_2ids(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type)
{
	char *tmp;
	int id1;
	int id2;
	char *edge_type;
	int target_type;
	ptr_edge edge;

	fgets(buf, 1023, fp);   //first line

	while (fgets(buf, 1023, fp) != NULL)
	{
		if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
			buf[strlen (buf) - 1] = '\0';


		tmp = strtok(buf, "|");
		id1 = atoi(tmp);
		printf("id1 = %d ",id1);

		tmp = strtok(NULL, "|");
		id2 = atoi(tmp);
		printf("id2 = %d \n",id2);

		edge_type = strdup(filename);
		target_type = targ_type;

		edge = create_edge(edge_type,id2,target_type,0,NULL);

		insertEdge(graph,id1,edge);

		free(edge_type);
	}
}

void load_2ids_and_extra(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type)
{
	char *tmp;
	int id1;
	int id2;
	char *edge_type;
	int target_type;
	ptr_edge edge;
	ptr_date extra;
	char *tempB;

	fgets(buf, 1023, fp);   //first line

	while (fgets(buf, 1023, fp) != NULL)
	{
		if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
			buf[strlen (buf) - 1] = '\0';


		tmp = strtok(buf, "|");
		id1 = atoi(tmp);
		printf(" id1 = %d ",id1);

		tmp = strtok(NULL, "|");
		id2 = atoi(tmp);
		printf("id2 = %d ",id2);

		tmp = strtok(NULL, "|");
		tempB = strdup(tmp);
		printf("extra = %s \n",tempB);

		extra = load_date(tempB,1);


		edge_type = strdup("person_workAt_organisation.csv");
		target_type = PERSON;

		edge = create_edge(edge_type,id2,target_type,0,((void *) extra));

		insertEdge(graph,id1,edge);

		free(tempB);
		free(edge_type);
	}
}


ptr_date load_date(char* buf,int i)
{
	char *tmpo;
	char *temp;
	ptr_date date;
	size_t year;
	size_t month;
	size_t day;
	size_t hour;
	size_t minute;
	size_t sec;
	int j;

	if(i == 1)
	{
		year = atoi(buf);
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		sec = 0;

		date = date_create(year,month,day,hour,minute,sec);

		return date;

	}


	tmpo = strtok(buf, "-");
	year = atoi(tmpo);
	//printf("year = %zu ",year);

	tmpo = strtok(NULL, "-");
	month = atoi(tmpo);
	//printf("month = %zu ",month);

	tmpo = strtok(NULL, "-");
	day = atoi(tmpo);
	//printf("day = %zu \n",day);



	if(i == 4)
	{
		//printf("\nBirthday :\n");

		hour = 0;
		//printf("hour = %zu ",hour);
		minute = 0;
		//printf("minute = %zu ",minute);
		sec = 0;
		//printf("sec = %zu \n",sec);
	}
	else if(i == 5)
	{
		//printf("\nCreation date :\n");
		//printf("year = %zu ",year);
		//printf("month = %zu ",month);
		//printf("day = %zu ",day);

		int len = strlen(tmpo) + 1;
		for(j=0;j<len;j++)
		{
			if(tmpo[j] != 'T') tmpo[j] = '-';
			else if(tmpo[j] == 'T')
			{
				//printf("find T\n");
				tmpo[j] = '|';
				break;
			}
		}

		temp = strdup(tmpo);
		//printf("inse %s\n",temp);

		tmpo = strtok(temp, "|");

		tmpo = strtok(NULL, ":");
		//printf("inse %s\n",tmpo);
		hour = atoi(tmpo);
		//printf("hour = %zu ",hour);

		tmpo = strtok(NULL, ":");
		minute = atoi(tmpo);
		//printf("minute = %zu ",minute);

		tmpo = strtok(NULL, ":");
		sec = atoi(tmpo);
		//printf("sec = %zu \n",sec);

		free(temp);
	}

	date = date_create(year,month,day,hour,minute,sec);

	return date;


}


size_t hash(int value, size_t size)
{
    return ( size * fmod( ( value * HASH_CONSTANT ), 1 ) );
}

size_t Graph_size( ptr_graph graph )
{
    return graph->size;
}

ht_ptr Graph_nodes( ptr_graph graph )
{
    return graph->table;
}
