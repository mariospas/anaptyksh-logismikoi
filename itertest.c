#include <stdio.h>

#include "linked_list.h"
#include "hash_table.h"

char *array[10] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
};

size_t hash( int key, size_t size );

//#if 0
int main(void)
{
    list_ptr list = LL_create( NULL );
    LL_iter_ptr it;
    int i;

    for ( i = 0; i < 10; ++i ) {
        LL_insert( list, array[i] );
    }

    it = LL_iter_create( list );
    do {
        printf( "%s\n", (char*) LL_iter_data( it ) );
    } while ( LL_iter_next( it ) );

    LL_iter_destroy( it );
    LL_destroy( list, NULL );
}
//#endif

#if 0
int main(void)
{
    ht_ptr table = HT_create( 2, 2, hash );
    HT_iter_ptr it;
    int i;

    for ( i = 0; i < 10; ++i ) {
        HT_insert( table, array[i], i );
    }

    HT_print(table);
    fputc( '\n', stdout );

    it = HT_iter_create( table );
    do {
        printf( "%s\n", (char*) HT_iter_data( it ) );
    } while ( HT_iter_next( it ) );

    HT_iter_destroy( it );
    HT_destroy( table, NULL );
}
#endif

size_t hash( int key, size_t size )
{
    /*
    return key;
    */
    return ( key * key + 3 ) * 4;
}
