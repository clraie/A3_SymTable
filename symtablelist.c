#include "symtable.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* Creates struct Node with string key, void pointer to value, and
Node pointer to next*/
struct Node {
    const char *key;
    void *value;
    struct Node *next;
};

/* Creates SymTable_T struct, has length (number of bindings) and a Node
pointer to the first Node in the list. */
struct SymTable_T {
    size_t length;
    struct Node *first;
};

/* Creates and returns new SymTable_T without bindings. Sets number of bindings
to zero and the first Node pointer to NULL. */
SymTable_T SymTable_new (void) {
    
    SymTable_T st = malloc(sizeof(*st));

    if (st == NULL) {
        return NULL;
    }

    st->length = 0;
    st->first = NULL;

    return st;
}

/* Traverses the linked list and frees each key and Node. Frees oSymTable
at the end.*/
void SymTable_free(SymTable_T oSymTable) {
    struct Node *temp;

    assert(oSymTable != NULL);

    while(oSymTable->first != NULL) {
        temp = oSymTable->first->next;
        free((char *)oSymTable->first->key);
        free(oSymTable->first);
        oSymTable->first = temp;
    }
    free(oSymTable);
}

/* Returns the number of bindings.*/
size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->length;
}

/* Traverses the linked list with a temp pointer p and returns 1 (true)
if the string compare finds pcKey. Returns 0 elsewise. */
int SymTable_contains (SymTable_T oSymTable, const char *pcKey) {
    struct Node *p = oSymTable->first;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            return 1;
        }
        p = p->next;
    }

    return 0;
}

/* Checks if oSymTable contains pcKey and returns 0 if true. Else, creates
a defensive copy of the key and puts it in the linked list. Increment the number
of bindings.*/
int SymTable_put (SymTable_T oSymTable,
        const char *pcKey, const void *pvValue) {
    
    struct Node *p;
    char *pcKeyCpy;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);

    if (SymTable_contains(oSymTable, pcKey)) {
        return 0;
    }
    p = malloc(sizeof(*p));
    if (p == NULL) {
        return 0;
    }
    pcKeyCpy = malloc(strlen(pcKey) + 1);
    if (pcKeyCpy == NULL) {
        free(p);
        return 0;
    }

    strcpy(pcKeyCpy, pcKey);
    p->key = pcKeyCpy;
    p->value = (void *)pvValue;
    p->next = oSymTable->first;
    oSymTable->first = p;
    oSymTable->length++;

    return 1;

}

/* Traverses the linked list with a temporary pointer p and 
replaces the value at pcKey with pvValue if found, then returns the 
former value. If not pcKey is not found, return NULL. */
void *SymTable_replace (SymTable_T oSymTable,
        const char *pcKey, const void *pvValue) {
    
    struct Node *p = oSymTable->first;
    void *old;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);

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

/* Traverses through the linked list with temp pointer p and returns 
a void pointer to the value at pcKey if found. If not found, returns NULL.*/
void *SymTable_get (SymTable_T oSymTable, const char *pcKey) {
    
    struct Node *p = oSymTable->first;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    while (p != NULL) {
        if (strcmp(p->key, pcKey) == 0) {
            return (void *)p->value;
        } else {
            p = p->next;
        }
    }
    return NULL;
}

/* Traverses the linked list with a temp pointer p and frees the key at pcKey if found.
Decrements the number of bindings and relinks the list. Returns NULL if pcKey is not found.*/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct Node *p = oSymTable->first;
    struct Node *previous = NULL;
    void *value;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

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

/*Traverses the linked list with temp pointer p and applies the function pfApply
to each Node. */
void SymTable_map(SymTable_T oSymTable,
        void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
        const void *pvExtra) {
    
    struct Node *p = oSymTable->first;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    while (p != NULL) {
        (*pfApply)(p->key, (void *)p->value, (void *)pvExtra);
        p = p->next;    
    }
}



