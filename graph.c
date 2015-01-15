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
	ptr_data_list data_list;
	ptr_data_list data2;
	//printf("Search for id = %d \n",id);
	ptr_entry node = HT_search( graph->table, id );
	//printf("in insert edge ID = %d\n",node->id);
	if(node == NULL)
	{
		//printf("Entry not Found\n\n");
		return 1;
	}
	else if(node != NULL)
	{
		data_list = LL_search(node->edges,((void *) (edge->edge_type)));
		//printf("Normal SEARCH\n");
		if(data_list == NULL)
		{
			//printf("First Insert\n");
			data_list = data_list_create((edge->edge_type));
			LL_insert(node->edges,(void *) data_list);
		}
		//printf("Normal Insert\n");
		LL_insert( data_list->list, (void*) edge );
		//printf("Normal FINISH\n");
	}

	//printf("Normal FINISH2\n");
	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id)
{
	ptr_entry node = NULL;
	node = HT_search(graph->table,id);
	//if(node == NULL) printf("NULL in lookupNode\n");
	return node;
}

int reachNode1( ptr_graph this, long start, long end )
{
    int level = 0, found = 0, buckets, bucket_size;
    ht_ptr visited1, visited2;
    ht_ptr frontier1, frontier2;

    if ( start == end ) {
        return 0;
    }

    /* Initialize values */
    buckets = 8;
    bucket_size = 8;
    visited1 = HT_create( buckets, bucket_size, hash );
    visited2 = HT_create( buckets, bucket_size, hash );
    frontier1 = HT_create( buckets, bucket_size, hash );
    frontier2 = HT_create( buckets, bucket_size, hash );

    HT_insert( visited1, (void*) start, start );
    HT_insert( frontier1, (void*) start, start );

    HT_insert( visited2, (void*) end, end );
    HT_insert( frontier2, (void*) end, end );

    while ( HT_size( visited1 ) < this->size && HT_size( visited2 ) < this->size ) {
        if ( expand_( this, &frontier1, visited1, &level ) == 0 ) {
            break;
        }
        if ( match_found_( frontier1, frontier2 ) ) {
            found = 1;
            break;
        }

        if ( expand_reversed_( this, &frontier2, visited2, &level ) == 0 ) {
            break;
        }
        if ( match_found_( frontier1, frontier2 ) ) {
            found = 1;
            break;
        }
    }

    /* Clean up */
    HT_destroy( visited1, NULL );
    HT_destroy( visited2, NULL );
    HT_destroy( frontier1, NULL );
    HT_destroy( frontier2, NULL );

    return found ? level : -1;
}

static int expand_( ptr_graph this, ht_ptr *frontier, ht_ptr visited, int *level )
{
    ht_ptr new_frontier = HT_create( 8, 8, hash );
    HT_iter_ptr frontier_it = HT_iter_create( *frontier );
    LL_iter_ptr edge_it;
    ptr_entry node_tryout;
    ptr_edge edge_tryout;
    int node_id, node_id2, advance = 0;
    list_ptr list_person;

    /* Iterate over the frontier nodes */
    do {
        node_id = (int) HT_iter_data( frontier_it );
        node_tryout = lookupNode( this, node_id );
        if(node_tryout != NULL)
        {
        	list_person = type_list(node_tryout,"person_knows_person.csv");
			if ( list_person == NULL ) {
				continue;
			}
			edge_it = LL_iter_create( list_person );

			/* For each node iterate over its edges */
			do {
				edge_tryout = LL_iter_data( edge_it );
				if ( (edge_tryout->target_type != this->id)  || (strcmp(edge_tryout->edge_type,"person_knows_person.csv") != 0))
				{
					continue;
				}
				node_id2 = edge_tryout->target_id;

				/* If its not already explored add it to the new frontier */
				if ( HT_search( visited, node_id2 ) == NULL ) {

					advance = 1;
					HT_insert( visited, (void*) node_id2, node_id2 );
					HT_insert( new_frontier, (void*) node_id2, node_id2 );
				}
			} while ( LL_iter_next( edge_it ) );

			LL_iter_destroy( edge_it );
        }
    } while ( HT_iter_next( frontier_it ) );

    HT_iter_destroy( frontier_it );
    HT_destroy( *frontier, NULL );
    *frontier = new_frontier;
    ++*level;
    return advance;
}

