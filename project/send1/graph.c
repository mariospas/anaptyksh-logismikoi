#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hash_table.h"
#include "entries.h"
#include "linked_list.h"


struct graph
{
	int id;          //panepistimio,atoma,douleia,etc
	ht_ptr table;
	int size;
};



ptr_graph createGraph(int id,int m,int c)
{
	ptr_graph graph;
	ht_ptr table;

	graph = malloc(sizeof(struct graph));
	table = HT_create(m,c);              //duo int gia orismata
	graph->table = table;
	graph->id = id;
	graph->size = 0;

	return graph;

}


int destroyGraph(ptr_graph graph)
{

	HT_destroy(graph->table,destroy_entry);  //int epistrofh h HT_destroy
	free(graph);

	return 0;
}


int insertNode(ptr_graph graph,ptr_entry entry,hash_f h)
{

	int key;

	HT_insert(graph->table,h,(void*) entry,key);
	graph->size++;

	return 0;
}


int insertEdge(ptr_graph graph,int id,ptr_edge filos,hash_f hash)
{
	ptr_entry node;
	void *temp;

	temp = HT_search(graph->table,id,hash);
	node = (ptr_entry) temp;

	LL_insert((list_ptr)(node->friends),(void*)filos);

	return 0;
}

ptr_entry lookupNode(ptr_graph graph,int id,hash_f hash)
{
	ptr_entry node;
	void *temp;

	temp = HT_search(graph->table,id,hash);
	node = (ptr_entry) temp;

	return node;
}




int reachNode1( ptr_graph this, int start, int end,hash_f hash )
{
    int i, result;
    for ( i = 0; i < this->size; ++i )
    {
        if ( IDS( this, start, end, i, hash ) != 0 )
        {
            return i;
        }
    }
    return -1;
}

int IDS( ptr_graph this, int start, int end, int level,hash_f hash )
{
    int i, j;
    list_ptr edges;
    ptr_edge *array;
    if ( level == 0 )
    {
        return start == end;
    }

    edges = ( (ptr_entry) HT_search( this->table, start, hash ) )->friends;
    array = (ptr_edge*) LL_export( edges );
    for ( i = 0; i < this->size - level; ++i )
    {
        for ( j = 0; j < LL_size( edges) ; ++j )
        {
            if ( IDS( this, array[j]->id, end, level - 1, hash ) != 0 )
            {
                free( array );
                return 1;
            }
        }
    }
    free( array );
    return 0;
}


ResultSet reachNodeN(ptr_graph graph, int start)
{
	int i,j,z=0;
	struct bucket *init;
	int apost, size = HT_size( graph->table );
    int bucket_size = HT_bsize( graph->table );

	ResultSet result;
	result.pinakas_id = malloc(graph->size * sizeof(int));
	result.pinakas_apost = malloc(graph->size * sizeof(int));


	for(i=0;i<(size);i++)
	{
		init = graph->table->buckets[i];
		while(init != NULL)
		{
			for(j=0;j<bucket_size;j++)
			{
				apost = reachNode1(graph, start, ( (ptr_entry) init->records[j].data )->id, hash);
				result.pinakas_apost[z] = apost;
				result.pinakas_id[z] = ( (ptr_entry) init->records[j].data )->id;
				z++;
			}
			init = init->overflow;
		}
	}
	return result;
}


void NEXT(ResultSet result, int *id, int *apostasi)
{
	static int i=0;

	*id = result.pinakas_id[i];
	*apostasi = result.pinakas_apost[i];
	i++;
}

int hash(int value, int size)
{
    return ( size * fmod((value * HASH_CONSTANT),1));
}
