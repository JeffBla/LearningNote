#include "main.h"

#define debug printf("%s:%d: ############### debug\n", __FILE__, __LINE__)

#define toupper(_char) (_char - (char)32)

const char* objectTypeName[] = {
    [OBJECT_TYPE_UNDEFINED] = "undefined",
    [OBJECT_TYPE_VOID] = "void",
    [OBJECT_TYPE_INT] = "int",
    [OBJECT_TYPE_FLOAT] = "float",
    [OBJECT_TYPE_BOOL] = "bool",
    [OBJECT_TYPE_STR] = "string",
    [OBJECT_TYPE_FUNCTION] = "function",
};

char* yyInputFileName;
bool compileError;

int indent = 0;
int scopeLevel = -1;
int funcLineNo = 0;
int variableAddress = 0;
ObjectType variableIdentType;

TableList table_list;
FunctionParmList cout_parm_list;

TableListNode* FindCurrTableNode() {
    TableListNode* curr = table_list.head;
    for (int i = 0; i < scopeLevel; i++) {
        curr = curr->next;
    }
    return curr;
}

void AddObject(Object* object) {
    // Find the current table node & object list
    TableListNode* curr_table_node = FindCurrTableNode();
    ObjectList* object_list = curr_table_node->object_list;

    // Add the object to the object list
    ObjectNode* new_object_node = (ObjectNode*)malloc(sizeof(ObjectNode));
    new_object_node->object = object;
    new_object_node->next = NULL;
    if (object_list->head == NULL) {
        object_list->head = new_object_node;
    } else {
        ObjectNode* curr_object_node = object_list->head;
        while (curr_object_node->next != NULL) {
            curr_object_node = curr_object_node->next;
        }
        curr_object_node->next = new_object_node;
    }
}

void AddTable() {
    TableListNode* new_table_node = (TableListNode*)malloc(sizeof(TableListNode));
    new_table_node->object_list = (ObjectList*)malloc(sizeof(ObjectList));
    new_table_node->object_list->head = NULL;
    new_table_node->next = NULL;
    if (table_list.head == NULL) {
        table_list.head = new_table_node;
    } else {
        TableListNode* curr_table_node = table_list.head;
        while (curr_table_node->next != NULL) {
            curr_table_node = curr_table_node->next;
        }
        curr_table_node->next = new_table_node;
    }
}

void pushScope() {
    printf("> Create symbol table (scope level %d)\n", ++scopeLevel);
    variableAddress = 0;
    AddTable();
}

void dumpScope() {
    printf("\n");
    printf("> Dump symbol table (scope level %d)\n", scopeLevel);
    TableListNode* curr_table_node = FindCurrTableNode();
    scopeLevel--;
    ObjectNode* curr_object_node = curr_table_node->object_list->head;
    // Index     Name                Type      Addr      Lineno    Func_sig
    printf("%-8s %-20s %-8s %-8s %-8s %-8s\n", "Index", "Name", "Type", "Addr", "Lineno", "Func_sig");
    while (curr_object_node != NULL) {
        printf("%-8d %-20s %-8s %-8ld %-8d %-8s\n",
               curr_object_node->object->symbol->index,
               curr_object_node->object->symbol->name,
               objectTypeName[curr_object_node->object->type],
               curr_object_node->object->symbol->addr,
               curr_object_node->object->symbol->lineno,
               "-");
        curr_object_node = curr_object_node->next;
    }
}

Object* createVariable(ObjectType variableType, char* variableName, int variableFlag) {
    printf("> Insert `%s` (addr: %d) to scope level %d\n", variableName, variableAddress++, scopeLevel);
    // Populate the object
    Object* new_object = (Object*)malloc(sizeof(Object));
    new_object->type = variableType;
    new_object->value = 0;
    new_object->symbol = (SymbolData*)malloc(sizeof(SymbolData));
    new_object->symbol->name = variableName;
    new_object->symbol->index = variableAddress;
    new_object->symbol->addr = variableAddress;
    new_object->symbol->lineno = yylineno;
    variableAddress++;
    // Add the object to the object list
    AddObject(new_object);
    return new_object;
}

