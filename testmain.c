#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "graph.h"
#include "graph_entry.h"
#include "graph_stats.h"
#include "dataset_handlers.h"
#include "hash_table.h"
#include "database.h"
#include "linked_list.h"
#include "queries.h"
#include "prejob.h"

#define TRUST_GRAPH_REL_PROPERTIES_NUM 1

#define CHECKINT(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }

#define CHECKDOUBLE(text, actual, expected)                         \
if ( ABS(actual,expected) > 0.01 ) {                                   \
  printf("%-30s: Failed | actual = %3f, expected = %3f\n",  \
         text,                                              \
         actual,                                            \
         expected);                                         \
} else {                                                    \
  printf("%-30s: Success\n", text);                     \
}

#define ABS(value1, value2) (value1 - value2 < 0 ) ? value2 - value1 : value1 - value2



#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

size_t hashing( int key, size_t size );

void printPersonProperties(ptr_entry n);

void testBetweennessCentrality(int bucketsNumber, int bucketSize);
void testClosenessCentrality(int bucketsNumber, int bucketSize);
ptr_edge setEdgeTrustProperties(int startNodeID, int endNodeID, double trust);
void testTidalTrust(int bucketsNumber, int bucketSize);

/*
char *cont[20] = {
    "No0",
    "No1",
    "No2",
    "No3",
    "No4",
    "No5",
    "No6",
    "No7",
    "No8",
    "No9",
    "No10",
    "No11",
    "No12",
    "No13",
    "No14",
    "No15",
    "No16",
    "No17",
    "No18",
    "No19"
};
*/
int main( int argc, char *argv[] )
{
	/*
    int i, max = 20;
    ht_ptr sample = HT_create( 2, 2, hashing );

    for ( i = 0; i < max; ++i ) {
        HT_insert( sample, cont[i], i );
    }

    HT_print( sample );

    fputc( '\n', stdout );
    printf( "%d: %s\n", 3, (char*) HT_search( sample, 3 ) );
    printf( "%d: %s\n", 8, (char*) HT_search( sample, 8 ) );
    printf( "%d: %s\n", 14, (char*) HT_search( sample, 14 ) );

    HT_destroy( sample, NULL );
    return EXIT_SUCCESS; // I hope
    */


	int m = 2;
	int c = 3;

	ptr_graph g;
	g = createGraph(PERSON,m,c);

#if 0
	printf("Set Person Properties\n");
	/*create node and set node properties*/
	ptr_entry n1 = setPersonProperties(5, "lonely", "loner", 29);
	ptr_entry n2 = setPersonProperties(1, "herald", "kllapi", 22);
	ptr_entry n3 = setPersonProperties(2, "marialena", "kiriakidi", 25);
	ptr_entry n4 = setPersonProperties(10, "antonia", "saravanou", 18);
	ptr_entry n5 = setPersonProperties(6, "manos", "karvounis", 19);
	ptr_entry n6 = setPersonProperties(3, "giannis", "chronis", 20);
	ptr_entry n7 = setPersonProperties(4, "christoforos", "sfiggos", 16);
	ptr_entry n8 = setPersonProperties(7, "stamatis", "xristoforidis", 24);
	ptr_entry n9 = setPersonProperties(8, "xristos", "mallios", 29);
	ptr_entry n10 = setPersonProperties(14, "johnny", "depp", 35);
	ptr_entry n11 = setPersonProperties(12, "fox", "mulder", 29);
	ptr_entry n12 = setPersonProperties(16, "dana", "scully", 25);



	printf("Insert Person Properties\n");
	/*insert nodes in graph*/
	insertNode(g, n1);
	insertNode(g, n2);
	insertNode(g, n3);
	insertNode(g, n4);
	insertNode(g, n5);
	insertNode(g, n6);
	insertNode(g, n7);
	insertNode(g, n8);
	insertNode(g, n10);
	insertNode(g, n9);
	insertNode(g, n11);
	insertNode(g, n12);

	printf("Try to LookUp\n");
	ptr_entry Nl2 = lookupNode(g,1);
	if(Nl2 == NULL) printf("NULL\n");
	else printPersonProperties(Nl2);

	printf("Set Edge Properties\n");
	/* Create edges and set properties */
	ptr_edge e1 = setEdgeProperties(6, "knows", 30);   //knows na bazame mia lista
	ptr_edge e2 = setEdgeProperties(1, "knows", 30);
	ptr_edge e3 = setEdgeProperties(2, "knows", 20);
	ptr_edge e4 = setEdgeProperties(1, "knows", 20);
	ptr_edge e5 = setEdgeProperties(4, "knows", 30);
	ptr_edge e6 = setEdgeProperties(1, "knows", 30);
	ptr_edge e7 = setEdgeProperties(6, "knows", 10);
	ptr_edge e8 = setEdgeProperties(2, "knows", 10);
	ptr_edge e9 = setEdgeProperties(3, "knows", 30);
	ptr_edge e10 = setEdgeProperties(4, "knows", 30);
	ptr_edge e11 = setEdgeProperties(7, "knows", 30);
	ptr_edge e12 = setEdgeProperties(4, "knows", 30);
	ptr_edge e13 = setEdgeProperties(8, "knows", 10);
	ptr_edge e14 = setEdgeProperties(4, "knows", 10);
	ptr_edge e15 = setEdgeProperties(10, "knows", 30);
	ptr_edge e16 = setEdgeProperties(3, "knows", 30);
	ptr_edge e17 = setEdgeProperties(7, "knows", 30);
	ptr_edge e18 = setEdgeProperties(10, "knows", 30);
	ptr_edge e19 = setEdgeProperties(14, "knows", 50);
	ptr_edge e20 = setEdgeProperties(10, "knows", 50);
	ptr_edge e21 = setEdgeProperties(12, "knows", 30);
	ptr_edge e22 = setEdgeProperties(14, "knows", 30);
	ptr_edge e23 = setEdgeProperties(16, "knows", 30);
	ptr_edge e24 = setEdgeProperties(12, "knows", 30);
	ptr_edge e25 = setEdgeProperties(14, "knows", 30);
	ptr_edge e26 = setEdgeProperties(16, "knows", 30);
	ptr_edge e27 = setEdgeProperties(8, "knows", 30);
	ptr_edge e28 = setEdgeProperties(3, "knows", 30);
	ptr_edge e29 = setEdgeProperties(10, "knows", 10);
	ptr_edge e30 = setEdgeProperties(8, "knows", 10);

	///ebala tis parakato gia na sundeso perissotera na ginei pio grhgoro
	ptr_edge e31 = setEdgeProperties(5, "knows", 30);
	ptr_edge e32 = setEdgeProperties(5, "knows", 10);
	ptr_edge e33 = setEdgeProperties(5, "knows", 10);



	printf("Insert Edge Properties\n");
	insertEdge(g, 1, e1);
	insertEdge(g, 6, e2);
	insertEdge(g, 1, e3);
	insertEdge(g, 2, e4);
	insertEdge(g, 1, e5);
	insertEdge(g, 4, e6);
	insertEdge(g, 2, e7);
	insertEdge(g, 6, e8);
	insertEdge(g, 4, e9);
	insertEdge(g, 3, e10);
	insertEdge(g, 4, e11);
	insertEdge(g, 7, e12);
	insertEdge(g, 4, e13);
	insertEdge(g, 8, e14);
	insertEdge(g, 3, e15);
	insertEdge(g, 10, e16);
	insertEdge(g, 10, e17);
	insertEdge(g, 7, e18);
	insertEdge(g, 10, e19);
	insertEdge(g, 14, e20);
	insertEdge(g, 14, e21);
	insertEdge(g, 12, e22);
	insertEdge(g, 12, e23);
	insertEdge(g, 16, e24);
	insertEdge(g, 16, e25);
	insertEdge(g, 14, e26);

	///ebala tis parakato gia na sundeso perissotera na ginei pio grhgoro
	insertEdge(g, 4, e31);
	insertEdge(g, 7, e32);
	insertEdge(g, 14, e33);
	insertEdge(g, 5, e20);
	insertEdge(g, 5, e17);


	printf("Try to LookUp\n");
	ptr_entry Nl1 = lookupNode(g,1);
	printPersonProperties(Nl1);

	int reach1 = reachNode1(g,1,12);
	printf("reach1 = %d\n\n",reach1);

	int reach2 = reachNode1(g,14,14);
	printf("reach2 = %d\n\n",reach2);

	int reach3 = reachNode1(g,3,16);
	printf("reach3 = %d\n\n",reach3);

	int reach4 = reachNode1(g,5,3);
	printf("reach4 = %d\n\n",reach4);



	//printf("Try to LookUp\n");
	ptr_entry N1 = lookupNode(g,1);
	printPersonProperties(N1);

	//printf("Try to LookUp\n");
	ptr_entry N2 = lookupNode(g,2);
	printPersonProperties(N2);

	//printf("Try to LookUp\n");
	ptr_entry N3 = lookupNode(g,3);
	printPersonProperties(N3);

	//printf("Try to LookUp\n");
	ptr_entry N4 = lookupNode(g,4);
	printPersonProperties(N4);

	//printf("Try to LookUp\n");
	ptr_entry N5 = lookupNode(g,5);
	printPersonProperties(N5);

	//printf("Try to LookUp\n");
	ptr_entry N6 = lookupNode(g,6);
	printPersonProperties(N6);

	//printf("Try to LookUp\n");
	ptr_entry N7 = lookupNode(g,7);
	printPersonProperties(N7);

	//printf("Try to LookUp\n");
	ptr_entry N8 = lookupNode(g,8);
	printPersonProperties(N8);

	//printf("Try to LookUp\n");
	ptr_entry N10 = lookupNode(g,10);
	printPersonProperties(N10);

	//printf("Try to LookUp\n");
	ptr_entry N12 = lookupNode(g,12);
	printPersonProperties(N12);

	//printf("Try to LookUp\n");
	ptr_entry N14 = lookupNode(g,14);
	printPersonProperties(N14);

	//printf("Try to LookUp\n");
	ptr_entry N16 = lookupNode(g,16);
	printPersonProperties(N16);

/*
	int id = -1,distance = -1;
	Result_ptr result = reachNodeN(g,1);

	while( ResultSet_next(result,&id,&distance))
	{
		printf("Start id = %d to end id = %d and distance = %d\n",result->from,id,distance);
	}
	printf("Start id = %d to end id = %d and distance = %d\n",result->from,id,distance);
*/



	///////////part2////////////////

	//degreeDistribution(g);

	/*
	int diam = diameter(g);
	printf("\nDIAMETER = %d\n",diam);

	int aver = 0;
	double apotel = 0.0;
	aver = averagePathLength(g,&apotel);
	printf("\nAVERAGE PATH = %f\n",apotel);

	int numofCC = numberOfCCs(g);
	printf("\nNumber of Connected Comp = %d\n",numofCC);

	int max = maxCC(g);
	printf("\nMax Connected Comp = %d\n",max);

	double denc = 1.0;
	int ok = density(g,&denc);
	printf("\nDensity = %f\n",denc);
	*/

	//testBetweennessCentrality(2,3);

	testClosenessCentrality(2,3);

#endif
	///load and test///
//#if 0
	ptr_database database = DB_create();

	g = DB_get_entity(database,PERSON);
	ptr_graph post_graph = DB_get_entity(database,POST);
	ptr_graph forum_graph = DB_get_entity(database,FORUM);
	ptr_graph tag_forum = DB_get_entity(database,TAG);
	ptr_graph comment_graph = DB_get_entity(database,COMMENT);
	printf("get_graph\n");

	load_graph(g);
	//printf("\n\n\n^^^^^^^^^^^^^^ POST_graph load ^^^^^^^^^^^^^^^\n\n\n");
	//load_graph(post_graph);
	//printf("\n\n\n^^^^^^^^^^^^^^ POST_graph finish load ^^^^^^^^^^^^^^^\n\n\n");
	printf("\n\n\n^^^^^^^^^^^^^^ FORUM_graph load ^^^^^^^^^^^^^^^\n\n\n");
	load_graph(forum_graph);
	printf("\n\n\n^^^^^^^^^^^^^^ FORUM_graph finish load ^^^^^^^^^^^^^^^\n\n\n");
	//printf("\n\n\n^^^^^^^^^^^^^^ tag_forum load ^^^^^^^^^^^^^^^\n\n\n");
	//load_graph(tag_forum);
	//printf("\n\n\n^^^^^^^^^^^^^^ tag_forum finish load ^^^^^^^^^^^^^^^\n\n\n");
	//printf("\n\n\n^^^^^^^^^^^^^^ comment_graph load ^^^^^^^^^^^^^^^\n\n\n");
	//load_graph(comment_graph);
	//printf("\n\n\n^^^^^^^^^^^^^^ comment_graph finish load ^^^^^^^^^^^^^^^\n\n\n");

#if 0
	//prepei na beltiothei gt einai argh
	int reach8 = reachNode1(g,6906,4814);
	printf("reach = %d start id = %d and end id = %d\n\n",reach8,6906,4814);


	reach8 = reachNode1(g,32,1631);
	printf("reach = %d start id = %d and end id = %d\n\n",reach8,32,1631);

	int diam = diameter(g);
	printf("\nDIAMETER = %d\n",diam);

	int aver = 0;
	double apotel = 0.0;
	aver = averagePathLength(g,&apotel);
	printf("\nAVERAGE PATH = %f\n",apotel);

	int numofCC = numberOfCCs(g);
	printf("\nNumber of Connected Comp = %d\n",numofCC);

	int max = maxCC(g);
	printf("\nMax Connected Comp = %d\n",max);

	double denc = 1.0;
	int ok = density(g,&denc);
	printf("\nDensity = %f\n",denc);



	ptr_entry N60 = lookupNode(g,6906);  //9800
	printPersonProperties(N60);

	ptr_entry N70 = lookupNode(g,1631);  //9800
	printPersonProperties(N70);

	//print_list_of_edges(N60);

	int loc = location_of_entry(N60);
	printf("\nLocation id = %d\n",loc);

	int last_work = last_work_or_study_of_entry(N60,"person_workAt_organisation.csv");
	printf("\nLast Work id = %d\n",last_work);

	int last_study = last_work_or_study_of_entry(N60,"person_studyAt_organisation.csv");
	printf("\nLast Study id = %d\n",last_study);

	int int1,int2;
	int com_int = common_interests_two_entries(N60,N70,&int1,&int2);
	printf("\nCommon Interest k = %d\n",com_int);

	int gap = generation_gap(N60,N70);
	printf("\nGap Years = %d\n",gap);

	int same_sex = same_gender(N60,N70);
	printf("\nSame Gender ? : ");
	if(same_sex) printf("Yes !!!\n");
	else printf("No !!!\n");



	ptr_entry N80 = lookupNode(g,3755);
	closeness_centrality(N80,g);

	betweenness_centrality(N80,g);

	/************ Erothma 1 *************/

	ptr_array_matches array = matchSuggestion(N80,1,400,40,5,g);
	printf("finish matchSugg\n");

	double simil = 0.0;
	int id_match = get_match(0,array,&simil);
	printf("\nMatch id = %d\n",id_match);
	delete_array_matches(array);

	printf("\n");

	/************ TELOS Erothma 1 *************/

	/************ Erothma 2 *************/

	int stalkersNum = 7, likesNumber = 1, centralityMode = 1;
	ptr_array_matches array_stalker;
	ptr_graph stalker_graph = getTopStalkers(stalkersNum,likesNumber,centralityMode,database,array_stalker);

	double score = 0.0;
	int w;
	for(w=0;w<(stalkersNum);w++)
	{
		int id_stalker = get_match(w,array,&score);
		printf("\nStalker id = %d and Score = %f\n",id_stalker,score);
	}

	int reach9 = reachNode1(stalker_graph,495,347);
	printf("reach9 = %d \n",reach9);

	//print_graph(stalker_graph);

	//delete_array_matches(array_stalker); // problem :(

	//degreeDistribution(stalker_graph);

	/************ TELOS Erothma 2 *************/

/#endif

	/************ Erothma 3 *************/

	int trendsNum = 4;
	    //allocate result tables before calling query and pass them as parameters
	char** womenTrends;
	womenTrends = (char**) malloc(trendsNum * sizeof (char*));
	char** menTrends;
	menTrends = (char**) malloc(trendsNum * sizeof (char*));
	findTrends(trendsNum, database, womenTrends, menTrends);

	int i,j;
	int len;
	for(i=0;i<trendsNum;i++)
	{
		printf("WomenTrend[%d] = %s",i,womenTrends[i]);
		len = strlen(womenTrends[i]);
		if(len < 20)
		{
			for(j=0;j<20-len;j++)
			{
				printf(" ");
			}
		}
		printf("MenTrend[%d] = %s\n",i,menTrends[i]);
	}
//#endif
	/************ TELOS Erothma 3 *************/

	/************ Erothma 4 *************/

	ptr_entry no = lookupNode(post_graph,4);
	if(no == NULL) printf("NULL\n");
	else printf("NOT NULL");

	printf("\n\nTry 4 querry\n\n");

	int forumId = 34680;
	ptr_graph trust_graph = buildTrustGraph(forumId,database);

	print_graph(trust_graph);

	int trustANodeId = 30;
	int trustBNodeId = 9805;
	int trustCNodeId = 9700;
	ptr_entry ta = lookupNode(trust_graph, trustANodeId);
	ptr_entry tb = lookupNode(trust_graph, trustBNodeId);
	ptr_entry tc = lookupNode(trust_graph, trustCNodeId);
	double trustAB = 0.0;
	trustAB = estimateTrust(ta, tb, trust_graph);
	printf("Trust between nodes (%d,%d) is %f\n\n", trustANodeId, trustBNodeId, trustAB);

	double trustAC;
	trustAC = estimateTrust(ta, tc, trust_graph);
	printf("Trust between nodes (%d,%d) is %f\n", trustANodeId, trustCNodeId, trustAC);

	testTidalTrust(4,4);

	/************ TELOS Erothma 4 *************/
#endif


	//print_graph(g);
	/**************** Part 3 ******************/

	int limit = 8;
	double score;
	ptr_array_matches array = find_topN_forums(forum_graph,limit);
	int g1;
	int forum_id;
	for(g1=0;g1<(limit);g1++)
	{
		forum_id = get_match(g1,array,&score);
		printf("\nForum id = %d and Members = %f\n",forum_id,score);
	}

	ptr_forum_database forums_database;
	forums_database = computeTopNForums(database,limit);

	printf("*********** forum_database ready **********\n");

	ptr_graph f = DB_forum_get_entity(forums_database,34680);  //34680 , 228560 , 228280
	if(f == NULL) printf("NULL graph\n");
	//printf("start print\n");
	print_graph(f);

	/********** klikes ready ***************/
	list_ptr communities;
	int com_size,w;
	ptr_community com;
	int id_com=80;
	ptr_graph com_graph;
	LL_iter_ptr iter;

	communities = computeCPMResults(f,4);
	if(communities != NULL)
	{
		com_size = LL_size(communities);
		iter = LL_iter_create(communities);

		for(w=0;w<com_size;w++)
		{
			com = (ptr_community) LL_iter_data(iter);
			id_com = Com_get_id(com);
			com_graph = Com_get_graph(com);
			printf("\nCOM ID=%d\n",id_com);
			KL_print_graph(com_graph);
			printf("\n");
			LL_iter_next(iter);
		}
		LL_iter_destroy(iter);
	}
	else printf("There aren't communities\n");
}


