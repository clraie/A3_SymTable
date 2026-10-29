#include "symtable.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct Node {
    const char *key;
    void *value;
    struct Node *next;
};

struct SymTable_T {
    size_t length;
    size_t bucketCount;
    struct Node **buckets;
};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

SymTable_T SymTable_new (void) {
    
    SymTable_T st = malloc(sizeof(*st));
    size_t i;

    if (st == NULL) {
        return NULL;
    }

    st->length = 0;
    st->bucketCount = 509;
    st->buckets = malloc(st->bucketCount * sizeof(struct Node *));
    if (st->buckets == NULL) {
        free(st);
        return NULL;
    }
    for (i = 0; i < st->bucketCount; i++) {
        st->buckets[i] = NULL;
    }

    return st;
}

void SymTable_free(SymTable_T oSymTable) {
    struct Node *temp;
    size_t i;
    for (i = 0; i < oSymTable->bucketCount; i++) {
        while (oSymTable->buckets[i] != NULL) {
            temp = oSymTable->buckets[i]->next;
            free((char *)oSymTable->buckets[i]->key);
            free(oSymTable->buckets[i]);
            oSymTable->buckets[i] = temp;
        }
    }
    free(oSymTable->buckets);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    return oSymTable->length;
}

int SymTable_contains (SymTable_T oSymTable, const char *pcKey) {
    size_t hash = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct Node *p = oSymTable->buckets[hash];

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            return 1;
        }
        p = p->next;
    }

    return 0;
}

int SymTable_put (SymTable_T oSymTable,
        const char *pcKey, const void *pvValue) {
    
    struct Node *p;
    char *pcKeyCpy;
    size_t uLength = SymTable_getLength(oSymTable);
    size_t newBucketCount = oSymTable->bucketCount;
    struct Node **newBuckets;
    size_t i;
    size_t hash;
    struct Node *temp;
    struct Node *newNode;

    
    if (SymTable_contains(oSymTable, pcKey)) {
        return 0;
    }


    switch(uLength) {
        case 509:
            newBucketCount = 1021;
            break;
        case 1021:
            newBucketCount = 2039;
            break;
        case 2039:
            newBucketCount = 4093;
            break;
        case 4093:
            newBucketCount = 8191;
            break;
        case 8191:
            newBucketCount = 16381;
            break;
        case 16381:
            newBucketCount = 65521;
            break;
    }

    p = malloc(sizeof(*p));
    if (p == NULL)
        return 0;

    pcKeyCpy = malloc(strlen(pcKey) + 1);
    if (pcKeyCpy == NULL) {
        free(p);
        return 0;
    }
    strcpy(pcKeyCpy, pcKey);

    if (newBucketCount != oSymTable->bucketCount) {
        newBuckets = calloc(newBucketCount, sizeof(*newBuckets));
        if (newBuckets == NULL) {
            free(pcKeyCpy);
            free(p);
            return 0;
        }

        for (i = 0; i < oSymTable->bucketCount; i++) {
            p = oSymTable->buckets[i];

             while (p != NULL) {
                temp = p->next;

                hash = SymTable_hash(p->key, newBucketCount);
                p->next = newBuckets[hash];
                newBuckets[hash] = p;

                p = temp;
            }       
        }
        free(oSymTable->buckets);
        oSymTable->buckets = newBuckets;
        oSymTable->bucketCount = newBucketCount;
    }

    hash = SymTable_hash(pcKey, oSymTable->bucketCount);
    newNode = oSymTable->buckets[hash];
    newNode->key = pcKeyCpy;
    newNode->value = (void *)pvValue;
    newNode->next = oSymTable->buckets[hash];
    oSymTable->buckets[hash] = newNode;
    oSymTable->length++;

    return 1;

}

void *SymTable_replace (SymTable_T oSymTable,
        const char *pcKey, const void *pvValue) {
    
    struct Node *p;
    void *old;
    int hash = SymTable_hash(pcKey, oSymTable->bucketCount);
    
    p = oSymTable->buckets[hash];
       
    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            old = p->value;
            p->value = (void *)pvValue;
            return old;
        } else {
            p = p->next;
        }
    }
    
    return NULL;
}

void *SymTable_get (SymTable_T oSymTable, const char *pcKey) {
    
    struct Node *p;
    int hash = SymTable_hash(pcKey, oSymTable->bucketCount);

    p = oSymTable->buckets[hash];

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            return (void *)p->value;
        } else {
            p = p->next;
        }
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct Node *p;
    struct Node *previous = NULL;
    void *value;

    int hash = SymTable_hash(pcKey, oSymTable->bucketCount);

    p = oSymTable->buckets[hash];

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            value = (void *)p->value;
            if (previous == NULL) {
                oSymTable->buckets[hash] = p->next;
            }
            else {
                previous->next = p->next;
            }
            free((char *)p->key);
            free(p);
            oSymTable->length--;
            return value;
        } else {
            previous = p;
            p = p->next;
        }
    }

    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
        void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
        const void *pvExtra) {
    
    struct Node *p;
    size_t i;

    for (i = 0; i < oSymTable->bucketCount; i++) 
    {   
        p = oSymTable->buckets[i];
        while (p != NULL) {
            (*pfApply)(p->key, (void *)p->value, (void *)pvExtra);
            p = p->next;    
        }
    }
}

