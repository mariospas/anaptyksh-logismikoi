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
