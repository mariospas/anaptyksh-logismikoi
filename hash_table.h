#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include <stdlib.h>

struct bucket;
struct record;

typedef struct hash_table *ht_ptr;
typedef struct HT_iterator *HT_iter_ptr;

typedef size_t (*hash_f)(int key, size_t size);
typedef void (*destroyer)(void *object);

/* Interface */
ht_ptr HT_create( int table_size, int bucket_size, hash_f hash );

void HT_destroy( ht_ptr this, destroyer destroy );

void HT_insert( ht_ptr this, void *element, int key );

void *HT_search( ht_ptr this, int id );

/* Helpers */
static void rec_bucket_destroy_( struct bucket *this, destroyer destroy );

static void rec_bucket_sort( struct bucket *this );

static void redistribute( ht_ptr this, struct bucket **a, struct bucket **b );

static void clear_bucket_( struct bucket *b, int size );

static int record_compare( const void *a, const void *b );

static int record_match( const void *key, const void *object );

static int pow_( int base, int exp );

/* Accessors */
int HT_size( ht_ptr this );

int HT_bsize( ht_ptr this );

/* Iterator */
HT_iter_ptr HT_iter_create( ht_ptr this );

void *HT_iter_data( HT_iter_ptr iterator );

int HT_iter_next( HT_iter_ptr iterator );

void HT_iter_reset( HT_iter_ptr it );

void HT_iter_destroy( HT_iter_ptr iterator );

/* Debug */
/*
void HT_print( ht_ptr this );
*/

#endif /* _HASH_TABLE_ */
