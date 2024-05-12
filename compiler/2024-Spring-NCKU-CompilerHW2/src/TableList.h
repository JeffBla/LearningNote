#ifndef TABLE_LIST_H
#define TABLE_LIST_H

#include <stdlib.h>

#include "ObjectList.h"
#include "compiler_common.h"

/**
 * Talbe List keep each scope's object list
 * Object List keep each object in the scope
 */

typedef struct objectList ObjectList;

typedef struct TableListNode {
    struct TableListNode* next;
    ObjectList* object_list;
} TableListNode;

typedef struct TableLinkedList {
    TableListNode* head;
    size_t size;  // the scope level equals to size
} TableList;

TableListNode* FindCurrTableNode(TableList* table_list);

/// Act as stack
void PushTable(TableList* table_list);
void PopTalble(TableList* table_list);

#endif  // TABLE_LIST_H