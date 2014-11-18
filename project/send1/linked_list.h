#ifndef _LINKED_LIST_
#define _LINKED_LIST_

struct linked_list;
typedef struct linked_list *list_ptr;

list_ptr LL_create( int (*match)( const void *a, const void *b) );

void LL_destroy( list_ptr this, void (*destroy)(void*) );

void LL_insert( list_ptr this, void *element );

void *LL_delete( list_ptr this, void *key );

void *LL_search( list_ptr this, void *key );

void **LL_export( list_ptr this );

int LL_size( list_ptr this );

#endif /* _LINKED_LIST_ */
