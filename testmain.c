#include <stdlib.h>

#include "hash_table.h"

int hashing( const void *key );

int main( int argc, char *argv[] )
{
    int i, max = atoi( argv[1] );
    ht_ptr sample = HT_create( 4, 4 );

    for ( i = 1; i <= max; ++i ) {
        HT_insert( sample, hashing, NULL, i );
    }

    HT_print( sample );
    HT_destroy( sample, NULL );

    return EXIT_SUCCESS; /* I hope */
}

int hashing( const void *key )
{
    return *( (int*) key );
}
