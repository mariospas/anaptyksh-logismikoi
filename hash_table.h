#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#define INIT_SIZE 4       //table size
#define BUCKET_SIZE 3

struct bucket;
struct hash_table;

typedef struct hash_table *ptr_hash;
typedef struct bucket *ptr_bucket;

typedef int (*hash_f)(void* key);


ptr_hash HT_create( void );

void rec_bucket_destroy(ptr_bucket this);

void HT_destroy(ptr_hash this);








#endif /* _HASH_TABLE_ */
