#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "linked_list.h"

struct linked_list {
	int size;
	struct LL_node *head;
	int (*match)( const void *a, const void *b );
};

struct LL_node {
	void *data;
	struct LL_node *next;
};

/* Basically same implementation as node */
struct LL_iterator {
    list_ptr base;
    void *data;
    struct LL_node *next;
};

/* Constructor; will return a fully allocated pointer */
list_ptr LL_create( int (*match)( const void *a, const void *b) )
{
	list_ptr this = malloc( sizeof(struct linked_list) );
	
	this->size = 0;
	this->head = NULL;
	this->match = match;
	return this;
}

/* Deconstructor */
void LL_destroy( list_ptr this, void (*destroy)(void*) )
{
	assert(this != NULL);
	struct LL_node *node = this->head, *next;
	while ( node != NULL ) {
		next = node->next;
		if ( destroy != NULL ) {
			destroy( node->data );
		}
		free( node );
		node = next;
	}
	free( this );
}

void LL_insert( list_ptr this, void *element )
{
	struct LL_node **node = &this->head;
    if ( *node != NULL ) {
        do {
            node = &(*node)->next;
        } while ( *node != NULL );
    }
	*node = malloc( sizeof(struct LL_node) );

	(*node)->data = element;
	(*node)->next = NULL;
    ++this->size;
}

void *LL_delete( list_ptr this, void *key )
{
	struct LL_node *node = this->head, *temp;
	void *result;
	if ( node == NULL
	|| ( this->match( node->data, key ) != 0 && node->next == NULL ) ) {
		return NULL;
	}
    if ( this->match( node->data, key ) == 0 ) {
        result = this->head->data;
        temp = this->head;
        this->head = this->head->next;
        free( temp );
        --this->size;
        return result;
    }
	while ( this->match( node->next->data, key ) != 0 ) {
		node = node->next;
		if ( node->next == NULL ) {
			return NULL;
		}
	}
	temp = node->next;
	node->next = temp->next;
	result = temp->data;
	free( temp );
	--this->size;
	return result;
}

void *LL_search( list_ptr this, void *key )
{
	struct LL_node *node = this->head;
	if ( node == NULL ) {
		return NULL;
	}
	while ( this->match( node->data, key ) != 0 ) {
		node = node->next;
		if ( node == NULL ) {
			return NULL;
		}
	}
	return node->data;
}

/* Returns an allocated array with the contents of the list */
void **LL_export( list_ptr this )
{
	int i;
	struct LL_node *node;
	void **result = malloc( this->size * sizeof(void *) );

	for ( i = 0, node = this->head;  i < this->size; ++i, node = node->next ) {
		assert( node != NULL );
		result[i] = node->data;
	}
	return result;
}

int LL_size( list_ptr this )
{
    return this->size;
}

LL_iter_ptr LL_iter_create( list_ptr list )
{
    assert( list->size > 0 );
    LL_iter_ptr it = malloc( sizeof( struct LL_iterator ) );
    it->base = list;
    it->data = it->base->head->data;

    it->next = it->base->head->next;

    return it;
}

void *LL_iter_data( LL_iter_ptr it )
{
    return it->data;
}

int LL_iter_next( LL_iter_ptr it )
{
    if ( it->next == NULL ) {
        return 0;
    }
    it->data = it->next->data;
    it->next = it->next->next;

    return 1;
}

void LL_iter_reset( LL_iter_ptr it )
{
    it->data = it->base->head->data;
    it->next = it->base->head->next;
}

void LL_iter_destroy( LL_iter_ptr it )
{
    free( it );
}
