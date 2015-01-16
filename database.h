#ifndef _DATABASE_
#define _DATABASE_

#include "graph.h"
#include "graph_entry.h"
#include "linked_list.h"


#define NUMBER_OF_ENTITIES 8
#define TABLE_DEFAULT_SIZE 4
#define BUCKET_DEFAULT_SIZE 4

typedef enum {
    NONE,
    COMMENT,
    FORUM,
    ORGANISATION,
    PERSON,
    PLACE,
    POST,
    TAG,
    TAGCLASS,
    STALKER
} entity_type;

typedef struct database *ptr_database;

ptr_database DB_create(void);

void DB_destroy( ptr_database this );

ptr_graph DB_get_entity( ptr_database this, entity_type id );



void isMemberInThisForum(ptr_graph graph,ptr_graph stalker_graph,int person_id);

void personHasInterestTag(ptr_graph graph_person,ptr_graph women,ptr_graph men,int tag_id);


list_ptr list_of_post_from_person_node(ptr_entry data,ptr_graph post_graph);

list_ptr list_of_comments_from_person_node(ptr_entry data,ptr_graph comment_graph);


#endif /* _DATABASE_ */
