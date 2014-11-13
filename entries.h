#ifndef _ENTRIES_
#define _ENTRIES_


typedef struct entry *ptr_entry;


typedef struct edge *ptr_edge;


ptr_entry create_entry(int id,void* properties,int (*match)( const void *a, const void *b));    //create node

void destroy_entry(ptr_entry this);

ptr_edge create_edge(int id, void* lista_idiotiton);

void destroy_edge(ptr_edge this);


#endif /* _ENTRIES_ */
