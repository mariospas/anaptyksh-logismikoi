#ifndef _DATABASE_
#define _DATABASE_

#include "graph.h"


#define NUMBER_OF_ENTITIES 8

typedef enum {
    NONE,
    COMMENT,
    FORUM,
    ORGANISATION,
    PERSON,
    PLACE,
    POST,
    TAG,
    TAGCLASS
} entity_type;

typedef struct database *ptr_database;

ptr_database DB_create(void);

void DB_destroy( ptr_database this );

ptr_graph DB_get_entity( ptr_database this, entity_type id );




#endif /* _DATABASE_ */
