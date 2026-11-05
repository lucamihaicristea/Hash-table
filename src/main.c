#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"

int main()
{
    ht_hash_table* ht = ht_new();
    assert(ht != NULL);

    printf("Testare inserare \n");
    ht_insert(ht, "name", "Luca");
    ht_insert(ht, "university", "Automatica");
    
    assert(strcmp(ht_search(ht, "name"), "Luca") == 0);
    assert(strcmp(ht_search(ht, "university"), "Automatica") == 0);
    assert(ht_search(ht, "nokey") == NULL);

    printf("Testare ștergere \n");
    ht_delete(ht, "name");
    assert(ht_search(ht, "name") == NULL);
    ht_print(ht);
    
    // Suprascriem o cheie stearsa
    ht_insert(ht, "name", "Mihai");
    assert(strcmp(ht_search(ht, "name"), "Mihai") == 0);
    ht_print(ht);
    printf("\n");
    printf( "Stress Test \n");
    for (int i = 0; i < 200; i++) {
        char key[20], val[20];
        sprintf(key, "k_%d", i);
        sprintf(val, "v_%d", i);
        ht_insert(ht, key, val);
    }
    ht_print(ht);

    
    // Verificam daca un element introdus anterior mai exista dupa redimensionare
    assert(strcmp(ht_search(ht, "k_150"), "v_150") == 0);


    ht_del_hash_table(ht);

    return 0;
}