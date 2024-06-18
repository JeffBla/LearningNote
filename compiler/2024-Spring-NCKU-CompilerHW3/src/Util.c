#include "Util.h"

int scopeLevel = 0;  // scope level equ to the size of the table list
int funcArgStartAddr = 0;

const char* objectTypeName[] = {
    [OBJECT_TYPE_UNDEFINED] = "undefined",
    [OBJECT_TYPE_VOID] = "void",
    [OBJECT_TYPE_INT] = "int",
    [OBJECT_TYPE_FLOAT] = "float",
    [OBJECT_TYPE_BOOL] = "bool",
    [OBJECT_TYPE_STR] = "string",
    [OBJECT_TYPE_FUNCTION] = "function",
    [OBJECT_TYPE_CHAR] = "char",
};

const char* objectJavaTypeName[] = {
    [OBJECT_TYPE_UNDEFINED] = "V",
    [OBJECT_TYPE_VOID] = "V",
    [OBJECT_TYPE_BOOL] = "Z",
    [OBJECT_TYPE_CHAR] = "C",
    [OBJECT_TYPE_INT] = "I",
    [OBJECT_TYPE_LONG] = "J",
    [OBJECT_TYPE_FLOAT] = "F",
    [OBJECT_TYPE_DOUBLE] = "D",
    [OBJECT_TYPE_STR] = "Ljava/lang/String;",
};

void pushLabelNum(int labelNum, LabelNumList* labelNumList) {
    if (labelNumList->head == NULL) {
        LabelNumNode* new_node = (LabelNumNode*)malloc(sizeof(LabelNumNode));
        new_node->labelNum = labelNum;
        new_node->next = NULL;
        labelNumList->head = new_node;
    } else {
        LabelNumNode* new_node = (LabelNumNode*)malloc(sizeof(LabelNumNode));
        new_node->labelNum = labelNum;
        new_node->next = labelNumList->head;
        labelNumList->head = new_node;
    }
}

void popLabelNum(LabelNumList* labelNumList) {
    if (labelNumList->head == NULL) {
        return;
    }
    LabelNumNode* temp = labelNumList->head;
    labelNumList->head = labelNumList->head->next;
    free(temp);
}

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

Object* findVariable_func(char* variableName, TableList* table_list) {
    if (variableName == NULL) {
        return NULL;
    }

    // Search the variable in the symbol table include the current one & the parents
    TableListNode* curr_table_node = FindCurrTableNode(table_list);
    ObjectNode* obj_node;
    while (curr_table_node != NULL) {
        obj_node = curr_table_node->object_list->head;
        while (obj_node != NULL) {
            if ((obj_node->object->type == OBJECT_TYPE_FUNCTION) && (strcmp(obj_node->object->symbol->name, variableName) == 0)) {
                return obj_node->object;
            }
            obj_node = obj_node->next;
        }

        curr_table_node = curr_table_node->next;
    }

    fprintf(stderr, "Error: function `%s` not found\n", variableName);
    return NULL;
}

ObjectType funcReturnType(Object* func) {
    int len = strlen(func->symbol->func_sig);
    switch (func->symbol->func_sig[len - 1]) {
        case 'I':
            return OBJECT_TYPE_INT;
        case 'F':
            return OBJECT_TYPE_FLOAT;
        case 'Z':
            return OBJECT_TYPE_BOOL;
        case ';':
            return OBJECT_TYPE_STR;
        case 'V':
            return OBJECT_TYPE_VOID;
        default:
            return OBJECT_TYPE_UNDEFINED;
    }
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