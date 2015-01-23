#include "prejob.h"


ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N)
{
	int i, graph_size = Graph_size(forum_graph);
	HT_iter_ptr iter;
	ht_ptr nodes = Graph_nodes(forum_graph);
	ptr_entry data;

	ptr_array_matches array = create_array_match(N);
	ptr_matches match;
	double similarity = 0.0;

	iter = HT_iter_create(nodes);
	//printf("In matchSuggestion\n");
	list_ptr member_list;
	double members;

	for(i=0;i<graph_size;i++)
	{
		data = ((ptr_entry)HT_iter_data(iter));
		member_list = type_list(data,"forum_hasMember_person.csv");

		if(member_list == NULL) continue;

		members = LL_size(member_list);
		match = create_match(data->id,members);
		insert_match(array,match);


		HT_iter_next(iter);
	}
	HT_iter_destroy(iter);

	return array;
}
