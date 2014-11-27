#include <stdlib.h>
#include "graph.h"
#include "entries.h"
#include "hash_table.h"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

ptr_entry setPersonProperties(int id, char* name, char* surname, int age);

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) ;

int hashing( const void *key );

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


	/*insert nodes in graph*/
	insertNode(n1, g);
	insertNode(n2, g);
	insertNode(n3, g);
	insertNode(n4, g);
	insertNode(n5, g);
	insertNode(n6, g);
	insertNode(n7, g);
	insertNode(n8, g);
	insertNode(n10, g);
	insertNode(n9, g);
	insertNode(n11, g);
	insertNode(n12, g);

	/* Create edges and set properties */
	ptr_edge e1 = setEdgeProperties(1, 6, "knows", 30);
	ptr_edge e2 = setEdgeProperties(6, 1, "knows", 30);
	ptr_edge e3 = setEdgeProperties(1, 2, "knows", 20);
	ptr_edge e4 = setEdgeProperties(2, 1, "knows", 20);
	ptr_edge e5 = setEdgeProperties(1, 4, "knows", 30);
	ptr_edge e6 = setEdgeProperties(4, 1, "knows", 30);
	ptr_edge e7 = setEdgeProperties(2, 6, "knows", 10);
	ptr_edge e8 = setEdgeProperties(6, 2, "knows", 10);
	ptr_edge e9 = setEdgeProperties(4, 3, "knows", 30);
	ptr_edge e10 = setEdgeProperties(3, 4, "knows", 30);
	ptr_edge e11 = setEdgeProperties(4, 7, "knows", 30);
	ptr_edge e12 = setEdgeProperties(7, 4, "knows", 30);
	ptr_edge e13 = setEdgeProperties(4, 8, "knows", 10);
	ptr_edge e14 = setEdgeProperties(8, 4, "knows", 10);
	ptr_edge e15 = setEdgeProperties(3, 10, "knows", 30);
	ptr_edge e16 = setEdgeProperties(10, 3, "knows", 30);
	ptr_edge e17 = setEdgeProperties(10, 7, "knows", 30);
	ptr_edge e18 = setEdgeProperties(7, 10, "knows", 30);
	ptr_edge e19 = setEdgeProperties(10, 14, "knows", 50);
	ptr_edge e20 = setEdgeProperties(14, 10, "knows", 50);
	ptr_edge e21 = setEdgeProperties(14, 12, "knows", 30);
	ptr_edge e22 = setEdgeProperties(12, 14, "knows", 30);
	ptr_edge e23 = setEdgeProperties(12, 16, "knows", 30);
	ptr_edge e24 = setEdgeProperties(16, 12, "knows", 30);
	ptr_edge e25 = setEdgeProperties(16, 14, "knows", 30);
	ptr_edge e26 = setEdgeProperties(14, 16, "knows", 30);
	ptr_edge e27 = setEdgeProperties(3, 8, "knows", 30);
	ptr_edge e28 = setEdgeProperties(8, 3, "knows", 30);
	ptr_edge e29 = setEdgeProperties(8, 10, "knows", 10);
	ptr_edge e30 = setEdgeProperties(10, 8, "knows", 10);

}

int hashing( const void *key )
{
    return *( (int*) key );
}





ptr_entry setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
    Properties* prop = createProperties(PERSON_PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    setStringProperty(surname, 1, prop);
    setIntegerProperty(age, 2, prop);

    /*create ptr_entry*/
    ptr_entry n = create_entry(id,(void) prop,match_edge);


    return n;
}

ptr_edge setEdgeProperties(int endNodeID, char* type, int weight) {

    /*create edge properties*/
    Properties* propEdge = createProperties(PERSON_REL_PROPERTIES_NUM);
    setStringProperty(type, 0, propEdge);
    setIntegerProperty(weight, 1, propEdge);

    /*create an edge */
    ptr_edge e = create_edge(endNodeID,(void) propEdge);

    return e;
}
