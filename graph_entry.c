#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph_entry.h"
#include "linked_list.h"
#include <math.h>


ptr_data_list data_list_create(char *special_id)
{
	ptr_data_list data = malloc(sizeof(struct data_of_list));

	data->list = LL_create(match_edge);
	data->special_id = strdup(special_id);

	return data;
}

void destroy_data_of_list(void *data)
{
	ptr_data_list data_list = (ptr_data_list) data;
	if(data_list != NULL)
	{
		if(data_list->special_id != NULL) free(data_list->special_id);
		LL_destroy(data_list->list,destroy_edge);
	}
}


int match_data_list(const void* a,const void* special_key)
{
	char* special_id = (char *) special_key;
	ptr_data_list data = (ptr_data_list) a;

	if(strcmp(data->special_id,special_id) == 0)
	{
		return 0;
	}
	else return 1;

}


ptr_entry create_entry(int id,void* properties, deallocator_f properties_destroy )      //create node
{
	ptr_entry node;

	node = malloc(sizeof(struct entry));

	node->id = id;
	node->properties = properties;
	node->edges = LL_create(match_data_list);
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
	LL_destroy(entry1->edges,destroy_data_of_list);
	free(entry1);
}

ptr_edge create_edge(char edge_type[EDGE_TYPE_BUF], int target_id, int target_type, double weight, void *extra  )
{
	ptr_edge new_edge = malloc(sizeof(struct edge));

    strcpy( new_edge->edge_type, edge_type );
	new_edge->target_id = target_id;
	new_edge->target_type = target_type;
	new_edge->weight = weight;
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


int match_entry(const void *a,const void *key)
{
	if( ((ptr_entry)a)->id == ((ptr_entry)key)->id  )
	{
		return 0;
	}
	else return 1;
}


void edge_change_weight(ptr_edge edge,double weight)
{
	edge->weight = weight;
}


//Iterators


void* Entry_take_list(ptr_entry this)
{
	return this->edges;
}

int size_of_friend_list(ptr_entry this)
{
	int size = 0;
	ptr_data_list data_list;

	data_list = LL_search(this->edges,"person_knows_person.csv");

	size = LL_size(data_list->list);

	return size;
}


int location_of_entry(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = type_list(node,"person_isLocatedIn_place.csv");
	if(list == NULL) return -1;

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

	list = type_list(node,target_type);
	if(list == NULL)
	{
		return last_work;
	}

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


int work_or_study_in_same_place(ptr_entry node1,ptr_entry node2,char *target_type)
{
	int com_int = 0;

	list_ptr list1 = type_list(node1,target_type);
	//print_list(list1);
	list_ptr list2 = type_list(node2,target_type);
	//print_list(list2);
	LL_iter_ptr iterList1;
	LL_iter_ptr iterList2;
	ptr_edge data1;
	ptr_edge data2;


	if(LL_size(list1) == 0 || LL_size(list2) == 0)
	{
		return com_int;
	}


	iterList1 = LL_iter_create(list1);

	iterList2 = LL_iter_create(list2);

	data1 = LL_iter_data(iterList1);
	data2 = LL_iter_data(iterList2);
	if(strcmp(data1->edge_type,target_type) == 0)
	{
		if(strcmp(data2->edge_type,target_type) == 0)
		{
			if(data2->target_id == data1->target_id) com_int++;
		}
		while(LL_iter_next(iterList2))
		{
			data2 = LL_iter_data(iterList2);
			if(strcmp(data2->edge_type,target_type) == 0)
			{
				if(data2->target_id == data1->target_id) com_int++;
			}
		}
		LL_iter_reset(iterList2);

	}

	while(LL_iter_next(iterList1))
	{
		data1 = LL_iter_data(iterList1);
		data2 = LL_iter_data(iterList2);
		if(strcmp(data1->edge_type,target_type) == 0)
		{
			if(strcmp(data2->edge_type,target_type) == 0)
			{
				if(data2->target_id == data1->target_id) com_int++;
			}
			while(LL_iter_next(iterList2))
			{
				data2 = LL_iter_data(iterList2);
				if(strcmp(data2->edge_type,target_type) == 0)
				{
					if(data2->target_id == data1->target_id) com_int++;
				}
			}
			LL_iter_reset(iterList2);

		}

	}


	LL_iter_destroy(iterList1);
	LL_iter_destroy(iterList2);

	return com_int;
}



void* type_list(ptr_entry node,char *type)
{

	void* new_list;
	ptr_data_list data_list;

	data_list = LL_search(node->edges,type);
	if(data_list == NULL)
	{
		//printf("return NULL %s\n",type);
		return NULL;
	}
	else new_list =((void *) (data_list->list));


	return new_list;
}


int common_interests_two_entries(ptr_entry node1,ptr_entry node2,int *interest1,int *interest2)
{
	int com_int = 0;

	list_ptr list1 = type_list(node1,"person_hasInterest_tag.csv");
	//print_list(list1);
	list_ptr list2 = type_list(node2,"person_hasInterest_tag.csv");
	//print_list(list2);
	LL_iter_ptr iterList1;
	LL_iter_ptr iterList2;
	ptr_edge data1;
	ptr_edge data2;


	//printf("size1 = %d size2 = %d\n",LL_size(list1),LL_size(list2));
	if((list1 == NULL) || (list2 == NULL))
	{
		//printf("size1 = %d size2 = %d\n",LL_size(list1),LL_size(list2));
		*interest1 = 0;
		*interest2 = 0;
		return com_int;
	}

	*interest1 = LL_size(list1);
	*interest2 = LL_size(list2);

	printf("HERe again\n");
	iterList1 = LL_iter_create(list1);

	iterList2 = LL_iter_create(list2);

	data1 = LL_iter_data(iterList1);
	data2 = LL_iter_data(iterList2);
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
		data1 = LL_iter_data(iterList1);
		data2 = LL_iter_data(iterList2);
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

	}


	LL_iter_destroy(iterList1);
	LL_iter_destroy(iterList2);

	return com_int;

}


int generation_gap(ptr_entry node1,ptr_entry node2)
{
	int gap = -1;

	int year1;
	size_t month1;
	size_t day1;
	size_t hour1;
	size_t minute1;
	size_t second1;

	int year2;
	size_t month2;
	size_t day2;
	size_t hour2;
	size_t minute2;
	size_t second2;

	ptr_person_info p_info1 = ((ptr_person_info)(node1->properties));
	ptr_date date1 = p_info1->birthday;

	ptr_person_info p_info2 = ((ptr_person_info)(node2->properties));
	ptr_date date2 = p_info2->birthday;

	get_date(date1,((size_t *) &year1),&month1,&day1,&hour1,&minute1,&second1);
	get_date(date2,((size_t *) &year2),&month2,&day2,&hour2,&minute2,&second2);

	gap = year1 - year2;
	gap = abs(gap);

	return gap;

}

int same_gender(ptr_entry node1,ptr_entry node2)
{
	ptr_person_info p_info1 = ((ptr_person_info)(node1->properties));
	gender_t gender1 = p_info1->gender;

	ptr_person_info p_info2 = ((ptr_person_info)(node2->properties));
	gender_t gender2 = p_info2->gender;

	return (gender1 == gender2);
}



int creator_of_post(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = type_list(node,"post_hasCreator_person.csv");

	if(list != NULL)
	{

		iterList = LL_iter_create(list);

		data = LL_iter_data(iterList);
		if(strcmp(data->edge_type,"post_hasCreator_person.csv") == 0)
		{
			LL_iter_destroy(iterList);
			return data->target_id;
		}
		while(LL_iter_next(iterList))
		{
			data = LL_iter_data(iterList);
			if(strcmp(data->edge_type,"post_hasCreator_person.csv") == 0)
			{
				LL_iter_destroy(iterList);
				return data->target_id;
			}
		}
	}

	return -1;   //an den brei tpt
}



int creator_of_comments(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = type_list(node,"comment_hasCreator_person.csv");

	if(list != NULL)
	{

		iterList = LL_iter_create(list);

		data = LL_iter_data(iterList);
		if(strcmp(data->edge_type,"comment_hasCreator_person.csv") == 0)
		{
			LL_iter_destroy(iterList);
			return data->target_id;
		}
		while(LL_iter_next(iterList))
		{
			data = LL_iter_data(iterList);
			if(strcmp(data->edge_type,"comment_hasCreator_person.csv") == 0)
			{
				LL_iter_destroy(iterList);
				return data->target_id;
			}
		}
	}

	return -1;   //an den brei tpt
}



int there_is_in_forum_member_list(ptr_entry node,int id_person)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = type_list(node,"forum_hasMember_person.csv");

	if(list != NULL)
	{

		iterList = LL_iter_create(list);

		data = LL_iter_data(iterList);
		if(id_person == data->target_id)
		{
			LL_iter_destroy(iterList);
			return 1;
		}
		while(LL_iter_next(iterList))
		{
			data = LL_iter_data(iterList);
			if(id_person == data->target_id)
			{
				LL_iter_destroy(iterList);
				return 1;
			}
		}
	}

	return 0;

}


