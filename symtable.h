/* Interface file for symbol table data type*/
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stddef.h>

   /* Node struct for linked list*/
  typedef struct Node *Node;
  /* SymTable_T struct for symbol table*/
  typedef struct SymTable_T *SymTable_T;

  /* Returns new SymTable_T object without bindings or NULL if 
  insufficient memory left */
  SymTable_T SymTable_new(void);

  /* Frees all memory occupied by oSymTable */
  void SymTable_free(SymTable_T oSymTable);

  /* Returns number of bindings in oSymTable */
  size_t SymTable_getLength(SymTable_T oSymTable);

  /* Adds new binding to oSymTable with key pcKey and value pvValue
  if key pcKey doesn't have a binding and returns true. Otherwise
  leaves oSymTable unchanged and returns 0. If insufficient memory,
  leaves oSymTable unchanged and returns 0.*/
  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

   /* If oSymTable has a binding with key pcKey, then replaces binding's
   value with pvValue and returns void pointer to old value. Otherwise
   leaves oSymTable unchanged and returns NULL. */
  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

   /* Returns 1 if oSymTable contains a binding whose key is pcKey, or
   NULL if no such binding exists.*/
  int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

  /* Returns value of binding within oSymTable whose key is pcKey,
  or NULL if no successful binding exists. */
  void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

  /* If oSymTable has a binding with key pcKey, then removes that binding from
  oSymTable and returns the binding's value. If doesn't contain, the function doesn't 
  change oSymTable and returns NULL. */
  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

  /* Apply the function pfApply to all bindings in oSymTable. Takes pvExtra as an extra
  parameter */
  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);


#endif