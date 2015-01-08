#ifndef _QUERIES_
#define _QUERIES_

#include "database.h"

typedef struct Matches *ptr_matches;

typedef struct array_matches *ptr_array_matches;


ptr_array_matches matchSuggestion(ptr_entry node, int commonInterest, int hops, int ageDiff, int limit, ptr_graph graph);


#endif /* _QUERIES_ */
