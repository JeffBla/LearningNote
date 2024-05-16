#include "Util.h"

const char* objectTypeName[] = {
    [OBJECT_TYPE_UNDEFINED] = "undefined",
    [OBJECT_TYPE_VOID] = "void",
    [OBJECT_TYPE_INT] = "int",
    [OBJECT_TYPE_FLOAT] = "float",
    [OBJECT_TYPE_BOOL] = "bool",
    [OBJECT_TYPE_STR] = "string",
    [OBJECT_TYPE_FUNCTION] = "function",
};

Object* findVariable(char* variableName, TableList* table_list) {
    if (variableName == NULL) {
        return NULL;
    }

    // Search the variable in the symbol table include the current one & the parents
    TableListNode* curr_table_node = FindCurrTableNode(table_list);
    ObjectNode* obj_node;
    while (curr_table_node != NULL) {
        obj_node = curr_table_node->object_list->head;
        while (obj_node != NULL) {
            if (strcmp(obj_node->object->symbol->name, variableName) == 0) {
                return obj_node->object;
            }
            obj_node = obj_node->next;
        }

        curr_table_node = curr_table_node->next;
    }

    fprintf(stderr, "Error: variable `%s` not found\n", variableName);
    return NULL;
}

char* strcat_copy(const char* str1, const char* str2) {
    int str1_len, str2_len;
    char* new_str;

    /* null check */

    str1_len = strlen(str1);
    str2_len = strlen(str2);

    new_str = malloc(str1_len + str2_len + 1);

    /* null check */

    memcpy(new_str, str1, str1_len);
    memcpy(new_str + str1_len, str2, str2_len + 1);

    return new_str;
}

void ExpTypeCheck(Object* a, Object* b, Object* out) {
    if (a->type == b->type) {
        out->type = a->type;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->type = OBJECT_TYPE_FLOAT;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->type = OBJECT_TYPE_FLOAT;
    } else if (a->type == OBJECT_TYPE_BOOL && b->type == OBJECT_TYPE_INT) {
        out->type = OBJECT_TYPE_INT;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_BOOL) {
        out->type = OBJECT_TYPE_INT;
    } else if (a->type == OBJECT_TYPE_BOOL && b->type == OBJECT_TYPE_FLOAT) {
        out->type = OBJECT_TYPE_FLOAT;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_BOOL) {
        out->type = OBJECT_TYPE_FLOAT;
    }
}

void ExpAssignTypeCheck(Object* dest, Object* val, Object* out) {
    out->type = dest->type;
}

uint64_t Float2Uint64(float f) {
    uint64_t i;
    memcpy(&i, &f, sizeof(i));
    return i;
}

float Uint64ToFloat(uint64_t i) {
    float f;
    memcpy(&f, &i, sizeof(f));
    return f;
}

uint64_t StringLiteral2Uint64(char* str) {
    uint64_t i;
    memcpy(&i, &str, sizeof str);
    return i;
}

char* Uint64ToString(uint64_t i) {
    char* str;
    memcpy(&str, &i, sizeof i);
    return str;
}