ObjectType PrintIdent(char* ident_name) {
    int addr = -1;
    ObjectType obj_type = OBJECT_TYPE_UNDEFINED;
    if (strcmp(ident_name, "endl") == 0) {
        obj_type = OBJECT_TYPE_STR;
    } else {
        Object* curr = findVariable(ident_name);
        if (curr != NULL) {
            obj_type = curr->type;
            addr = curr->symbol->addr;
        }
    }

    printf("IDENT (name=%s, address=%d)\n", ident_name, addr);
    return obj_type;
}

void pushFunParm(ObjectType variableType, char* variableName, int variableFlag) {
    createVariable(variableType, variableName, variableFlag);
}

void createFunction(ObjectType variableType, char* funcName) {
    printf("func: %s\n", funcName);

    createVariable(OBJECT_TYPE_FUNCTION, funcName, VAR_FLAG_DEFAULT);
}

void debugPrintInst(char instc, Object* a, Object* b, Object* out) {
}

bool objectExpression(const char* op, Object* dest, Object* val, Object* out) {
    bool isDone;
    switch (op[0]) {
        case '+':
            isDone = objectExpAdd(dest, val, out);
            break;
        case '-':
            isDone = objectExpSub(dest, val, out);
            break;
        case '*':
            isDone = objectExpMul(dest, val, out);
            break;
        case '/':
            isDone = objectExpDiv(dest, val, out);
            break;
        case '%':
            isDone = objectExpRem(dest, val, out);
            break;
        case '>':  // >>
            isDone = objectExpShr(dest, val, out);
            break;
        case '<':  // <<
            isDone = objectExpShl(dest, val, out);
            break;
        default:
            isDone = false;
            break;
    }
    return isDone;
}

bool objectExpAdd(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if ((a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) || (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT)) {
        out->type = a->type;
        out->value = a->value + b->value;
        isDone = true;
    }
    printf("ADD\n");
    return isDone;
}

bool objectExpSub(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if ((a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) || (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT)) {
        out->type = a->type;
        out->value = a->value - b->value;
        isDone = true;
    }
    printf("SUB\n");
    return isDone;
}

bool objectExpMul(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if ((a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) || (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT)) {
        out->type = a->type;
        out->value = a->value * b->value;
        isDone = true;
    }
    printf("MUL\n");
    return isDone;
}

bool objectExpDiv(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if ((a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) || (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT)) {
        out->type = a->type;
        out->value = a->value / b->value;
        isDone = true;
    }
    printf("DIV\n");
    return isDone;
}

bool objectExpRem(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) {
        out->type = a->type;
        out->value = a->value % b->value;
        isDone = true;
    }
    printf("REM\n");
    return isDone;
}

bool objectExpShr(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) {
        out->type = a->type;
        out->value = a->value >> b->value;
        isDone = true;
    }
    printf("SHR\n");
    return isDone;
}

bool objectExpShl(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_INT) {
        out->type = a->type;
        out->value = a->value << b->value;
        isDone = true;
    }
    printf("SHL\n");
    return isDone;
}

bool objectExpBinary(char op, Object* a, Object* b, Object* out) {
    return false;
}

bool objectExpBoolean(const char* op, Object* a, Object* b, Object* out) {
    bool isDone;
    if (strcmp(op, "!") == 0) {
        isDone = objectExpBoolNot(a, out);
    } else if (strcmp(op, "&&") == 0) {
        isDone = objectExpBoolAnd(a, b, out);
    } else if (strcmp(op, "||") == 0) {
        isDone = objectExpBoolOr(a, b, out);
    } else if (strcmp(op, ">") == 0) {
        isDone = objectExpGtr(a, b, out);
    } else if (strcmp(op, "<") == 0) {
        isDone = objectExpLes(a, b, out);
    } else if (strcmp(op, "==") == 0) {
        isDone = objectExpBoolEq(a, b, out);
    } else if (strcmp(op, "!=") == 0) {
        isDone = objectExpBoolNeq(a, b, out);
    } else if (strcmp(op, ">=") == 0) {
        isDone = objectExpGeq(a, b, out);
    } else if (strcmp(op, "<=") == 0) {
        isDone = objectExpLeq(a, b, out);
    } else {
        isDone = false;
    }
    return isDone;
}

bool objectExpBoolNot(Object* a, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = !a->value;
    printf("NOT\n");
    return true;
}

