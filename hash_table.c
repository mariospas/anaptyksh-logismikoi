#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

struct bucket
{
	int counter;                 //posa exei mesa
	void* records[BUCKET_SIZE];
	ptr_bucket overflow;
};

struct hash_table
{
	int i;               //allazei kathe fora pou allazei h hash function
	int p;				 //allazei kathe fora pou exoume uperxeilish
	int m;               //size
	ptr_bucket buckets;
};




ptr_hash HT_create( void )
{
	int i;
	ptr_hash this = malloc( sizeof( struct hash_table) );

	this->i = 0;
	this->p = 0;
	this->buckets = malloc(INIT_SIZE * sizeof(struct bucket));

	for(i=0;i<INIT_SIZE;i++)
	{
		this->buckets[i] = NULL;
	}

	return this;
}

void rec_bucket_destroy(ptr_bucket this)
{

	if(this->overflow != NULL)
	{
		rec_bucket_destroy(this->overflow);
	}
	free(this);
}

void HT_destroy(ptr_hash this)
{
	int i;
	ptr_bucket next;

	//isos diagrapsoume kai listes mesa se eggrafes

	rec_bucket_destroy(this->buckets);

	free(this->buckets);
}


void HT_insert_node(ptr_hash this,hash_f h,void* element)
{

}
