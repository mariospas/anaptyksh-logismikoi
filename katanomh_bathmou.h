#ifndef _KATANOMH_
#define _KATANOMH_

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "entries.h"
#include "hash_table.h"

//#####katanomh############

typedef struct katanomh *ptr_katanomh;

int match_friend(void *a,void *key);

ptr_katanomh create_data(int arithmos_filon);

void manage_list(list_ptr list,int arithmos_filon);

void degreeDistribution(ptr_graph g);

//####diametros#####

void return_maxN(Result_ptr result,int size,int* max);

int diameter(ptr_graph g);

//############mesh timh monopation########

void sum_from_result(Result_ptr result,int size,double *sum);

double averagePathLength(ptr_graph g);


//#########plithos sunektikon grafimaton######


int match_id(void *a,void *key);

int node_exist(list_ptr list,ptr_entry node);

void rec_search_dfs(ptr_graph g,list_ptr list,ptr_entry node,int *size);

int numberOfCCs(ptr_graph g);

int maxCC(ptr_graph g);











#endif /* _KATANOMH_ */
