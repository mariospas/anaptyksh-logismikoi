#ifndef _LINKED_LIST_
#define _LINKED_LIST_

struct linked_list;
typedef struct linked_list *list_ptr;
typedef struct LL_iterator *LL_iter_ptr;

/* Interface */
list_ptr LL_create( int (*match)( const void *a, const void *b) );

void LL_destroy( list_ptr this, void (*destroy)(void*) );

void LL_insert( list_ptr this, void *element );

void *LL_delete( list_ptr this, void *key );

void *LL_pop( list_ptr this);

void *LL_search( list_ptr this, void *key );

void **LL_export( list_ptr this );

int LL_size( list_ptr this );

list_ptr LL_copy( list_ptr this );

/* Iterator */
LL_iter_ptr LL_iter_create( list_ptr this );

void *LL_iter_data( LL_iter_ptr it );

int LL_iter_next( LL_iter_ptr it );

void LL_iter_reset( LL_iter_ptr it );

void LL_iter_destroy( LL_iter_ptr it );

#endif /* _LINKED_LIST_ */
