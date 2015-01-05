#ifndef _GRAPH_STATS_
#define _GRAPH_STATS_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "graph_entry.h"
#include "hash_table.h"
#include "linked_list.h"

/********** GRAPH DISTRIBUTION ***********/

typedef struct katanomh *ptr_katanomh;

int match_friend( const void *a, const void *key);

ptr_katanomh create_data(int arithmos_filon);

void destroy_data(void* data);

void manage_list(list_ptr list,int arithmos_filon);

void degreeDistribution(ptr_graph g);

/********** GRAPH DIAMETER ***********/

void return_maxN(Result_ptr result,int size,int* max);

int diameter(ptr_graph g);

/********** AVERAGE PATH LENGTH ***********/

void sum_from_result(Result_ptr result,int size,double *sum);

int averagePathLength(ptr_graph g,double *apotel);

/********** COHERENT GRAPHS ***********/

typedef struct dataCC *dataCC_ptr;

dataCC_ptr create_dataCC(int id);

void destroy_dataCC(void* data);

int match_id( const void *a, const void *key);

int node_exist(list_ptr list,ptr_entry node);

void rec_search_dfs(ptr_graph g,list_ptr list,ptr_entry node,int *size);

int numberOfCCs(ptr_graph g);

int maxCC(ptr_graph g);

double density(ptr_graph g,double *d);

/********** CENTRALITY *************/

double closeness_centrality( ptr_entry n, ptr_graph g );

double betweenness_centrality( ptr_entry n, ptr_graph g );

#endif /* _GRAPH_STATS_ */

