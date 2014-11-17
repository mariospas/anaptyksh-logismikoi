#ifndef _HASH_TABLE_
#define _HASH_TABLE_

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

typedef struct hash_table *ht_ptr;

typedef int (*hash_f)(int key, int size);
typedef void (*destroyer)(void *object);

/* Interface */
ht_ptr HT_create( int table_size, int bucket_size );

void HT_destroy( ht_ptr this, destroyer destroy );

void HT_insert( ht_ptr this, hash_f h, void *element, int key );

void *HT_search( ht_ptr this, int id, hash_f hash );

/* Helpers */
static void rec_bucket_destroy_( struct bucket *this, destroyer destroy );

static void redistribute( ht_ptr this, struct bucket **a, struct bucket **b );

static void clear_bucket( struct bucket *b, int size );

static int record_compare( const void *a, const void *b );

static int record_match( const void *key, const void *object );

static int pow_( int base, int exp );

/* Debug */
/*
void HT_print( ht_ptr this );
*/

#endif /* _HASH_TABLE_ */
