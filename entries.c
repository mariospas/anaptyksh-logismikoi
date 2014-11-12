#include "entries.h"
#include <stdio.h>
#include <stdlib.h>


struct entry
{
	int id;
	void* properties;
	void* friends;    //deiktis se lista filon
};


struct edge
{
	int id;    //id filou
	void* lista_idiotiton;    //knows,respect,etc
};


ptr_entry create_entry(int id,void* properties)      //create node
{
	ptr_entry node;

	node = malloc(sizeof(struct entry));

	node->id = id;
	node->properties = properties;
	node->friends = NULL;
	//node->friends = (void*) LL_create(match);   na to doume

	return node;

}



ptr_edge create_edge(int id, void* lista_idiotiton)
{
	ptr_edge akmh;

	akmh = malloc(sizeof(struct edge));

	akmh->id = id;
	akmh->lista_idiotiton = lista_idiotiton;

	return akmh;
}
