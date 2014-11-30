#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 
#include "entries.h"

#include "hash_table.h"

struct node_key
{
	int key;
	int id;
};

struct record {

    int id; /* key */
    void *data;
};

struct bucket {

    /* # of records filled */
    int counter;

    /* Assignment */
    struct record *records;

    /* Overflow buckets */
    struct bucket *overflow;
};

struct hash_table {

    /* Hash table initial size */
    int init_size;

    /* Hash table current size */
    int size;

    /* Bucket_size */
    int bucket_size;

    /* Changes any time a new hashing is applied */
    int level;     // i

    /* Splitting bucket index */
    int next;     // p

    /* Assignment */
    struct bucket **buckets;
};

struct HT_iterator {

    /* Hash table */
    ht_ptr base;

    /* Bucket index at the table */
    int bucket_index;

    /* Current bucket */
    struct bucket *current_bucket;

    /* Record index insibe bucket */
    int record_index;
};


ht_ptr HT_create( int table_size, int bucket_size )
{
    int i, j;
    ht_ptr this = malloc( sizeof( struct hash_table) );

    this->level = 0;
    this->next = 0;
    this->init_size = table_size;
    this->size = table_size;
    this->bucket_size = bucket_size;
    this->buckets = malloc( this->init_size * sizeof(struct bucket *) ); 

    for( i = 0; i < this->init_size; ++i ) {
        this->buckets[i] = malloc( sizeof(struct bucket) );
        this->buckets[i]->counter = 0;
        this->buckets[i]->overflow = NULL;

        this->buckets[i]->records = malloc( this->bucket_size * sizeof(struct record) );
        for ( j = 0; j < this->bucket_size; ++j ) {
            this->buckets[i]->records[j].id = -1;
            this->buckets[i]->records[j].data = NULL;
        }
    }

    return this;
}

static void rec_bucket_destroy_( struct bucket *this, destroyer destroy )
{
    int i;
    if ( this->overflow != NULL ) {
        rec_bucket_destroy_( this->overflow, destroy );
    }

    if ( destroy != NULL ) {
        for ( i = 0; i < this->counter; ++i ) {
            if ( this->records[i].id > -1 ) {
                destroy( this->records[i].data );
            }
        }
    }
    free( this->records );
    free( this );
}

void HT_destroy( ht_ptr this, destroyer destroy )
{
    int i;
    for ( i = 0; i < this->size; ++i ) {
        rec_bucket_destroy_( this->buckets[i], destroy );
    }
    free( this->buckets );
    free( this );
}


void HT_insert( ht_ptr this, hash_f h, void *element, int key )
{
    int i, index, hashed = h( key, this->size );
    struct bucket **found;

    /* Apply small h first */
    index = hashed % ( pow_(2, this->level) * this->init_size );

    /* If it's split investigate further */
    if ( index < this->next ) {
        index = hashed % ( pow_(2, this->level + 1) * this->init_size );
    }

    found = &this->buckets[ index ];
    while ( (*found)->overflow != NULL ) {
        found = &(*found)->overflow;
    }

    /* Case: Enough space */
    if ( (*found)->counter < this->bucket_size ) {
        ; 

    /* Case: Split */
    } else {

        /* Split */
        this->buckets = realloc( this->buckets, ++this->size * sizeof(struct bucket*) );
        assert( ( this->size - 1 ) == ( this->next + ( pow_(2, this->level) * this->init_size ) ) );

        found = &this->buckets[ index ];
        while ( (*found)->overflow != NULL ) {
            found = &(*found)->overflow;
        }

        this->buckets[ this->size - 1 ] = malloc( sizeof(struct bucket) );
        this->buckets[ this->size - 1 ]->counter = 0;
        this->buckets[ this->size - 1 ]->overflow = NULL;
        this->buckets[ this->size - 1 ]->records = malloc( this->bucket_size * sizeof(struct record) );
        for ( i = 0; i < this->bucket_size; ++i ) {
            this->buckets[ this->size - 1 ]->records[i].id = -1;
            this->buckets[ this->size - 1 ]->records[i].data = NULL;
        }
        redistribute( this, &this->buckets[ this->next ], &this->buckets[ this->size - 1 ] );

        /* Same bucket to be split; No overflow */
        if ( (*found)->counter < this->bucket_size ) {
            assert( this->buckets[ this->next ] == *found );

        /* Overflow */
        } else {
            found = &(*found)->overflow;
            *found = malloc( sizeof(struct bucket) );
            (*found)->records = malloc( this->bucket_size * sizeof(struct record) );
            clear_bucket( *found, this->bucket_size );
        }
               
        /* New level check */
        if ( this->size == ( pow_(2, this->level + 1) * this->init_size ) ) {
            ++this->level;
            this->next = 0;
        } else {
            ++this->next;
        }
    } 

    (*found)->records[ (*found)->counter ].id = hashed;
    (*found)->records[ (*found)->counter ].data = element;
    ++(*found)->counter;

    printf("---records->id = %d\n",hashed);
    printf("---records->data->id = %d\n",(((ptr_entry) element)->id) );

    qsort( (*found)->records, (*found)->counter, sizeof(struct record), record_compare );
}           

