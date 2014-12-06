#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "entries.h"
#include "hash_table.h"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

//int hashing( int key, int size );

void printPersonProperties(ptr_entry n);


int main( int argc, char *argv[] )
{
   /* int i, max = atoi( argv[1] );
    ht_ptr sample = HT_create( 4, 4 );

    for ( i = 1; i <= max; ++i ) {
        HT_insert( sample, hashing, NULL, i );
    }

    HT_print( sample );
    HT_destroy( sample, NULL );

    return EXIT_SUCCESS; // I hope
	*/

	int m = 2;
	int c = 3;

	ptr_graph g;
	g = createGraph(0,m,c);

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
	insertNode(g, n1, hash);
	insertNode(g, n2, hash);
	insertNode(g, n3, hash);
	insertNode(g, n4, hash);
	insertNode(g, n5, hash);
	insertNode(g, n6, hash);
	insertNode(g, n7, hash);
	insertNode(g, n8, hash);
	insertNode(g, n10, hash);
	insertNode(g, n9, hash);
	insertNode(g, n11, hash);
	insertNode(g, n12, hash);

	printf("Set Edge Properties\n");
	/* Create edges and set properties */
	ptr_edge e1 = setEdgeProperties(6, "knows", 30);
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
	insertEdge(g, 1, e1, hash);
	insertEdge(g, 6, e2, hash);
	insertEdge(g, 1, e3, hash);
	insertEdge(g, 2, e4, hash);
	insertEdge(g, 1, e5, hash);
	insertEdge(g, 4, e6, hash);
	insertEdge(g, 2, e7, hash);
	insertEdge(g, 6, e8, hash);
	insertEdge(g, 4, e9, hash);
	insertEdge(g, 3, e10, hash);
	insertEdge(g, 4, e11, hash);
	insertEdge(g, 7, e12, hash);
	insertEdge(g, 4, e13, hash);
	insertEdge(g, 8, e14, hash);
	insertEdge(g, 3, e15, hash);
	insertEdge(g, 10, e16, hash);
	insertEdge(g, 10, e17, hash);
	insertEdge(g, 7, e18, hash);
	insertEdge(g, 10, e19, hash);
	insertEdge(g, 14, e20, hash);
	insertEdge(g, 14, e21, hash);
	insertEdge(g, 12, e22, hash);
	insertEdge(g, 12, e23, hash);
	insertEdge(g, 16, e24, hash);
	insertEdge(g, 16, e25, hash);
	insertEdge(g, 14, e26, hash);

	printf("Try to LookUp\n");
	ptr_entry Nl1 = lookupNode(g,1,hash);
	printPersonProperties(Nl1);


}








/*
int hashing( int key,int size )
{
    return (key % size);
}
*/




ptr_entry setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
    Properties prop = createProperties(PERSON_PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    setStringProperty(surname, 1, prop);
    setIntegerProperty(age, 2, prop);

    /*create ptr_entry*/
    ptr_entry n = create_entry(id,((void*) prop),match_edge);


    return n;
}

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) {

    /*create edge properties*/
    Properties propEdge = createProperties(PERSON_REL_PROPERTIES_NUM);
    setStringProperty(type, 0, propEdge);
    setIntegerProperty(weight, 1, propEdge);

    /*create an edge */
    ptr_edge e = create_edge(endNodeID,((void*) propEdge));

    return e;
}


void printPersonProperties(ptr_entry n)
{
	Properties p = (Properties) (n->properties);
	printf("Name : %s \n",p->name);
}
