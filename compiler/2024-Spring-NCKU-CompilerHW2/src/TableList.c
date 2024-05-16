#include "TableList.h"

#include <stdio.h>

TableListNode* FindCurrTableNode(TableList* table_list) {
    return table_list->head;
}

void PushTable(TableList* table_list) {
    TableListNode* new_table_node = (TableListNode*)malloc(sizeof(TableListNode));
    new_table_node->object_list = (ObjectList*)malloc(sizeof(ObjectList));
    new_table_node->object_list->head = NULL;
    new_table_node->next = NULL;
    if (table_list->head == NULL) {
        table_list->head = new_table_node;
    } else {
        new_table_node->next = table_list->head;
        table_list->head = new_table_node;
    }
    table_list->size++;
}

void PopTalble(TableList* table_list) {
    TableListNode* curr_table_node = FindCurrTableNode(table_list);
    ObjectNode* curr_object_node = curr_table_node->object_list->head;
    while (curr_object_node != NULL) {
        ObjectNode* temp = curr_object_node;
        curr_object_node = curr_object_node->next;
        FreeObject(temp->object);
        free(temp);
    }
    free(curr_table_node->object_list);
    table_list->head = curr_table_node->next;
    free(curr_table_node);
    table_list->size--;
}