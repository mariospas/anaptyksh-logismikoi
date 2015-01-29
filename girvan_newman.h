#include <stdlib.h>

#include "graph.h"

struct Communities {
    size_t size;
    struct community **array;
};

struct community {
    int id;
    size_t size;
    int *members;
};

struct Communities *girvan_newman_method( double limit, ptr_graph g );
