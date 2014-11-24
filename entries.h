#ifndef _ENTRIES_
#define _ENTRIES_


typedef struct entry *ptr_entry;


typedef struct edge *ptr_edge;


ptr_entry create_entry(int id,void* properties);    //create node

ptr_edge create_edge(int id, void* lista_idiotiton);


#endif /* _ENTRIES_ */