bool objectExpBoolAnd(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value && b->value;
    printf("LAN\n");
    return true;
}

bool objectExpBoolOr(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value || b->value;
    printf("LOR\n");
    return true;
}

bool objectExpBoolEq(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value == b->value;
    printf("EQL\n");
    return true;
}

bool objectExpBoolNeq(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value != b->value;
    printf("NEQ\n");
    return true;
}

bool objectExpGtr(Object* a, Object* b, Object* out) {
    bool isDone = false;
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value > b->value;
    printf("GTR\n");
    return true;
}

bool objectExpLes(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value < b->value;
    printf("LES\n");
    return true;
}

bool objectExpGeq(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value >= b->value;
    printf("GEQ\n");
    return true;
}

bool objectExpLeq(Object* a, Object* b, Object* out) {
    out->type = OBJECT_TYPE_BOOL;
    out->value = a->value <= b->value;
    printf("LEQ\n");
    return true;
}

bool objectExpAssign(char op, Object* dest, Object* val, Object* out) {
    return false;
}

bool objectValueAssign(Object* dest, Object* val, Object* out) {
    return false;
}

bool objectExpBinaryNot(Object* dest, Object* out) {
    bool isDone = false;
    if (dest->type == OBJECT_TYPE_INT || dest->type == OBJECT_TYPE_FLOAT) {
        out->type = dest->type;
        out->value = ~dest->value;
        isDone = true;
    }
    printf("BNT\n");
    return false;
}

bool objectNegExpression(Object* dest, Object* out) {
    bool isDone = false;
    if (dest->type == OBJECT_TYPE_INT || dest->type == OBJECT_TYPE_FLOAT) {
        out->type = dest->type;
        out->value = -dest->value;
        isDone = true;
    }
    printf("NEG\n");
    return isDone;
}

bool objectNotExpression(Object* dest, Object* out) {
    bool isDone = false;
    if (dest->type == OBJECT_TYPE_BOOL) {
        out->type = dest->type;
        out->value = !dest->value;
        isDone = true;
    }
    printf("NOT\n");
    return isDone;
}

bool objectIncAssign(Object* a, Object* out) {
    return false;
}

bool objectDecAssign(Object* a, Object* out) {
    return false;
}

bool objectCast(ObjectType variableType, Object* dest, Object* out) {
    return false;
}

Object* findVariable(char* variableName) {
    if (variableName == NULL) {
        return NULL;
    }

    TableListNode* curr_table_node = FindCurrTableNode();
    ObjectNode* obj_node = curr_table_node->object_list->head;
    while (obj_node != NULL) {
        if (strcmp(obj_node->object->symbol->name, variableName) == 0) {
            return obj_node->object;
        }
        obj_node = obj_node->next;
    }
    return obj_node->object;
}

void pushFunInParm(Object* variable) {
    FunctionParmTypeNode* new_node = (FunctionParmTypeNode*)malloc(sizeof(FunctionParmTypeNode));
    new_node->type = variable->type;
    new_node->next = NULL;
    if (cout_parm_list.head == NULL) {
        cout_parm_list.head = new_node;
    } else {
        FunctionParmTypeNode* curr = cout_parm_list.head;
        while (curr->next != NULL) {
            curr = curr->next;
        }

        curr->next = new_node;
    }
}

void stdoutPrint() {
    printf("cout");
    FunctionParmTypeNode* curr = cout_parm_list.head;
    while (curr != NULL) {
        printf(" %s", objectTypeName[curr->type]);
        curr = curr->next;
    }
    printf("\n");

    ClearCoutParm();
}

void ClearCoutParm() {
    FunctionParmTypeNode* curr = cout_parm_list.head;
    while (curr != NULL) {
        FunctionParmTypeNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    cout_parm_list.head = NULL;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        yyin = fopen(yyInputFileName = argv[1], "r");
    } else {
        yyin = stdin;
    }
    if (!yyin) {
        printf("file `%s` doesn't exists or cannot be opened\n", yyInputFileName);
        exit(1);
    }

    table_list.head = NULL;
    cout_parm_list.head = NULL;

    // Start parsing
    yyparse();
    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    yylex_destroy();
    return 0;
}