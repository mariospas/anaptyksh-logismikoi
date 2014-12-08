#ifndef _KATANOMH_
#define _KATANOMH_

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "entries.h"
#include "hash_table.h"


typedef struct katanomh *ptr_katanomh;

int match_friend(void *a,void *key);

ptr_katanomh create_data(int arithmos_filon);

void manage_list(list_ptr list,int arithmos_filon);

void degreeDistribution(ptr_graph g);













#endif /* _KATANOMH_ */
