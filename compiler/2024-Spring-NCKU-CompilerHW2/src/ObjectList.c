#include "ObjectList.h"

void PushObject(Object* object, TableList* table_list) {
    // Find the current table node & object list
    TableListNode* curr_table_node = FindCurrTableNode(table_list);
    ObjectList* object_list = curr_table_node->object_list;

    // Add the object to the object list
    ObjectNode* new_object_node = (ObjectNode*)malloc(sizeof(ObjectNode));
    new_object_node->object = object;
    new_object_node->next = NULL;
    new_object_node->prev = NULL;
    if (object_list->head == NULL) {
        object_list->head = new_object_node;
    } else {
        new_object_node->next = object_list->head;
        object_list->head->prev = new_object_node;
        object_list->head = new_object_node;
    }
}