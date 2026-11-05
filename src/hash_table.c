#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash_table.h"
#include "prime.h"

static void *xmalloc( size_t size )
{
    void *ptr = malloc(size);
    if( !ptr && size > 0 )
    {
        fprintf( stderr, "Error: insufficient memory");
        exit( EXIT_FAILURE );
    }
    return ptr;
}

static void *xcalloc( size_t num, size_t size )
{
    void *ptr = calloc(num, size);
    if( !ptr && (num * size) > 0 )
    {
        fprintf( stderr, "Error: insufficient memory");
        exit( EXIT_FAILURE );
    }
    return ptr;
}

static ht_item *ht_new_item( const char *k, const char *v)
{
    ht_item *i = malloc( sizeof( ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

static ht_hash_table *ht_new_sized( const int base_size )
{
    ht_hash_table *ht = xmalloc( sizeof( ht_hash_table ));
    ht->base_size = base_size;

    ht->size = next_prime( ht->base_size );

    ht->count = 0;
    ht->items = xcalloc( (size_t)ht->size, sizeof( ht_item*));
    return ht;
}

ht_hash_table *ht_new()
{
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

void ht_del_hash_table(ht_hash_table *ht)
{
    for( int i = 0; i < ht->size; i++ )
    {
        ht_item *item = ht->items[ i ];
        if( item != NULL && item != &HT_DELETED_ITEM )
            ht_del_item( item );
    }
    free( ht->items );
    free( ht );
}

static void ht_resize( ht_hash_table *ht, const int base_size )
{
    if( base_size < HT_INITIAL_BASE_SIZE )
        return;
    ht_hash_table *new_ht = ht_new_sized(base_size );
    for( int i = 0; i < ht->size; i++ )
    {
        ht_item *item = ht->items[ i ];
        if( item != NULL && item != &HT_DELETED_ITEM )
            ht_insert(new_ht, item->key, item->value );
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up( ht_hash_table *ht )
{
    const int new_size = ht->base_size *2;
    ht_resize( ht, new_size );
}

static void ht_resize_down( ht_hash_table *ht )
{
    const int new_size = ht->base_size / 2;
    ht_resize( ht, new_size );
}

static void ht_del_item( ht_item *i )
{
    free(i->key);
    free(i->value);
    free(i);
}


static int ht_hash( const char *s, const int a, const int m)
{
    long hash = 0;
    const int len_s = strlen( s );
    for( int i = 0; i < len_s; i++ )
        hash = (hash * a + (unsigned char )s[ i ] ) % m;
    return (int)hash;
}
/// string ul este tratat ca pe un numar scris in baza si calculeaza valoarea numerica modulo dimensiunea tabelei

static int ht_get_hash( const char *s, const int num_buckets, const int attempt )
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets );
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets );
    return ( hash_a + ( attempt * ( hash_b + 1))) % num_buckets;
}
///folosim double hash pt eficienta si evitarea coliziunilor


static ht_item HT_DELETED_ITEM = { NULL, NULL };
void ht_insert( ht_hash_table *ht, const char* key, const char *value )
{
    const int load = ht->count * 100 / ht->size;
    if( load > 70 )
        ht_resize_up(ht);

    ht_item* item = ht_new_item( key, value );
    int index = ht_get_hash( item->key, ht->size, 0 );
    ht_item *curr_item = ht->items[ index ];
    int i = 1;
    int ftombstone = -1;
    while( curr_item != NULL )
    {
        if( curr_item == &HT_DELETED_ITEM )
        {
            if( ftombstone == -1 )
                ftombstone = index;
        }
        else if( strcmp(curr_item->key, key ) == 0 )
            {
                ht_del_item(curr_item);
                ht->items[ index ] = item;
                return;
            }
        
        index = ht_get_hash( item->key, ht->size, i );
        curr_item = ht->items[ index ];
        i++;
    }


    if( ftombstone != -1 )
        ht->items[ ftombstone ] = item;
    else
        ht->items[ index ] = item;

    ht->count++;
}

char *ht_search(ht_hash_table *ht, const char* key )
{
    int index = ht_get_hash( key, ht->size, 0 );
    ht_item *item = ht->items[ index ];
    int i = 1;
    while( item != NULL )
    {
        if( item != &HT_DELETED_ITEM)
        {
            if(strcmp( item->key, key ) == 0 )
                return item->value;
            }
        index = ht_get_hash( key, ht->size, i );
        item = ht->items[ index ];
        i++;
    }
    return NULL;
}


void ht_delete( ht_hash_table *ht, const char* key )
{
    const int load = ht->count * 100 / ht->size;
    if( load < 10 )
        ht_resize_down(ht);

    int index = ht_get_hash( key, ht->size, 0 );
    ht_item *item = ht->items[ index ];
    int i = 1;
    while( item != NULL )
    {
        if( item != &HT_DELETED_ITEM )
        {
            if( strcmp(item->key, key ) == 0 )
            {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
                ht->count--;
                return;
            }
        }
        index = ht_get_hash( key, ht->size, i );
        item = ht->items[ index ];
        i++;
    }
}