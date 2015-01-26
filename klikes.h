#pragma once

#include "queries.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "database.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"
#include "prejob.h"


typedef struct community *ptr_community;


ptr_community Com_create(int id,ptr_graph graph);

void Com_destroy(ptr_community com);

int Com_match(const void* a,const void* key);

int Com_get_id_and_graph(ptr_community com,ptr_graph graph);


/************ klikes struct ****************/

typedef struct klika *ptr_klika;

ptr_klika KL_create(int CliqueSize);

void KL_destroy(ptr_klika klika);

void KL_insert(ptr_klika klika,int id);

int KL_match(const void* a,const void* b);

int KL_isFull(ptr_klika klika);

void KL_print(ptr_klika klika);

/************ klika struct finish ***********/


list_ptr computeCPMResults(ptr_graph graph,int cliqueSize);
