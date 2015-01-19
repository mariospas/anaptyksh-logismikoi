#ifndef _QUERIES_
#define _QUERIES_

#include "database.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Matches *ptr_matches;

typedef struct array_matches *ptr_array_matches;

typedef struct trend *ptr_trend;

typedef struct array_trends *ptr_array_trends;


ptr_matches create_match(int id,double similarity);

void delete_match(ptr_matches match);

void insert_match (ptr_array_matches array,ptr_matches match);

int compare_match (const void * a, const void * b);

ptr_array_matches create_array_match(int limit);

void delete_array_matches(ptr_array_matches array);

int get_match(int pos,ptr_array_matches array,double *score);



ptr_array_matches matchSuggestion(ptr_entry node, int commonInterest, int hops, int ageDiff, int limit, ptr_graph graph);

ptr_graph Create_Stalkers_Graph(int stalkersNum,int likesNumber,int centralityMode,ptr_database database);

ptr_graph getTopStalkers(int stalkersNum,int likesNumber,int centralityMode,ptr_database database,ptr_array_matches stalkersCloseCentr);




ptr_trend create_trend(int size,char* tag_name);

void delete_trend(ptr_trend trend);

void insert_trend (ptr_array_trends array,ptr_trend trend);

int compare_trend (const void * a, const void * b);

ptr_array_trends create_array_trends(int limit);

void delete_array_trends(ptr_array_trends array);

char* get_trend_name(int pos,ptr_array_trends array,int *size);


void findTrends(int trendsNum,ptr_database database,char** womenTrends,char** menTrends);




double trust(ptr_entry node_i,ptr_entry node_j,ptr_graph graph,ptr_graph post_graph,ptr_graph comment_graph);

ptr_graph buildTrustGraph(int forumID,ptr_database database);

double estimateTrust(ptr_entry a, ptr_entry b, ptr_graph trust_graph);



typedef struct data_trust *ptr_data_trust;

double data_trust_get_trust(ptr_data_trust data);

int data_trust_get_id(ptr_data_trust data);

int data_trust_get_level(ptr_data_trust data);

int match_trust(const void *a, const void *key);

ptr_data_trust create_data_trust(int id,double trust_so_far,int level);

void destroy_data_trust(void* data);

int check_to_insert_in_list(list_ptr fringe,ptr_edge edge,double trust_so_far,int level);

double rec_trust_search(ptr_graph trust_graph,list_ptr fringe,int target_id,int *flag);


#endif /* _QUERIES_ */
