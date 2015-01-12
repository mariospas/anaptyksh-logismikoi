#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 

#include "hash_table.h"
#include "graph_entry.h"

struct record {

    int id; /* table produced key */
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

    /* Hash function */
    hash_f hash;
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


ht_ptr HT_create( int table_size, int bucket_size, hash_f hash )
{
    int i, j;
    ht_ptr this = malloc( sizeof( struct hash_table) );

    this->level = 0;
    this->next = 0;
    this->init_size = table_size;
    this->size = table_size;
    this->hash = hash;
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

static void rec_bucket_destroy_( struct bucket *this, deallocator destroy_entry )
{
    int i;
    if ( this->overflow != NULL ) {
        rec_bucket_destroy_( this->overflow, destroy_entry );
    }

    if ( destroy_entry != NULL ) {
        for ( i = 0; i < this->counter; ++i ) {
            if ( this->records[i].id > -1 ) {
                destroy_entry( this->records[i].data );
            }
        }
    }
    free( this->records );
    free( this );
}

static void rec_bucket_sort( struct bucket *this )
{
    int i;
    if ( this->overflow != NULL ) {
        rec_bucket_sort( this->overflow );
    }
    qsort( this->records, this->counter, sizeof(struct record), record_compare );
}

void HT_destroy( ht_ptr this, deallocator destroy_entry )
{
    int i;
    for ( i = 0; i < this->size; ++i ) {
        rec_bucket_destroy_( this->buckets[i], destroy_entry );
    }
    free( this->buckets );
    free( this );
}


void HT_insert( ht_ptr this, void *element, int key )
{
    int i, index, hashed = this->hash( key, this->size );
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
        (*found)->records[ (*found)->counter ].id = hashed;
        (*found)->records[ (*found)->counter ].data = element;
        qsort( (*found)->records, ++(*found)->counter, sizeof(struct record), record_compare );

    /* Case: Split */
    } else {

        /* Add a new index */
        this->buckets = realloc( this->buckets, ++this->size * sizeof(struct bucket*) );
        assert( ( this->size - 1 ) == ( this->next + ( pow_(2, this->level) * this->init_size ) ) );

        /* Fill the newly created bucket */
        this->buckets[ this->size - 1 ] = malloc( sizeof(struct bucket) );
        this->buckets[ this->size - 1 ]->records = malloc( this->bucket_size * sizeof(struct record) );
        clear_bucket_( this->buckets[ this->size - 1 ], this->bucket_size );

        /* Setup addition */
        found = &this->buckets[ index ];
        while ( *found != NULL ) {
            found = &(*found)->overflow;
        }
        *found = malloc( sizeof(struct bucket) );
        (*found)->records = malloc( this->bucket_size * sizeof(struct record) );
        clear_bucket_( *found, this->bucket_size );

        (*found)->records[ (*found)->counter ].id = hashed;
        (*found)->records[ (*found)->counter ].data = element;
        ++(*found)->counter;

        /* Spit the contents os the appropriate buckets */
        redistribute( this, &this->buckets[ this->next ], &this->buckets[ this->size - 1 ] );

        /* New level check */
        if ( this->size == ( pow_(2, this->level + 1) * this->init_size ) ) {
            ++this->level;
            this->next = 0;
        } else {
            ++this->next;
        }
    } 

    /*
    printf("---records->id = %d\n",hashed);
    printf("---records->data->id = %d\n",(((ptr_entry) element)->id) );
    */

}           

void *HT_search( ht_ptr this, int id )
{
	//printf("HT_search 1 \n");
    int index, key = this->hash( id, this->size );
    struct bucket *found;
    void *result;

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
        /*
    	printf("HT_search key = %d and id = %d\n",key,id);
        */
        result = bsearch( (void*) &key, found->records, found->counter, sizeof(struct record), record_match );
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
    clear_bucket_( *a, this->bucket_size );

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
            while ( (*chosen)->counter == this->bucket_size ) {
                chosen = &(*chosen)->overflow;
                if ( *chosen == NULL ) {
                    *chosen = malloc( sizeof(struct bucket) );
                    (*chosen)->records = malloc( this->bucket_size * sizeof(struct record) );
                    clear_bucket_( *chosen, this->bucket_size );
                }
            }

            (*chosen)->records[ (*chosen)->counter ].id = temp->records[i].id;
            (*chosen)->records[ (*chosen)->counter ].data = temp->records[i].data;
            ++(*chosen)->counter;
        }
        temp = temp->overflow;
    } while ( temp != NULL );

    rec_bucket_sort( *a );
    rec_bucket_sort( *b );

    rec_bucket_destroy_( temp1, NULL );  
}

static void clear_bucket_( struct bucket *b, int size )
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

static int record_match( const void *key, const void *object )
{
    return *( (int*) key ) - ( ( (struct record*) object )->id );
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

    if ( it->base->buckets[0]->counter == 0 ) {
        HT_iter_next( it );
    }

    return it;
}

void *HT_iter_data( HT_iter_ptr it )
{
    return it->current_bucket->records[ it->record_index ].data;
}

int HT_iter_next( HT_iter_ptr it )
{
    if ( ( it->bucket_index == ( it->base->size - 1 ) )
      && ( it->current_bucket->overflow == NULL )
      && ( it->record_index == ( it->current_bucket->counter - 1 ) ) ) {
        return 0;
    }

    if ( it->record_index < ( it->current_bucket->counter - 1 ) ) {
        ++it->record_index;
    } else if ( it->current_bucket->overflow != NULL ) {
        it->current_bucket = it->current_bucket->overflow;
        it->record_index = 0;
    } else {
        assert( it->bucket_index < ( it->base->size - 1 ) );
        do {
            it->current_bucket = it->base->buckets[ ++it->bucket_index ];
            it->record_index = 0;

            if ( it->bucket_index == it->base->size ) {
                return 0;
            }
        } while ( it->current_bucket->counter == 0 );
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