void *HT_search( ht_ptr this, int id, hash_f hash )
{
	//printf("HT_search 1 \n");
    int index, key = hash( id, this->size );
    struct bucket *found;
    void *result;
    struct node_key keyNode;
    keyNode.id = id;
    keyNode.key = key;

    //printf("HT_search 2 \n");
    /* Apply small hashing first */
    index = key % ( pow_(2, this->level) * this->init_size );

    //printf("HT_search 3 \n");
    /* If it's split investigate further */
    if ( index < this->next ) {
        index = key % ( pow_(2, this->level + 1) * this->init_size );
    }

    //printf("HT_search 4 \n");
    found = this->buckets[ index ];
    do {
    	printf("HT_search key = %d and id = %d\n",key,id);
        //result = bsearch( (void*) &key, found->records, found->counter, sizeof(struct record), record_match );
    	result = bsearch( (void*) &keyNode, found->records, found->counter, sizeof(struct record), record_match );
        found = found->overflow;
    } while ( result == NULL && found != NULL );
    //printf("HT_search 6 \n");
    return ( result != NULL ) ? ( (struct record*) result )->data : NULL;
}

static void redistribute( ht_ptr this, struct bucket **a, struct bucket **b )
{
    struct bucket *temp, *temp1, **chosen;
    int i, placement;

    /* Copy initial bucket contents to somewhere else */
    temp = malloc( sizeof(struct bucket) );
    temp->counter = (*a)->counter;
    temp->overflow = (*a)->overflow;
    temp->records = malloc( this->bucket_size * sizeof(struct record) );
    memcpy( temp->records, (*a)->records, this->bucket_size * sizeof(struct record) );
    temp1 = temp;

    /* Clear initial bucket */
    clear_bucket( *a, this->bucket_size );

    do {
        for ( i = 0; i < temp->counter; ++i ) {
            placement = temp->records[i].id % ( pow_(2, this->level + 1) * this->init_size );
            
            /* Initial bucket */
            if ( placement == this->next ) {
                chosen = a;

            /* New bucket */
            } else {
                chosen = b;
            }

            /* Overflow bucket */
            if ( (*chosen)->counter == this->bucket_size ) {
                chosen = &(*chosen)->overflow;
                *chosen = malloc( sizeof(struct bucket) );
                (*chosen)->records = malloc( this->bucket_size * sizeof(struct record) );
                clear_bucket( *chosen, this->bucket_size );
            }
            (*chosen)->records[ (*chosen)->counter ].id = temp->records[i].id;
            (*chosen)->records[ (*chosen)->counter ].data = temp->records[i].data;
            ++(*chosen)->counter;
        }
        if ( temp->overflow != NULL ) {
            temp = temp->overflow;
        } else {
            break;
        }
    } while (1);

    qsort( (*a)->records, (*a)->counter, sizeof(struct record), record_compare );
    qsort( (*b)->records, (*b)->counter, sizeof(struct record), record_compare );

    rec_bucket_destroy_( temp1, NULL );  
}

