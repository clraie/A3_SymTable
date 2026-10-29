#include "symtable.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct Node {
    const char *key;
    void *value;
    struct Node *next;
} Node;


struct SymTable_T {
    size_t length;
    struct Node *first;
};

SymTable_T SymTable_new (void) {
    
    SymTable_T st = malloc(sizeof(*st));

    if (st == NULL) {
        return NULL;
    }

    st->length = 0;
    st->first = NULL;

    return st;
}

void SymTable_free(SymTable_T oSymTable) {
    struct Node *temp;
    while(oSymTable->first != NULL) {
        temp = oSymTable->first->next;
        free((char *)oSymTable->first->key);
        free(oSymTable->first);
        oSymTable->first = temp;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    return oSymTable->length;
}

int SymTable_contains (SymTable_T oSymTable, const char *pcKey) {
    struct Node *p = oSymTable->first;
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

    if (SymTable_contains(oSymTable, pcKey))
        return 0;

    p = malloc(sizeof(*p));
    if (p == NULL)
        return 0;

    pcKeyCpy = malloc(strlen(pcKey) + 1);
    if (pcKeyCpy == NULL) {
        free(p);
        return 0;
    }

    strcpy(pcKeyCpy, pcKey);
    p->key = pcKeyCpy;
    p->value = pvValue;
    p->next = oSymTable->first;
    oSymTable->first = p;
    oSymTable->length++;

    return 1;

}

void *SymTable_replace (SymTable_T oSymTable,
        const char *pcKey, const void *pvValue) {
    
    struct Node *p = oSymTable->first;
    void *old;

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
    
    struct Node *p = oSymTable->first;

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
    struct Node *p = oSymTable->first;
    struct Node *previous = NULL;
    void *value;

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            value = (void *)p->value;
            if (previous == NULL) {
                oSymTable->first = oSymTable->first->next;
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
    
    struct Node *p = oSymTable->first;

    while (p != NULL) {
        (*pfApply)(p->key, (void *)p->value, (void *)pvExtra);
        p = p->next;    
    }
}


