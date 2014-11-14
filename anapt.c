
int reachNode1(ptr_graph graph,int id_first,int id_last,hash_f hash)
{
	ptr_entry first;
	ptr_entry last;
	int count = 0;
	int flag = 1;
	ptr_edge last_edge = malloc(sizeof(struct edge));
	ptr_edge prev_last_edge = malloc(sizeof(struct edge));

	first = lookupNode(graph,id_first,hash);
	last = lookupNode(graph,id_last,hash);

	if(first != NULL && last != NULL)
	{
		if(first->id == last->id) return 0;
		else
		{
			count++;
			rec_bfs(graph,id_first,first,last,hash,&count,&flag,last_edge,prev_last_edge);
		}
	}

}



int rec_bfs(ptr_graph graph,int riza,ptr_entry current,ptr_entry last,hash_f hash,int *count,int* flag,ptr_edge last_edge,ptr_edge prev_last_edge)
{
	int err;
	int i;
	ptr_edge *data_array;
	list_ptr lista_filon;
	int size;


	lista_filon = (list_ptr)current->friends;
	size = lista_filon->size;
	data_array = LL_export(lista_filon);


	err = rec_sl(current->friends,last->id,count,hash,last_edge);
	if(err == 0 && *flag == 1)
	{
		*flag = 0;
		for(i=0;i<(size-1);i++)
		{
			current = lookupNode(graph,data_array[i]->id,hash);
			rec_bfs(graph,riza,current,last,hash,count,flag);
		}
		if(prev_last_edge->id == data_array[size-1]->id)
		{
			prev_last_edge = last_edge;
			*flag = 1;
			for(i=0;i<(size-1);i++)
			{
				current = lookupNode(graph,data_array[i]->id,hash);
				rec_bfs(graph,riza,current,last,hash,count,flag);
			}
		}
	}
	else if(err == 0 && *flag == 0)
	{

	}

}





int rec_sl(list_ptr lista_filon,int id_last,int* counter,hash_f hash,ptr_edge last)
{
	int err;
	int i;
	ptr_edge *data_array;
	int size = lista_filon->size;

	memcpy(last,data_array[size-1],sizeof(struct edge));
	data_array = LL_export(lista_filon);

	for(i=0;i<(size-1);i++)
	{
		if(data_array[i]->id == id_last)
		{
			free(data_array);
			return 1;
		}
	}
	free(data_array);
	return 0;
}