static void clear_bucket( struct bucket *b, int size )
{
    int i;
    b->counter = 0;
    b->overflow = NULL;
    for ( i = 0; i < size; ++i ) {
        b->records[i].id = -1;
        b->records[i].data = NULL;
    }
}

static int record_compare( const void *a, const void *b )
{
    return ( ( (struct record*) a )->id ) - ( ( (struct record*) b )->id );
}

/*static int record_match( const void *key, const void *object )
{
    return *( (int*) key ) - ( ( (struct record*) object )->id );
}*/

static int record_match( const void *key, const void *object )
{
	printf("^^^Record Match^^^\n");
	printf("key node = %d ---- object id = %d\n",(((struct node_key *) key)->key),( ( (struct record*) object )->id ));
	printf("key id = %d ---- object data id = %d\n",(((struct node_key *) key)->id),(((ptr_entry) ( ( (struct record*) object )->data ))->id) );
	printf("^^^^^^^^^^^^^^^^^^\n");
	if( !( (((struct node_key *) key)->key) - ( ( (struct record*) object )->id ) )  )
	{
		return (((ptr_entry) ( ( (struct record*) object )->data ))->id) - (((struct node_key *) key)->id);
	}
	return 1;
}


static int pow_( int base, int exp )
{
    int i, result = 1;
    for ( i = 0; i < exp; ++i ) {
        result *= base;
    }
    return result;
}

/* Debug */
void HT_print( ht_ptr this )
{
    int i, j;
    struct bucket *b;

    for ( i = 0; i < this->size; ++i ) {
        printf( "%d. ", i );
        b = this->buckets[i];
        do {
            for ( j = 0; j < b->counter; ++j ) {
                printf( "%d ", b->records[j].id );
            }
            if ( b->overflow != NULL ) {
                printf("-> ");
            }
            b = b->overflow;
        } while ( b != NULL );
        fputc( '\n', stdout );
    }
}

int HT_size( ht_ptr this )
{
    return this->size;
}

int HT_bsize( ht_ptr this )
{
    return this->bucket_size;
}

HT_iter_ptr HT_iter_create( ht_ptr this )
{
    HT_iter_ptr it = malloc( sizeof( struct HT_iterator ) );
    it->base = this;
    it->bucket_index = 0;
    it->current_bucket = it->base->buckets[0];
    it->record_index = 0;

    return it;
}

void *HT_iter_data( HT_iter_ptr it )
{
    return it->current_bucket->records[ it->record_index ].data;
}

int HT_iter_has_next( HT_iter_ptr it )
{
}

int HT_iter_next( HT_iter_ptr it )
{
    if ( ( it->bucket_index == ( it->base->size - 1 ) )
      || ( it->current_bucket->overflow == NULL )
      || ( it->record_index == ( it->base->bucket_size - 1 ) ) ) {
        return 0;
    }

    if ( it->record_index < ( it->base->bucket_size - 1 ) ) {
        ++it->record_index;
    } else if ( it->current_bucket->overflow != NULL ) {
        it->current_bucket = it->current_bucket->overflow;
        it->record_index = 0;
    } else {
        assert( it->bucket_index < ( it->base->size - 1 ) );
        it->current_bucket = it->base->buckets[ ++it->bucket_index ];
        it->record_index = 0;
    }
    return 1;
}

void HT_iter_reset( HT_iter_ptr it )
{
    it->bucket_index = 0;
    it->current_bucket = it->base->buckets[0];
    it->record_index = 0;
}

void HT_iter_destroy( HT_iter_ptr it )
{
    free( it );
}
