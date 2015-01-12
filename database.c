
#include "database.h"
#include "graph.h"


struct entity {
	int id;
	ptr_graph assignment;
};

struct database {
	struct entity entities[NUMBER_OF_ENTITIES];
};


ptr_database DB_create(void)
{
	//printf("insert DB_create\n");
    int i;
    ptr_database retval = malloc( sizeof(struct database) );
    for ( i = 0; i < NUMBER_OF_ENTITIES; ++i ) {
        retval->entities[i].id = i;
        retval->entities[i].assignment = createGraph( i, TABLE_DEFAULT_SIZE, BUCKET_DEFAULT_SIZE );
    }
    //printf("exiting DB_create\n");
    return retval;
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
	printf("this->entities[id].id = %d   and id = %d\n",this->entities[id].id,id);
    return this->entities[id].assignment;
}
