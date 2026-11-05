#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define HT_INITIAL_BASE_SIZE 53

typedef struct{
    char *key;
    char *value;
} ht_item;

typedef struct{
    int size;
    int base_size;
    int count;
    ht_item **items;
} ht_hash_table;

void ht_insert( ht_hash_table *ht, const char* key, const char* value );
char *ht_search( ht_hash_table *ht, const char* key );
void ht_delete( ht_hash_table *h, const char*key );
void ht_del_hash_table( ht_hash_table *ht );