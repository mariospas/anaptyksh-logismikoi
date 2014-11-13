#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 

#include "hash_table.h"

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


ht_ptr HT_create( int table_size, int bucket_size )
{
	int i, j;
	ht_ptr this = malloc( sizeof( struct hash_table) );

	this->level = 0;
	this->next = 0;
    this->init_size = table_size;
    this->size = table_size;
    this->bucket_size = bucket_size;
	this->buckets = malloc( this->init_size * sizeof(struct bucket *) );         ///mhpos thelei ena asteri

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
    int i, index, hashed = h( (void*) &key );
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
        ; /////////////////////////////////////////////////???

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

    qsort( (*found)->records, (*found)->counter, sizeof(struct record), record_compare );
}           

void *HT_search( ht_ptr this, int id, hash_f hash )
{
    int index, key = hash( (void*)& id);
    struct bucket *found;
    void *result;

    /* Apply small hashing first */
    index = key % ( pow_(2, this->level) * this->init_size );

    /* If it's split investigate further */
    if ( index < this->next ) {
        index = key % ( pow_(2, this->level + 1) * this->init_size );
    }

    found = this->buckets[ index ];
    result = bsearch( (void*) &key, found->records, found->counter, sizeof(struct record), record_match );

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