static int expand_reversed_( ptr_graph this, ht_ptr *frontier, ht_ptr visited, int *level )
{
    ht_ptr new_frontier = HT_create( 8, 8, hash );
    HT_iter_ptr frontier_it = HT_iter_create( *frontier );
    HT_iter_ptr graph_it = HT_iter_create( this->table );
    LL_iter_ptr edge_it;
    ptr_entry node_tryout;
    ptr_edge edge_tryout;
    int node_id, advance = 0;
    list_ptr list_person;

    /* Iterate over the nodes on the frontier */
    do {
        node_id = (int) HT_iter_data( frontier_it );

        /* For each such node search for other nodes that point to it */
        do {
            node_tryout = HT_iter_data( graph_it );
            if(node_tryout != NULL)
            {
            	list_person = type_list(node_tryout,"person_knows_person.csv");

				if ( list_person == NULL
				  || HT_search( visited, node_tryout->id ) != NULL ) {
					continue;
				}
				edge_it = LL_iter_create( list_person );
				do {
					edge_tryout = LL_iter_data( edge_it );
					if ( (edge_tryout->target_type != this->id)  || (strcmp(edge_tryout->edge_type,"person_knows_person.csv") != 0))
					{
						continue;
					}
					if ( edge_tryout->target_id == node_id ) {
						advance = 1;
						HT_insert( visited, (void*) node_tryout->id, node_tryout->id );
						HT_insert( new_frontier, (void*) node_tryout->id, node_tryout->id );
						break;
					}
				} while ( LL_iter_next( edge_it ) );
				LL_iter_destroy( edge_it );
            }
        } while ( HT_iter_next( graph_it ) );
        HT_iter_reset( graph_it );
    } while ( HT_iter_next( frontier_it ) );

    HT_iter_destroy( frontier_it );
    HT_iter_destroy( graph_it );
    HT_destroy( *frontier, NULL );
    *frontier = new_frontier;
    ++*level;
    return advance;
}

static int match_found_( ht_ptr frontier1, ht_ptr frontier2 )
{
    int found = 0, id1, id2;
    HT_iter_ptr it1, it2;

    it1 = HT_iter_create( frontier1 );
    it2 = HT_iter_create( frontier2 );

    do {
        id1 = (int) HT_iter_data( it1 );
        do {
            id2 = (int) HT_iter_data( it2 );
            if ( HT_search( frontier1, id2 ) != NULL ) {
                found = 1;
                break;
            }
        } while ( HT_iter_next( it2 ) );
        if ( found == 1 ) {
            break;
        }

        HT_iter_reset( it2 );
    } while ( HT_iter_next( it1 ) );

    /* Clean up */
    HT_iter_destroy( it1 );
    HT_iter_destroy( it2 );

    return found;
}

