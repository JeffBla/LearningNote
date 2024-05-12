#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

#include "TableList.h"
#include "compiler_common.h"

/**
 * Object List keep each object in the scope
 */

typedef struct TableLinkedList TableList;

typedef struct ObjectNode {
    struct ObjectNode* prev;
    struct ObjectNode* next;
    Object* object;
} ObjectNode;

typedef struct objectList {
    ObjectNode* head;
} ObjectList;

/// Act as stack
void PushObject(Object* object, TableList* table_list);

#endif  // OBJECT_LIST_H