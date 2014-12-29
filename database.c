
#include "database.h"
#include "graph.h"

#define TABLE_DEFAULT_SIZE 4
#define BUCKET_DEFAULT_SIZE 4

struct entity {
	int id;
	ptr_graph assignment;
};

struct database {
	struct entity entities[NUMBER_OF_ENTITIES];
};


ptr_database DB_create(void)
{
    int i;
    ptr_database retval = malloc( sizeof(struct database) );
    for ( i = 0; i < NUMBER_OF_ENTITIES; ++i ) {
        retval->entities[i].id = i;
        retval->entities[i].assignment = createGraph( i, TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE );
    }
}

void DB_destroy( ptr_database this )
{
    int i;
    for ( i = 0; i < NUMBER_OF_ENTITIES; ++i ) {
        destroyGraph( this->entities[i].assignment );
    }
    free( this );
}

ptr_graph DB_get_entity( ptr_database this, entity_type id )
{
    return this->entities[id].assignment;
}
