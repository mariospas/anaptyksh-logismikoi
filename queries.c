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


ptr_array_matches matchSuggestion(ptr_entry node, int commonInterest, int hops, int ageDiff, int limit, ptr_graph graph)
{
	int location_node = location_of_entry(node);
	int work = last_work_or_study_of_entry(node,"person_workAt_organisation.csv");
	int study = last_work_or_study_of_entry(node,"person_studyAt_organisation.csv");
	int koinaEndiaf;
	int gap;
	int same_sex;
	int apostash;

	int location_node2;
	int work2;
	int study2;

	int i, graph_size = Graph_size(graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(graph);
	ptr_entry data;

	ptr_array_matches array = malloc(sizeof(struct array_matches));

	array->pinakas = malloc(limit * sizeof(ptr_matches));


	iter = HT_iter_create(nodes);
	printf("In Diameter\n");

	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));

		location_node2 = location_of_entry(data);
		work2 = last_work_or_study_of_entry(data,"person_workAt_organisation.csv");
		study2 = last_work_or_study_of_entry(data,"person_studyAt_organisation.csv");
		koinaEndiaf = common_interests_two_entries(node,data);
		gap = generation_gap(node,data);
		same_sex = same_gender(node,data);

		if(work == -1)
		{
			if((location_node == location_node2)
				&& (study == study2)
				&& (koinaEndiaf > commonInterest)
				&& (gap <= ageDiff)
				&& (same_sex)
				&& (apostash <= hops) )
			{

			}
		}
		else if(study == -1)
		{
			if((location_node == location_node2)
				&& (work == work2)
				&& (koinaEndiaf > commonInterest)
				&& (gap <= ageDiff)
				&& (same_sex)
				&& (apostash <= hops) )
			{

			}
		}


		HT_iter_next(iter);
	}

	HT_iter_destroy(iter);




}



