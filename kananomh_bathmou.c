#include "katanomh_bathmou.h"


//########katanomh#######################

struct katanomh
{
	int size;
	int arithmos_filon;
};


int match_friend(void *a,void *key)
{
	if( ((ptr_katanomh)a)->arithmos_filon == *((int*)key) )
	{
		(((ptr_katanomh)a)->size)++;
		return 0;
	}
	else return 1;
}


ptr_katanomh create_data(int arithmos_filon)
{
	ptr_katanomh node;
	node = malloc(sizeof(struct katanomh));

	node->arithmos_filon = arithmos_filon;
	node->size = 1;

	return node;
}

void manage_list(list_ptr list,int arithmos_filon)
{
	void* data;
	ptr_katanomh new_data;

	if(list != NULL)
	{
		data = LL_search(list,&arithmos_filon);
		if(data == NULL)
		{
			new_data = create_data(arithmos_filon);
			LL_insert(list,((void *)new_data));
		}

	}
}


void degreeDistribution(ptr_graph g)
{
	list_ptr list = LL_create(match_friend);
	int i;
	HT_iter_ptr iter;
	LL_iter_ptr iterList;
	ptr_entry data;
	list_ptr friend_list;
	int list_size;
	FILE* fp;
	ptr_katanomh dataKat;

	if(g != NULL)
	{
		iter = HT_iter_create(g->table);


		for(i=0;i < g->size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			friend_list =((list_ptr)Entry_take_list(data));
			list_size = LL_size(friend_list);                  //theoroume oti oloi einai knows ara to megethos tis listas
															//einai kai to zhtoumeno mas

			manage_list(list,list_size);

			HT_iter_next(iter);
		}

		fp = fopen("dedomena_katanomhs.txt","w+");

		iterList = LL_iter_create(list);

		for(i=0;i<LL_size(list);i++)
		{
			dataKat = (ptr_katanomh)LL_iter_data(iterList);
			fprintf(fp,"%d %f\n",dataKat->arithmos_filon,((dataKat->size)/(g->size)));

			LL_iter_next(iterList);
		}



		LL_iter_destroy(iterList);
		HT_iter_destroy(iter);

	}
}



//#################diametros#########################

void return_maxN(Result_ptr result,int size,int* max)
{
	int i;
	int id;
	int distance;

	for(i=0;i<size;i++)
	{
		ResultSet_next(result,&id,&distance,i);
		if(*max < distance)
		{
			*max = distance;
		}
	}

}


int diameter(ptr_graph g)
{
	int i;
	HT_iter_ptr iter;
	ptr_entry data;
	Result_ptr result;
	int max = -7;

	if(g != NULL)
	{
		iter = HT_iter_create(g->table);

		for(i=0;i<g->size;i++)
		{
			data = ((ptr_entry)HT_iter_data(iter));
			result = reachNodeN(g,data->id);
			return_maxN(result,g->size,&max);

			HT_iter_next(iter);
		}

		HT_iter_destroy(iter);
	}

	return max;
}


