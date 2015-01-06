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
	printf("Search for id = %d \n",id);
	ptr_entry node = HT_search( graph->table, id );
	printf("in insert edge ID = %d\n",node->id);
	if(node == NULL)
	{
		printf("Entry not Found\n\n");
		return 1;
	}
	else if(node != NULL)
	{
		printf("Normal Insert\n");
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
    if ( level == 0 )
    {
        return start == end;
    }

    edges = ( (ptr_entry) HT_search( this->table, start ) )->edges;

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
              && rec_search( this, edge->target_id, end, level - 1 ) != 0 )
            {
                result = 1;
                break;
            }
        }
        while ( LL_iter_next( edge_it ) );
        LL_iter_reset( edge_it );
    }
    LL_iter_destroy( edge_it );
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

	if (buf == NULL)
	{
		printf ("No memory\n");
		return;
	}


	if(graph->id == PERSON)
	{
		int id;
		char *firstname;
		char *lastname;
		gender_t gender;
		ptr_date birthday;
		ptr_date creationDate;
		char *location_ip;
		char *browser;


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
			printf("gender = %d ",gender);

			tmp = strtok(NULL, "|");
			tempB = strdup(tmp);
			printf("\ntmpBirt = %s\n",tmp);
			//birthday = load_date(temp,4);

			tmp = strtok(NULL, "|");
			printf("\ntmpCREAT = %s\n",tmp);
			tempCreat = strdup(tmp);
			//creationDate = load_date(temp,5);

			tmp = strtok(NULL, "|");
			location_ip = strdup(tmp);
			printf("\nlocation_ip = %s\n",tmp);

			tmp = strtok(NULL, "|");
			browser = strdup(tmp);
			printf("\nbrowser = %s\n",tmp);

			birthday = load_date(tempB,4);
			creationDate = load_date(tempCreat,5);

			ptr_person_info person = person_create(id,firstname,lastname,gender,creationDate,location_ip,browser);

			entry = create_entry(id,((void *)person),person_delete);

			insertNode(graph,entry);

		}

	}
}


ptr_date load_date(char* buf,int i)
{
	char *tmpo;
	ptr_date date;
	size_t year;
	size_t month;
	size_t day;
	size_t hour;
	size_t minute;
	size_t sec;
	int j;

	printf("\nInsert Date :\n");

	tmpo = strtok(buf, "-");
	printf("\nInsert Date 2: %s\n",tmpo);
	year = atoi(tmpo);
	printf("year = %zu ",year);

	tmpo = strtok(NULL, "-");
	month = atoi(tmpo);
	printf("month = %zu ",month);

	tmpo = strtok(NULL, "-");
	day = atoi(tmpo);
	printf("day = %zu \n",day);



	if(i == 4)
	{
		//printf("\nBirthday :\n");

		hour = 0;
		printf("hour = %zu ",hour);
		minute = 0;
		printf("minute = %zu ",minute);
		sec = 0;
		printf("sec = %zu \n",sec);
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
				printf("find T\n");
				tmpo[j] = '|';
				break;
			}
		}

		printf("inse %s\n",tmpo);

		tmpo = strtok(NULL, "|");
		hour = atoi(tmpo);
		printf("hour = %zu ",hour);

		tmpo = strtok(NULL, ":");
		minute = atoi(tmpo);
		printf("minute = %zu ",minute);

		tmpo = strtok(NULL, ":");
		sec = atoi(tmpo);
		printf("sec = %zu \n",sec);
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