int there_is_in_tag_list(ptr_entry node,int tag_id)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;

	list = type_list(node,"person_hasInterest_tag.csv");

	if(list != NULL)
	{

		iterList = LL_iter_create(list);

		data = LL_iter_data(iterList);
		if(tag_id == data->target_id)
		{
			LL_iter_destroy(iterList);
			return 1;
		}
		while(LL_iter_next(iterList))
		{
			data = LL_iter_data(iterList);
			if(tag_id == data->target_id)
			{
				LL_iter_destroy(iterList);
				return 1;
			}
		}
	}

	return 0;

}




void print_list_of_edges(ptr_entry node)
{
	list_ptr list;
	LL_iter_ptr iterList;
	ptr_edge data;
	ptr_data_list data_list;
	LL_iter_ptr iter;

	list = ((list_ptr) Entry_take_list(node));
	if(list != NULL && LL_size(list) != 0)
	{

		iterList = LL_iter_create(list);

		data_list = LL_iter_data(iterList);

		if(data_list->list != NULL && LL_size(data_list->list) != 0)
		{
			iter = LL_iter_create(data_list->list);
			data = LL_iter_data(iter);
			printf("Edge ID = %d and Type = %s and Target_type = %d and WEIGHT = %f\n",data->target_id,data->edge_type,data->target_type,data->weight);
			while(LL_iter_next(iter))
			{
				data = LL_iter_data(iter);
				printf("Edge ID = %d and Type = %s and Target_type = %d and WEIGHT = %f\n",data->target_id,data->edge_type,data->target_type,data->weight);
			}
			LL_iter_destroy(iter);

			while(LL_iter_next(iterList))
			{
				data_list = LL_iter_data(iterList);
				iter = LL_iter_create(data_list->list);
				data = LL_iter_data(iter);
				printf("Edge ID = %d and Type = %s and Target_type = %d and WEIGHT = %f\n",data->target_id,data->edge_type,data->target_type,data->weight);
				while(LL_iter_next(iter))
				{
					data = LL_iter_data(iter);
					printf("Edge ID = %d and Type = %s and Target_type = %d and WEIGHT = %f\n",data->target_id,data->edge_type,data->target_type,data->weight);
				}
				LL_iter_destroy(iter);
			}

			LL_iter_destroy(iterList);
		}
	}
}

void print_list(list_ptr list)
{
	LL_iter_ptr iterList;
	ptr_edge data;

	iterList = LL_iter_create(list);

	data = LL_iter_data(iterList);
	printf("*****Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	while(LL_iter_next(iterList))
	{
		data = LL_iter_data(iterList);
		printf("*****Edge ID = %d and Type = %s\n",data->target_id,data->edge_type);
	}

	LL_iter_destroy(iterList);
}
