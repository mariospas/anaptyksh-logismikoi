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
	int current_size;
	int limit;
	ptr_matches* pinakas;
};


ptr_matches create_match(int id,int similarity)
{
	ptr_matches match = malloc(sizeof(struct Matches));

	match->id = id;
	match->similarity_score = similarity;

	return match;
}

void delete_match(ptr_matches match)
{
	free(match);
}

void insert_match (ptr_array_matches array,ptr_matches match)
{

	if( (array->current_size) < (array->limit) )
	{
		array->current_size++;
		array->pinakas[array->current_size] = match;
		printf("array match id = %d and match id = %d\n",(array->pinakas[array->current_size])->id,match->id);
		printf("current = %d\n",array->current_size);

		qsort (array->pinakas, ((array->current_size)+1), sizeof(struct Matches), compare_match);
		printf("array match id = %d and match id = %d\n",(array->pinakas[array->current_size])->id,match->id);
	}
	else if( (array->current_size) == (array->limit) )
	{
		if(compare_match(array->pinakas[array->limit],match) > 0 )
		{
			delete_match(array->pinakas[array->limit]);
			array->pinakas[array->limit] = match;
			printf("array match id = %d and match id = %d\n",(array->pinakas[array->current_size])->id,match->id);

			qsort (array->pinakas, ((array->current_size)+1), sizeof(struct Matches), compare_match);
			printf("array match id = %d and match id = %d\n",(array->pinakas[array->current_size])->id,match->id);
		}
	}
}

int compare_match (const void * a, const void * b)
{
	ptr_matches match1 = ((ptr_matches) a);
	ptr_matches match2 = ((ptr_matches) b);

	int simil1 = match1->similarity_score;
	int simil2 = match2->similarity_score;

	return ( simil2 - simil1 );
}


ptr_array_matches create_array_match(int limit)
{
	ptr_array_matches array = malloc(sizeof(struct array_matches));

	array->pinakas = malloc(limit * sizeof(ptr_matches));
	array->current_size = -1;
	array->limit = limit - 1;

	return array;
}


void delete_array_matches(ptr_array_matches array)
{
	int limit = array->limit;
	int i;

	for(i=0;i<=limit;i++)
	{
		delete_match(array->pinakas[i]);
	}
	free(array);
}

int get_match(int pos,ptr_array_matches array)
{
	if(pos > array->current_size)
	{
		printf("Wrong pos !!!\n");
		return -1;
	}
	printf("In get_match\n");
	printf("ID = %d\n",(array->pinakas[pos])->id);

	return ((array->pinakas[pos])->id);
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

	ptr_array_matches array = create_array_match(limit);
	ptr_matches match;
	int similarity = 0;

	iter = HT_iter_create(nodes);
	printf("In matchSuggestion\n");

	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));
		printf("Node id = %d and Data id = %d\n",node->id,data->id);

		location_node2 = location_of_entry(data);
		//printf("location\n");
		work2 = last_work_or_study_of_entry(data,"person_workAt_organisation.csv");
		//printf("work\n");
		study2 = last_work_or_study_of_entry(data,"person_studyAt_organisation.csv");
		//printf("study\n");
		koinaEndiaf = common_interests_two_entries(node,data);
		//printf("koinaEndiaf\n");
		gap = generation_gap(node,data);
		//printf("gap\n");
		same_sex = same_gender(node,data);
		//printf("same_sex\n");
		apostash = 1;//reachNode1(graph,node->id,data->id);
		//printf("apostash\n");


		if(work == -1)
		{
			if((location_node == location_node2)
				&& (study == study2)
				&& (koinaEndiaf > commonInterest)
				&& (gap <= ageDiff)
				&& (same_sex)
				&& (apostash <= hops) )
			{
				printf("*****insert one match data id = %d\n",data->id);
				similarity = i;
				match = create_match(data->id,similarity);

				insert_match(array,match);
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
				printf("*****insert one match data id = %d\n",data->id);
				similarity = i;
				match = create_match(data->id,similarity);

				insert_match(array,match);
			}
		}


		HT_iter_next(iter);
	}

	HT_iter_destroy(iter);

	return array;
}



