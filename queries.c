#include "queries.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"

struct Matches
{
	int id;
	int similarity_score;
};

struct array_matches
{
	ptr_matches* pinakas;
};


ptr_matches create_match(int id,int similarity)
{
	ptr_matches match = malloc(sizeof(struct Matches));

	match->id = id;
	match->similarity_score = similarity;

	return match;
}

#if 0
ptr_array_matches matchSuggestion(ptr_entry n, int k, int h, int x, int limit, ptr_graph g)
{
	int location_node;
	int i;
	ptr_entry current;
	ptr_entry node;
	HT_iter_ptr iter;
	ptr_array_matches array = malloc(sizeof(struct array_matches));

	array->pinakas = malloc(limit * sizeof(ptr_matches));




}

#endif

