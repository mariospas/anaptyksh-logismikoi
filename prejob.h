#pragma once

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
#include "queries.h"
#include "database.h"
#include "graph.h"
#include "graph_stats.h"
#include "graph_entry.h"

#define perror2(s, e) fprintf(stderr, "%s: %s\n",s,strerror(e))

typedef struct findTopN *ptr_findTopN;


pthread_mutex_t mutex_find_top_n = PTHREAD_MUTEX_INITIALIZER;


ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N);

void* array_thread_insert(void* topNdata);
