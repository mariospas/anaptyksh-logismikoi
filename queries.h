#ifndef _QUERIES_
#define _QUERIES_

#include "database.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Matches *ptr_matches;

typedef struct array_matches *ptr_array_matches;


ptr_matches create_match(int id,double similarity);

void delete_match(ptr_matches match);

void insert_match (ptr_array_matches array,ptr_matches match);

int compare_match (const void * a, const void * b);

ptr_array_matches create_array_match(int limit);

void delete_array_matches(ptr_array_matches array);

int get_match(int pos,ptr_array_matches array);



ptr_array_matches matchSuggestion(ptr_entry node, int commonInterest, int hops, int ageDiff, int limit, ptr_graph graph);

ptr_graph getTopStalkers(int stalkersNum,int likesNumber,int centralityMode,ptr_database database,ptr_array_matches stalkersCloseCentr);

#endif /* _QUERIES_ */
