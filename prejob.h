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



ptr_array_matches find_topN_forums(ptr_graph forum_graph,int N);
