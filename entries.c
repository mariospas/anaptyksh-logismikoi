#include "entries.h"
#include <stdio.h>
#include <stdlib.h>


ptr_entry create_entry(int id,void* properties,int (*match)( const void *a, const void *b))      //create node
{
	ptr_entry node;

	node = malloc(sizeof(struct entry));

	node->id = id;
	node->properties = properties;
	//node->friends = NULL;
	node->friends = (void*) LL_create(match);         //edges

	return node;

}


void destroy_entry(void* entry)
{
	ptr_entry this = (ptr_entry) entry;
	if(this->properties != NULL)
	{
		LL_destroy((list_ptr)this->properties,NULL);     //sto null xreiazete mia destroy gia tin lista idiotiton
	}

	if(this->properties != NULL)
	{
		LL_destroy( (list_ptr)this->friends,destroy_edge);
	}

	free(this);

}



ptr_edge create_edge(int id, void* lista_idiotiton)
{
	ptr_edge akmh;

	akmh = malloc(sizeof(struct edge));

	akmh->id = id;
	akmh->lista_idiotiton = lista_idiotiton;

	return akmh;
}


void destroy_edge(void* edge)
{
	ptr_edge this = (ptr_edge) edge;
	if(this->lista_idiotiton != NULL)
	{
		LL_destroy((list_ptr)this->lista_idiotiton,NULL);    //sto null xreiazete mia destroy gia tin lista idiotiton
	}
	free(this);
}


int match_edge(void *a,void *key)
{
	if( ((ptr_edge)a)->id == ((ptr_edge)key)->id  )
	{
		return 0;
	}
	else return 1;
}


int match_entry(void *a,void *key)
{
	if( ((ptr_entry)a)->id == ((ptr_entry)key)->id  )
	{
		return 0;
	}
	else return 1;
}
