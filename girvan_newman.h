#include <stdlib.h>

#include "graph.h"
#include "linked_list.h"

struct Communities {
    size_t size;
    struct community **array;
};

struct community {
    int id;
    size_t size;
    int *members;
};

list_ptr girvan_newman_method( double limit, ptr_graph g );

list_ptr GI_numberOfCCs(ptr_graph g);
