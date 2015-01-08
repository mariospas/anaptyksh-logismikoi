#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph_entry.h"
#include "linked_list.h"
#include "dataset_handlers.h"

ptr_entry create_entry(int id,void* properties, deallocator_f properties_destroy )      //create node
{
	ptr_entry node;

	node = malloc(sizeof(struct entry));

	node->id = id;
	node->properties = properties;
	node->edges = LL_create(match_edge);
	node->properties_destroy = properties_destroy;

	return node;
}

void destroy_entry(void *entry )
{
    ptr_entry entry1 = (ptr_entry) entry;
	if(entry1->properties != NULL)
	{
        entry1->properties_destroy( entry1->properties );
	}
	free(entry1);
}

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, int weight, void *extra  )
{
	ptr_edge new_edge = malloc(sizeof(struct edge));

    strcpy( new_edge->edge_type, edge_type );
	new_edge->target_id = target_id;
	new_edge->target_type = target_type;
	new_edge->weight = 0;
	new_edge->extra_data = extra;

	return new_edge;
}


void destroy_edge(void *edge)
{
	ptr_edge data = ((ptr_edge) edge);
	if(strcmp(data->edge_type,"person_workAt_organisation.csv") == 0)
	{
		free(data->extra_data);
	}
	else if(strcmp(data->edge_type,"person_studyAt_organisation.csv") == 0)
	{
		free(data->extra_data);
	}
	else if(strcmp(data->edge_type,"person_likes_post.csv") == 0)
	{
		free(data->extra_data);
	}
	free(edge);
}


int match_edge(const void *a, const void *b)
{
    ptr_edge a1 = (ptr_edge) a;
    ptr_edge b1 = (ptr_edge) b;
    assert( a1 != NULL && b1 != NULL );
    if ( a1->target_type == b1->target_type
      && a1->target_id == b1->target_id
      && a1->weight == b1->weight
      && ( ! strcmp( a1->edge_type, b1->edge_type ) ) ) {
       return 0;
    }
    return -1;
}


int match_entry(void *a,void *key)
{
	if( ((ptr_entry)a)->id == ((ptr_entry)key)->id  )
	{
		return 0;
	}
	else return 1;
}






//Iterators


void* Entry_take_list(ptr_entry this)
{
	return this->edges;
}

int size_of_friend_list(ptr_entry this)
{
	int size = 0;

	size = LL_size(((list_ptr)(this->edges)));

	return size;
}


int location_of_entry(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = ((list_ptr) Entry_take_list(node));

	iterList = LL_iter_create(list);

	data = LL_iter_data(iterList);
	if(strcmp(data->edge_type,"person_isLocatedIn_place.csv") == 0)
	{
		LL_iter_destroy(iterList);
		return data->target_id;
	}
	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		if(strcmp(data->edge_type,"person_isLocatedIn_place.csv") == 0)
		{
			LL_iter_destroy(iterList);
			return data->target_id;
		}
	}

	return -1;   //an den brei tpt
}


int last_work_or_study_of_entry(ptr_entry node,char *target_type)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	size_t year;
	size_t month;
	size_t day;
	size_t hour;
	size_t minute;
	size_t second;

	size_t max_year = 1;
	int last_work = -1;

	list = ((list_ptr) Entry_take_list(node));

	iterList = LL_iter_create(list);

	data = LL_iter_data(iterList);
	if(strcmp(data->edge_type,target_type) == 0)
	{
		get_date( ((ptr_date)(data->extra_data)),&year,&month,&day,&hour,&minute,&second);
		if(max_year < year)
		{
			max_year = year;
			last_work = data->target_id;
		}
		//printf("year = %zu and work_id = %d\n",year,data->target_id);
	}
	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		if(strcmp(data->edge_type,target_type) == 0)
		{
			get_date(((ptr_date)(data->extra_data)),&year,&month,&day,&hour,&minute,&second);
			if(max_year < year)
			{
				max_year = year;
				last_work = data->target_id;
			}
			//printf("year = %zu and work_id = %d\n",year,data->target_id);
		}
	}

	LL_iter_destroy(iterList);

	return last_work;
}



list_ptr interest_list(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;
	ptr_edge new;
	list_ptr new_list;

	list = ((list_ptr) Entry_take_list(node));

	iterList = LL_iter_create(list);

	new_list = LL_create(match_edge);

	data = LL_iter_data(iterList);
	if(strcmp(data->edge_type,"person_hasInterest_tag.csv") == 0)
	{
		ptr_edge new = create_edge(data->edge_type,data->target_id,data->target_type,data->weight,data->extra_data);
		LL_insert(new_list,((void *)new));
	}

	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		if(strcmp(data->edge_type,"person_hasInterest_tag.csv") == 0)
		{
			ptr_edge new = create_edge(data->edge_type,data->target_id,data->target_type,data->weight,data->extra_data);
			LL_insert(new_list,((void *)new));
		}
	}

	LL_iter_destroy(iterList);

	return new_list;
}


int common_interests_two_entries(ptr_entry node1,ptr_entry node2)
{
	int com_int = 0;

	list_ptr list1 = interest_list(node1);
	list_ptr list2 = interest_list(node2);
	LL_iter_ptr iterList1;
	LL_iter_ptr iterList2;
	ptr_edge data1;
	ptr_edge data2;


	printf("insert in common00\n");
	//list1 = ((list_ptr) Entry_take_list(node1));
	iterList1 = LL_iter_create(list1);

	//list2 = ((list_ptr) Entry_take_list(node2));
	iterList2 = LL_iter_create(list2);

	printf("insert in common000\n");

	data1 = LL_iter_data(iterList1);
	if(strcmp(data1->edge_type,"person_hasInterest_tag.csv") == 0)
	{
		if(strcmp(data2->edge_type,"person_hasInterest_tag.csv") == 0)
		{
			if(data2->target_id == data1->target_id) com_int++;
		}
		while(LL_iter_next(iterList2))
		{
			data2 = LL_iter_data(iterList2);
			if(strcmp(data2->edge_type,"person_hasInterest_tag.csv") == 0)
			{
				if(data2->target_id == data1->target_id) com_int++;
			}
		}
		LL_iter_reset(iterList2);

	}

	while(LL_iter_next(iterList1))
	{
		printf("insert in common1\n");
		data1 = LL_iter_data(iterList1);
		if(strcmp(data1->edge_type,"person_hasInterest_tag.csv") == 0)
		{
			printf("insert in common2\n");
			if(strcmp(data2->edge_type,"person_hasInterest_tag.csv") == 0)
			{
				printf("insert in common3\n");
				if(data2->target_id == data1->target_id) com_int++;
			}
			while(LL_iter_next(iterList2))
			{
				printf("insert in common in loop2\n");
				data2 = LL_iter_data(iterList2);
				if(strcmp(data2->edge_type,"person_hasInterest_tag.csv") == 0)
				{
					printf("insert in common4\n");
					if(data2->target_id == data1->target_id) com_int++;
				}
			}
			LL_iter_reset(iterList2);

		}

	}

	printf("insert in common5\n");

	LL_iter_destroy(iterList1);
	LL_iter_destroy(iterList2);

	LL_destroy(list1,destroy_edge);
	LL_destroy(list2,destroy_edge);

	return com_int;

}



void print_list_of_edges(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = ((list_ptr) Entry_take_list(node));

	iterList = LL_iter_create(list);

	data = LL_iter_data(iterList);
	printf("Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		printf("Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	}

	LL_iter_destroy(iterList);
}
