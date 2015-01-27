#pragma once

#include "queries.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>
#include "database.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"


typedef struct data_forum *ptr_data_forum;

ptr_data_forum DF_create(ptr_database database,int forum_id,ptr_forum_database forums_database);

void DF_destroy(ptr_data_forum data);


ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N);

void* Forum_graph_create_insert(void *data);

ptr_forum_database computeTopNForums(ptr_database database,int limit);









typedef struct community *ptr_community;


ptr_community Com_create(int id,ptr_graph graph);

void Com_destroy(ptr_community com);

int Com_match(const void* a,const void* key);

int Com_get_id(ptr_community com);

ptr_graph Com_get_graph(ptr_community com);


/************ klikes struct ****************/

typedef struct klika *ptr_klika;

ptr_klika KL_create(int CliqueSize);

void KL_destroy(ptr_klika klika);

void KL_insert(ptr_klika klika,int id);

int KL_match(const void* a,const void* b);

int KL_isFull(ptr_klika klika);

void KL_print(ptr_klika klika);

int KL_match_1(ptr_klika A,ptr_klika B);

void KL_print_graph(ptr_graph graph);

ptr_edge KL_copy_edge(ptr_edge data);

ptr_entry KL_copy_entry_person_knows_person(ptr_graph graph,ptr_entry data);

void KL_print_list_of_edges(ptr_entry node);

ptr_entry Com_test_if_exist(list_ptr communities,int id);

list_ptr KL_numberOfCCs(ptr_graph g);

/************ klika struct finish ***********/


list_ptr computeCPMResults(ptr_graph graph,int cliqueSize);

ptr_klika find_clique(ptr_graph graph,ptr_entry node,int* bhmata,int start,list_ptr klikes,int max_bhmata);

