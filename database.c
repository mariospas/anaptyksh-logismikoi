
#include "database.h"
#include "graph_stats.h"
#include "graph.h"

#define ENTITY_NUMBER 7


typedef enum {
 COMMENT,
 FORUM,
 ORGANISATION,
 PERSON,
 PLACE,
 POST,
 TAG
} entity_type;

struct database
{
	ptr_entity entities[ENTITY_NUMBER];
};

struct entity
{
	int id;
	ptr_graph basic;
	ptr_graph* sxeseis;
};









///mia sunarthsh pou na pernei properties kai na ta bazeis sto idiothtes (person,forum,comments)