#if 0
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
#endif


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
		//printf("here!!!\n");

		char *firstname;
		char *lastname;
		gender_t gender;
		ptr_date birthday;



		int id1;
		int id2;
		ptr_date extra;

		/************** person.cvs *************/
		printf("!!! person !!!\n");
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
			//printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			firstname = strdup(tmp);
			//printf("firstname = %s ",firstname);

			tmp = strtok(NULL, "|");
			lastname = strdup(tmp);
			//printf("lastname = %s ",lastname);

			tmp = strtok(NULL, "|");
			if(strcmp(tmp,"male") == 0) gender = MALE;
			if(strcmp(tmp,"female") == 0) gender = FEMALE;
			//printf("gender = %d \n",gender);

			tmp = strtok(NULL, "|");
			tempB = strdup(tmp);
			//printf("Birthday = %s ",tmp);
			//birthday = load_date(temp,4);

			tmp = strtok(NULL, "|");
			//printf("Creation Date = %s \n",tmp);
			tempCreat = strdup(tmp);
			//creationDate = load_date(temp,5);

			tmp = strtok(NULL, "|");
			location_ip = strdup(tmp);
			//printf("location_ip = %s ",tmp);

			tmp = strtok(NULL, "|");
			browser = strdup(tmp);
			//printf("browser = %s\n\n",tmp);

			birthday = load_date(tempB,4);
			creationDate = load_date(tempCreat,5);

			ptr_person_info person = person_create(id,firstname,lastname,gender,birthday,creationDate,location_ip,browser);

			entry = create_entry(id,((void *)person),person_delete);

			insertNode(graph,entry);

			free(tempB);
			free(tempCreat);
		}

		/********* person_knows_person.cvs **************/
		printf("!!! person_knows_person !!!\n");
		if ( ( fp = fopen( "dataset/person_knows_person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_knows_person.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);


		/********* person_hasInterest_tag.csv *************/
		printf("!!! person_hasInterest_tag !!!\n");
		if ( ( fp = fopen( "dataset/person_hasInterest_tag.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_hasInterest_tag.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);

		/********** person_isLocatedIn_place.csv ***********/
		printf("!!! person_isLocatedIn_place !!!\n");
		if ( ( fp = fopen( "dataset/person_isLocatedIn_place.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_isLocatedIn_place.csv");

		load_2ids(graph,buf,fp,filename,PERSON);

		free(filename);

		/********** person_workAt_organisation.csv ************/
		printf("!!! person_workAt_organisation !!!\n");
		if ( ( fp = fopen( "dataset/person_workAt_organisation.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_workAt_organisation.csv");

		load_2ids_and_extra(graph,buf,fp,filename,PERSON,1);

		free(filename);



		/*********** person_studyAt_organisation.csv ************/
		printf("!!! person_studyAt_organisation !!!\n");
		if ( ( fp = fopen( "dataset/person_studyAt_organisation.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_studyAt_organisation.csv");

		load_2ids_and_extra(graph,buf,fp,filename,PERSON,1);

		free(filename);


		/********** person_likes_post.csv ************/
		printf("!!! person_likes_post !!!\n");
		if ( ( fp = fopen( "dataset/person_likes_post.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("person_likes_post.csv");

		load_2ids_and_extra(graph,buf,fp,filename,PERSON,5);

		free(filename);

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
			//printf("id = %d ",id);

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
			//else printf("imageFile = %s ",imageFile);
			//printf("Creation Date = %s \n",tempCreat);


			if(flag == 0) tmp = strtok(NULL, "|");
			else flag = 0;
			location_ip = strdup(tmp);
			//printf("location_ip = %s ",tmp);

			tmp = strtok(NULL, "|");
			browser = strdup(tmp);
			//printf("browser = %s\n",tmp);

			tmp = strtok(NULL, "|");
			//printf("tmp = %s\n",tmp);
			if(tmp != NULL && tmp[0] != '\0')
			{
				language = strdup(tmp);
				//printf("language = %s ",language);
			}
			else language = NULL;

			tmp = strtok(NULL, "|");
			//printf("tmp = %s\n",tmp);
			if(tmp != NULL && tmp[0] != '\0')
			{
				content = strdup(tmp);
				//printf("content = %s ",content);
			}
			else content = NULL;

			//printf("\n");
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
		printf("!!! forum !!!\n");
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
			//printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			title = strdup(tmp);
			//printf("title = %s ",title);


			tmp = strtok(NULL, "|");
			//printf("Creation Date = %s \n",tmp);
			tempCreat = strdup(tmp);

			//printf("\n");
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
			free(title);
			//printf("finish POST\n");
		}

		/*********** forum_containerOf_post.csv ************/
		printf("!!! forum_containerOf_post !!!\n");
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
		printf("!!! forum_hasMember_person !!!\n");
		if ( ( fp = fopen( "dataset/forum_hasMember_person.csv", "r" ) ) == NULL ) //Reading a file
		{
			printf( "File could not be opened.\n" );
		}

		filename = strdup("forum_hasMember_person.csv");

		load_2ids_and_extra(graph,buf,fp,filename,FORUM,5);

		free(filename);

	}
	else if(graph->id == TAG)
	{
		char* name1;
		char* url1;

		/************** tag.csv *************/
		printf("!!! forum !!!\n");
		if ( ( fp = fopen( "dataset/tag.csv", "r" ) ) == NULL ) //Reading a file
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
			//printf("id = %d ",id);

			tmp = strtok(NULL, "|");
			name1 = strdup(tmp);
			//printf("name1 = %s ",name1);

			tmp = strtok(NULL, "|");
			url1 = strdup(tmp);
			//printf("url1 = %s \n",url1);

			ptr_tag_info tag = tag_create(id,name1,url1);

			//printf("before create entry\n");

			entry = create_entry(id,((void *)tag),tag_delete);

			//printf("before insert node\n");

			insertNode(graph,entry);

			free(name1);
			free(url1);
		}

	}

	close(fp);
}


void load_2ids(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type)
{
	char *tmp;
	int id1;
	int id2;
	char *edge_type;
	int target_type;
	ptr_edge edge;

	edge_type = strdup(filename);
	target_type = targ_type;

	fgets(buf, 1023, fp);   //first line

	while (fgets(buf, 1023, fp) != NULL)
	{
		//printf("in while\n");
		if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
			buf[strlen (buf) - 1] = '\0';


		tmp = strtok(buf, "|");
		id1 = atoi(tmp);
		//printf("id1 = %d \n",id1);

		tmp = strtok(NULL, "|");
		id2 = atoi(tmp);
		//printf("id2 = %d \n",id2);

		edge = create_edge(edge_type,id2,target_type,0,NULL);

		insertEdge(graph,id1,edge);
		//printf("again while\n");
	}
	free(edge_type);
}

void load_2ids_and_extra(ptr_graph graph,char *buf,FILE *fp,char *filename,int targ_type,int choice)
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
		//printf(" id1 = %d ",id1);

		tmp = strtok(NULL, "|");
		id2 = atoi(tmp);
		//printf("id2 = %d ",id2);

		tmp = strtok(NULL, "|");
		tempB = strdup(tmp);
		//printf("extra = %s \n",tempB);

		extra = load_date(tempB,choice);


		edge_type = strdup(filename);
		target_type = targ_type;

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
    assert( value >= 0 );
    return value + 1;
    /*
    return ( size * fmod( ( value * HASH_CONSTANT ), 1 ) );
    */
}


void print_graph(ptr_graph graph)
{
	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;
	Result_ptr result;
	int max = -7;
	ptr_person_info p;

	if(graph != NULL)
	{
		iter = HT_iter_create(nodes);
		//printf("In Diameter\n");
		printf("****Graph id = %d and Size = %d\n",graph->id,graph->size);

		for(i=0;i<graph_size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			p = (ptr_person_info) (data->properties);
			printf("Name : %s and ID = %d\n",p->first_name,p->id);

			print_list_of_edges(data);
			printf("\n");

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);
	}
}





ptr_edge copy_edge(ptr_edge data)
{
	ptr_date date;
	ptr_date date1 = NULL;
	if((data->extra_data) != NULL)
	{
		date = ((ptr_date) (data->extra_data));
		date1 = date_create(date->year,date->month,date->day,date->hour,date->minute,date->second);
	}
	ptr_edge edge = create_edge(data->edge_type,data->target_id,data->target_type,data->weight,(void *)date1);
	return edge;
}


ptr_entry copy_entry(ptr_entry data)
{
	ptr_entry entry = create_entry(data->id,data->properties,data->properties_destroy);

	ptr_edge* result = ((ptr_edge*)LL_export(data->edges));

	int size = LL_size(data->edges);
	int i;

	ptr_edge edge;

	for(i=0;i<size;i++)
	{
		edge = copy_edge(result[i]);
		LL_insert(entry->edges,edge);
	}

	free(result);

	return entry;
}


ptr_entry copy_entry_person_knows_person(ptr_entry data)
{
	ptr_person_info info = ((ptr_person_info) (data->properties));
	ptr_date date = ((ptr_date)(info->birthday));
	ptr_date birthday = date_create(date->year,date->month,date->day,date->hour,date->minute,date->second);
	date = ((ptr_date)(info->creation_date));
	ptr_date creationDate = date_create(date->year,date->month,date->day,date->hour,date->minute,date->second);
	ptr_person_info info1 = person_create(info->id,info->first_name,info->surname,info->gender,birthday,creationDate,info->location_ip,info->browser_used);

	ptr_entry entry = create_entry(data->id,info1,data->properties_destroy);
	list_ptr list_of_person = type_list(data,"person_knows_person.csv");

	ptr_data_list data_list;
	char edge_type[] = "person_knows_person.csv";


	if(list_of_person != NULL)
	{
		ptr_edge* result = ((ptr_edge*)LL_export(list_of_person));

		int size = LL_size(list_of_person);
		int i;

		ptr_edge edge;

		data_list = LL_search(entry->edges,((void *) (edge_type)));
		//printf("Normal SEARCH\n");
		if(data_list == NULL)
		{
			//printf("First Insert\n");
			data_list = data_list_create((edge_type));
			LL_insert(entry->edges,(void *) data_list);
		}

		for(i=0;i<size;i++)
		{
			edge = copy_edge(result[i]);
			LL_insert(data_list->list,edge);
		}

		free(result);
	}

	return entry;
}










size_t Graph_size( ptr_graph graph )
{
    return graph->size;
}

ht_ptr Graph_nodes( ptr_graph graph )
{
    return graph->table;
}
