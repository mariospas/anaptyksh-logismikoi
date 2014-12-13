#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "entries.h"
#include "hash_table.h"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

size_t hashing( int key, size_t size );

void printPersonProperties(ptr_entry n);

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

int main( int argc, char *argv[] )
{
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

#if 0
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

#endif
}

ptr_entry setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
    Properties prop = createProperties(PERSON_PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    setStringProperty(surname, 1, prop);
    setIntegerProperty(age, 2, prop);

    /*create ptr_entry*/
    ptr_entry n = create_entry(id,((void*) prop));


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

size_t hashing( int key, size_t size )
{
    return key * key * 2 + 20;
}
