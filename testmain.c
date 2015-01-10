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
if ( (actual - expected < 0.01) || (expected - actual < 0.01)) {                                   \
  printf("%-30s: Failed | actual = %3f, expected = %3f\n",  \
         text,                                              \
         actual,                                            \
         expected);                                         \
} else {                                                    \
  printf("%-30s: Success\n", text);                     \
}


#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

size_t hashing( int key, size_t size );

void printPersonProperties(ptr_entry n);

void testBetweennessCentrality(int bucketsNumber, int bucketSize);
void testClosenessCentrality(int bucketsNumber, int bucketSize);


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

	//testClosenessCentrality(2,3);

#endif
	///load and test///

	ptr_database database = DB_create();

	g = DB_get_entity(database,PERSON);
	ptr_graph post_graph = DB_get_entity(database,POST);
	ptr_graph forum_graph = DB_get_entity(database,FORUM);
	printf("get_graph\n");

	load_graph(g);
	printf("\n\n\n^^^^^^^^^^^^^^ POST_graph load ^^^^^^^^^^^^^^^\n\n\n");
	load_graph(post_graph);
	printf("\n\n\n^^^^^^^^^^^^^^ POST_graph finish load ^^^^^^^^^^^^^^^\n\n\n");
	printf("\n\n\n^^^^^^^^^^^^^^ FORUM_graph load ^^^^^^^^^^^^^^^\n\n\n");
	load_graph(forum_graph);
	printf("\n\n\n^^^^^^^^^^^^^^ FORUM_graph finish load ^^^^^^^^^^^^^^^\n\n\n");

	ptr_entry N60 = lookupNode(g,6906);  //9800
	printPersonProperties(N60);

	ptr_entry N70 = lookupNode(g,1631);  //9800
	printPersonProperties(N70);

	print_list_of_edges(N60);

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

	//prepei na beltiothei gt einai argh
	//int reach8 = reachNode1(g,6906,4814);
	//printf("reach8 = %d\n\n",reach8);

	ptr_entry N80 = lookupNode(g,3755);

	ptr_array_matches array = matchSuggestion(N80,1,400,40,5,g);
	printf("finish matchSugg\n");
	int id_match = get_match(0,array);
	printf("\nMatch id = %d\n",id_match);
	delete_array_matches(array);


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
    CHECKDOUBLE("Small Graph betweenness centrality node:1 ", betwCentrty1, 0.0 / 6);

    double betwCentrty2 = betweenness_centrality(n2Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:2 ", betwCentrty2, 3.5 / 6);

    double betwCentrty3 = betweenness_centrality(n3Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:3 ", betwCentrty3, 1.0 / 6);

    double betwCentrty4 = betweenness_centrality(n4Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:4 ", betwCentrty4, 1.0 / 6);

    double betwCentrty5 = betweenness_centrality(n5Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:5 ", betwCentrty5, 0.5 / 6);
}


void testClosenessCentrality(int bucketsNumber, int bucketSize) {
    //create small graph for testing betweenness Centrality
	ptr_graph gClos = createGraph(PERSON,bucketsNumber, bucketSize);


    ptr_entry n1Clos = create_entry(1,NULL,person_delete);
	ptr_entry n2Clos = create_entry(2, NULL,person_delete);
	ptr_entry n3Clos = create_entry(3, NULL,person_delete);
	ptr_entry n4Clos = create_entry(4, NULL,person_delete);
	ptr_entry n5Clos = create_entry(5, NULL,person_delete);
	ptr_entry n6Clos = create_entry(6, NULL,person_delete);
	ptr_entry n7Clos = create_entry(7, NULL,person_delete);

    insertNode(gClos, n1Clos);
    insertNode( gClos, n2Clos);
    insertNode( gClos, n3Clos);
    insertNode( gClos, n4Clos);
    insertNode( gClos, n5Clos);
    insertNode( gClos, n6Clos);
    insertNode( gClos, n7Clos);

    char edge_type[EDGE_TYPE_BUF] = "person_knows_person.csv";
	int target_type = PERSON;


    /* Create edges and set properties */
    ptr_edge e1Clos = create_edge(edge_type,2,target_type,10,NULL);
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




    /* Insert edges in graph */
    insertEdge(gClos,1, e1Clos);
    insertEdge(gClos,2, e2Clos);
    insertEdge(gClos,2, e3Clos);
    insertEdge(gClos,2, e4Clos);
    insertEdge(gClos,3, e5Clos);
    insertEdge(gClos,3, e6Clos);
    insertEdge(gClos,4, e7Clos);
    insertEdge(gClos,4, e8Clos);
    insertEdge(gClos,5, e9Clos);
    insertEdge(gClos,5, e10Clos);

    double closCentrty1 = closeness_centrality(n1Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:1 ", closCentrty1, 0.4 / 6);

    double closCentrty2 = closeness_centrality(n2Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:2 ", closCentrty2, 0.4 / 6);

    double closCentrty3 = closeness_centrality(n3Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:3 ", closCentrty3, 0.55 / 6);

    double closCentrty4 = closeness_centrality(n4Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:4 ", closCentrty4, 0.6 / 6);

    double closCentrty5 = closeness_centrality(n5Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:5 ", closCentrty5, 0.55 / 6);

    double closCentrty6 = closeness_centrality(n6Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:6 ", closCentrty6, 0.4 / 6);

    double closCentrty7 = closeness_centrality(n7Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:7 ", closCentrty7, 0.4 / 6);

}

