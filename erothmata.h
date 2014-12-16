#include "database.h"

typedef struct Matches *ptr_matches;

typedef struct array_matches *ptr_array_matches;

ptr_matches matchSuggestion(ptr_entry n, int k, int h, int x, int limit, ptr_graph g);