ptr_entry setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
	gender_t gender = MALE;
	ptr_date creation_date = date_create(1,1,1,1,1,1);
	char location_ip[30] = "192.168.1.1";
	char browser[20] = "chrome";

    ptr_person_info prop = person_create(id,name,surname,gender,creation_date,creation_date,location_ip,browser);

    /*create ptr_entry*/
    ptr_entry n = create_entry(id,((void*) prop),person_delete);


    return n;
}

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) {

    /*create edge properties*/
    char edge_type[EDGE_TYPE_BUF] = "person_knows_person.csv";
    int target_id = endNodeID;
    int target_type = PERSON;

    /*create an edge */
    ptr_edge e = create_edge(edge_type,target_id,target_type,weight,NULL);

    return e;
}


void printPersonProperties(ptr_entry n)
{
	//printf("entry_id = %d\n",n->id);
	ptr_person_info p = (ptr_person_info) (n->properties);
	printf("Name : %s and ID = %d\n",p->first_name,p->id);
}


size_t hashing( int key, size_t size )
{
    return 100 / (key+1) + 30;
}


void testBetweennessCentrality(int bucketsNumber, int bucketSize) {
    //create small graph for testing betweenness Centrality

    ptr_graph gBetw = createGraph(PERSON,bucketsNumber, bucketSize);

    ptr_entry n1Betw = create_entry(1,NULL,person_delete);
    ptr_entry n2Betw = create_entry(2, NULL,person_delete);
    ptr_entry n3Betw = create_entry(3, NULL,person_delete);
    ptr_entry n4Betw = create_entry(4, NULL,person_delete);
    ptr_entry n5Betw = create_entry(5, NULL,person_delete);

    insertNode(gBetw,n1Betw);
    insertNode(gBetw,n2Betw);
    insertNode(gBetw,n3Betw);
    insertNode(gBetw,n4Betw);
    insertNode(gBetw,n5Betw);


    char edge_type[EDGE_TYPE_BUF] = "person_knows_person.csv";
	int target_type = PERSON;

	/*create an edge */


    /* Create edges and set properties */
	ptr_edge e1Betw = create_edge(edge_type,2,target_type,10,NULL);
	ptr_edge e2Betw = create_edge(edge_type, 1, target_type,10,NULL);
    ptr_edge e3Betw = create_edge(edge_type, 3, target_type,10,NULL);
    ptr_edge e4Betw = create_edge(edge_type, 4, target_type,10,NULL);
    ptr_edge e5Betw = create_edge(edge_type, 2, target_type,10,NULL);
    ptr_edge e6Betw = create_edge(edge_type, 5, target_type,10,NULL);
    ptr_edge e7Betw = create_edge(edge_type, 2, target_type,10,NULL);
    ptr_edge e8Betw = create_edge(edge_type, 5, target_type,10,NULL);
    ptr_edge e9Betw = create_edge(edge_type, 3, target_type,10,NULL);
    ptr_edge e10Betw = create_edge(edge_type, 4, target_type,10,NULL);

    /* Insert edges in graph */
    insertEdge(gBetw, 1, e1Betw);
    insertEdge(gBetw, 2, e2Betw);
    insertEdge(gBetw, 2, e3Betw);
    insertEdge(gBetw, 2, e4Betw);
    insertEdge(gBetw, 3, e5Betw);
    insertEdge(gBetw, 3, e6Betw);
    insertEdge(gBetw, 4, e7Betw);
    insertEdge(gBetw, 4, e8Betw);
    insertEdge(gBetw, 5, e9Betw);
    insertEdge(gBetw, 5, e10Betw);

    double betwCentrty1 = betweenness_centrality(n1Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:1 ", betwCentrty1, 0.0 / 6.0);

    double betwCentrty2 = betweenness_centrality(n2Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:2 ", betwCentrty2, 3.5 / 6.0);

    double betwCentrty3 = betweenness_centrality(n3Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:3 ", betwCentrty3, 1.0 / 6.0);

    double betwCentrty4 = betweenness_centrality(n4Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:4 ", betwCentrty4, 1.0 / 6.0);

    double betwCentrty5 = betweenness_centrality(n5Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:5 ", betwCentrty5, 0.5 / 6.0);
}


void testClosenessCentrality(int bucketsNumber, int bucketSize) {
    //create small graph for testing betweenness Centrality
	ptr_graph gClos = createGraph(PERSON,bucketsNumber, bucketSize);

	/*
    ptr_entry n1Clos = create_entry(1,13,person_delete);
	ptr_entry n2Clos = create_entry(2, 13,person_delete);
	ptr_entry n3Clos = create_entry(3, 13,person_delete);
	ptr_entry n4Clos = create_entry(4, 13,person_delete);
	ptr_entry n5Clos = create_entry(5, 13,person_delete);
	ptr_entry n6Clos = create_entry(6, 13,person_delete);
	ptr_entry n7Clos = create_entry(7, 13,person_delete);
	*/

	ptr_entry n1Clos = setPersonProperties(1, "lonely", "loner", 29);
	ptr_entry n2Clos = setPersonProperties(2, "herald", "kllapi", 22);
	ptr_entry n3Clos = setPersonProperties(3, "marialena", "kiriakidi", 25);
	ptr_entry n4Clos = setPersonProperties(4, "antonia", "saravanou", 18);
	ptr_entry n5Clos = setPersonProperties(5, "manos", "karvounis", 19);
	ptr_entry n6Clos = setPersonProperties(6, "giannis", "chronis", 20);
	ptr_entry n7Clos = setPersonProperties(7, "christoforos", "sfiggos", 16);


    insertNode(gClos, n1Clos);
    insertNode( gClos, n2Clos);
    insertNode( gClos, n3Clos);
    insertNode( gClos, n4Clos);
    insertNode( gClos, n5Clos);
    insertNode( gClos, n6Clos);
    insertNode( gClos, n7Clos);

    char edge_type[EDGE_TYPE_BUF] = "person_knows_person.csv";
    printf("edge_type = %s\n",edge_type);
	int target_type = PERSON;


    /* Create edges and set properties */
   /* ptr_edge e1Clos = create_edge(edge_type,2,target_type,10,NULL);
	ptr_edge e2Clos = create_edge(edge_type, 3, target_type,10,NULL);
	ptr_edge e3Clos = create_edge(edge_type, 1, target_type,10,NULL);
	ptr_edge e4Clos = create_edge(edge_type, 3, target_type,10,NULL);
	ptr_edge e5Clos = create_edge(edge_type, 1, target_type,10,NULL);
	ptr_edge e6Clos = create_edge(edge_type, 2, target_type,10,NULL);
	ptr_edge e7Clos = create_edge(edge_type, 4, target_type,10,NULL);
	ptr_edge e8Clos = create_edge(edge_type, 3, target_type,10,NULL);
	ptr_edge e9Clos = create_edge(edge_type, 5, target_type,10,NULL);
	ptr_edge e10Clos = create_edge(edge_type, 4, target_type,10,NULL);

	ptr_edge e11Clos = create_edge(edge_type,6,target_type,10,NULL);
	ptr_edge e12Clos = create_edge(edge_type, 7, target_type,10,NULL);
	ptr_edge e13Clos = create_edge(edge_type, 5, target_type,10,NULL);
	ptr_edge e14Clos = create_edge(edge_type, 7, target_type,10,NULL);
	ptr_edge e15Clos = create_edge(edge_type, 5, target_type,10,NULL);
	ptr_edge e16Clos = create_edge(edge_type, 6, target_type,10,NULL);
	*/

	ptr_edge e1Clos = setEdgeProperties(2, "knows", 30);   //knows na bazame mia lista
	ptr_edge e2Clos = setEdgeProperties(3, "knows", 30);
	ptr_edge e3Clos = setEdgeProperties(1, "knows", 20);
	ptr_edge e4Clos = setEdgeProperties(3, "knows", 20);
	ptr_edge e5Clos = setEdgeProperties(1, "knows", 30);
	ptr_edge e6Clos = setEdgeProperties(2, "knows", 30);
	ptr_edge e7Clos = setEdgeProperties(4, "knows", 10);
	ptr_edge e8Clos = setEdgeProperties(3, "knows", 10);
	ptr_edge e9Clos = setEdgeProperties(5, "knows", 30);
	ptr_edge e10Clos = setEdgeProperties(4, "knows", 30);
	ptr_edge e11Clos = setEdgeProperties(6, "knows", 30);
	ptr_edge e12Clos = setEdgeProperties(7, "knows", 30);
	ptr_edge e13Clos = setEdgeProperties(5, "knows", 10);
	ptr_edge e14Clos = setEdgeProperties(7, "knows", 10);
	ptr_edge e15Clos = setEdgeProperties(5, "knows", 30);
	ptr_edge e16Clos = setEdgeProperties(6, "knows", 30);



    /* Insert edges in graph */
    insertEdge(gClos,1, e1Clos);
    insertEdge(gClos,1, e2Clos);
    insertEdge(gClos,2, e3Clos);
    insertEdge(gClos,2, e4Clos);
    insertEdge(gClos,3, e5Clos);
    insertEdge(gClos,3, e6Clos);
    insertEdge(gClos,3, e7Clos);
    insertEdge(gClos,4, e8Clos);
    insertEdge(gClos,4, e9Clos);
    insertEdge(gClos,5, e10Clos);
    insertEdge(gClos,5, e11Clos);
    insertEdge(gClos,5, e12Clos);
    insertEdge(gClos,6, e13Clos);
    insertEdge(gClos,6, e14Clos);
    insertEdge(gClos,7, e15Clos);
    insertEdge(gClos,7, e16Clos);

    printf("Try to LookUp\n");
	ptr_entry Nl1 = lookupNode(gClos,3);
	//printPersonProperties(Nl1);
	print_list_of_edges(Nl1);

    int reach_dok = reachNode1(gClos,4,3);
    printf("reach_dok = %d\n\n",reach_dok);

    print_graph(gClos);

    double closCentrty1 = closeness_centrality(n1Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:1 ", closCentrty1, 3.33 / 6.0);

    double closCentrty2 = closeness_centrality(n2Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:2 ", closCentrty2, 3.33 / 6.0);

    double closCentrty3 = closeness_centrality(n3Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:3 ", closCentrty3, 4.16 / 6.0);

    double closCentrty4 = closeness_centrality(n4Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:4 ", closCentrty4, 4.0 / 6.0);

    double closCentrty5 = closeness_centrality(n5Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:5 ", closCentrty5, 4.16 / 6.0);

    double closCentrty6 = closeness_centrality(n6Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:6 ", closCentrty6, 3.33 / 6.0);

    double closCentrty7 = closeness_centrality(n7Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:7 ", closCentrty7, 3.33 / 6.0);

}


void testTidalTrust(int bucketsNumber, int bucketSize) {
    //create small graph for testing tidal's trust algorithm result
    ptr_graph gtt = createGraph(PERSON,bucketsNumber, bucketSize);

    ptr_entry n1tt = create_entry(1,NULL,NULL);
    ptr_entry n2tt = create_entry(2, NULL,NULL);
    ptr_entry n3tt = create_entry(3, NULL,NULL);
    ptr_entry n4tt = create_entry(4, NULL,NULL);
    ptr_entry n5tt = create_entry(5, NULL,NULL);
    ptr_entry n6tt = create_entry(6, NULL,NULL);
    ptr_entry n7tt = create_entry(7, NULL,NULL);
    ptr_entry n8tt = create_entry(8, NULL,NULL);
    ptr_entry n9tt = create_entry(9, NULL,NULL);
    ptr_entry n10tt = create_entry(10, NULL,NULL);
    ptr_entry n11tt = create_entry(11, NULL,NULL);

    insertNode(gtt, n1tt);
    insertNode(gtt, n2tt);
    insertNode(gtt, n3tt);
    insertNode(gtt, n4tt);
    insertNode(gtt, n5tt);
    insertNode(gtt, n6tt);
    insertNode(gtt, n7tt);
    insertNode(gtt, n8tt);
    insertNode(gtt, n9tt);
    insertNode(gtt, n10tt);
    insertNode(gtt, n11tt);


    ptr_edge e1tt = setEdgeTrustProperties(1, 2, 1.0);
    ptr_edge e2tt = setEdgeTrustProperties(1, 5, 1.0);
    ptr_edge e3tt = setEdgeTrustProperties(2, 3, 0.9);
    ptr_edge e4tt = setEdgeTrustProperties(2, 4, 0.9);
    ptr_edge e5tt = setEdgeTrustProperties(3, 6, 0.8);
    ptr_edge e6tt = setEdgeTrustProperties(4, 6, 0.3);
    ptr_edge e7tt = setEdgeTrustProperties(4, 7, 0.9);
    ptr_edge e8tt = setEdgeTrustProperties(5, 10, 0.9);
    ptr_edge e9tt = setEdgeTrustProperties(6, 9, 1.0);
    ptr_edge e10tt = setEdgeTrustProperties(7, 8, 1.0);
    ptr_edge e11tt = setEdgeTrustProperties(8, 9, 1.0);
    ptr_edge e12tt = setEdgeTrustProperties(9, 11, 1.0);
    ptr_edge e13tt = setEdgeTrustProperties(10, 11, 0.4);

    /* Insert edges in graph */
    insertEdge(gtt, 1, e1tt);
    insertEdge(gtt, 1, e2tt);
    insertEdge(gtt, 2, e3tt);
    insertEdge(gtt, 2, e4tt);
    insertEdge(gtt, 3, e5tt);
    insertEdge(gtt, 4, e6tt);
    insertEdge(gtt, 4, e7tt);
    insertEdge(gtt, 5, e8tt);
    insertEdge(gtt, 6, e9tt);
    insertEdge(gtt, 7, e10tt);
    insertEdge(gtt, 8, e11tt);
    insertEdge(gtt, 9, e12tt);
    insertEdge(gtt, 10, e13tt);


    ptr_entry att = lookupNode(gtt, 1);
    printf("att->id = %d\n",att->id);

    ptr_entry btt = lookupNode(gtt, 11);
    printf("btt->id = %d\n\n",btt->id);
    //Estimate trust(1,11)
    double trust1_11 = estimateTrust(att, btt, gtt);
    CHECKDOUBLE("Graph estimate trust (1,11)", trust1_11, 0.36);

    //Estimate trust(1,9)
    ptr_entry ctt = lookupNode(gtt, 9);
    printf("ctt->id = %d\n\n",ctt->id);
    double trust1_9 = estimateTrust(att, ctt, gtt);
    CHECKDOUBLE("Graph estimate trust (1,9)", trust1_9, 0.72);

}

ptr_edge setEdgeTrustProperties(int startNodeID, int endNodeID, double trust) {

    /*create an ptr_edge*/
    ptr_edge e = create_edge("person_knows_person.csv",endNodeID,PERSON,trust,NULL);

    return e;
}


