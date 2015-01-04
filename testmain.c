#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "graph_entry.h"
#include "dataset_handlers.h"
#include "hash_table.h"
#include "database.h"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

size_t hashing( int key, size_t size );

void printPersonProperties(ptr_entry n);


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

	//part2

	degreeDistribution(g);

}


ptr_entry setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
	gender_t gender = MALE;
	ptr_date creation_date = date_create(1,1,1,1,1,1);
	char location_ip[30] = "192.168.1.1";
	char browser[20] = "chrome";

    ptr_person_info prop = person_create(id,name,surname,gender,creation_date,location_ip,browser);

    /*create ptr_entry*/
    ptr_entry n = create_entry(id,((void*) prop),person_delete);


    return n;
}

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) {

    /*create edge properties*/
    char edge_type[EDGE_TYPE_BUF] = "PersonKnowsPerson";
    int target_id = endNodeID;
    int target_type = PERSON;

    /*create an edge */
    ptr_edge e = create_edge(edge_type,target_id,target_type,weight);

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
