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
// int scopeLevel = -1;  // scope level equ to the size of the table list
int funcLineNo = 0;
int variableAddress = 0;
ObjectType variableIdentType;

TableList table_list;
FunctionParmList cout_parm_list;

void pushScope() {
    // New scope will be indexed by the size of the table list
    printf("> Create symbol table (scope level %ld)\n", table_list.size);
    variableAddress = 0;
    PushTable(&table_list);
}

void dumpScope() {
    printf("\n");
    printf("> Dump symbol table (scope level %ld)\n", table_list.size - 1);
    TableListNode* curr_table_node = FindCurrTableNode(&table_list);
    ObjectNode* curr_object_node = curr_table_node->object_list->head;
    // Index     Name                Type      Addr      Lineno    Func_sig
    printf("%-8s %-20s %-8s %-8s %-8s %-8s\n", "Index", "Name", "Type", "Addr", "Lineno", "Func_sig");
    if (curr_object_node != NULL) {
        // Loop through the object list
        while (curr_object_node->next != NULL) {
            curr_object_node = curr_object_node->next;
        }

        while (curr_object_node != NULL) {
            printf("%-8d %-20s %-8s %-8ld %-8d %-8s\n",
                   curr_object_node->object->symbol->index,
                   curr_object_node->object->symbol->name,
                   objectTypeName[curr_object_node->object->type],
                   curr_object_node->object->symbol->addr,
                   curr_object_node->object->symbol->lineno,
                   "-");
            curr_object_node = curr_object_node->prev;
        }
    }
    PopTalble(&table_list);
}

Object* createVariable(ObjectType variableType, char* variableName, int variableFlag) {
    printf("> Insert `%s` (addr: %d) to scope level %ld\n", variableName, variableAddress++, table_list.size - 1);
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
    PushObject(new_object, &table_list);
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
    ExpTypeCheck(dest, val, out);
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

bool objectExpNeg(Object* dest, Object* out) {
    out->type = dest->type;
    out->value = -dest->value;
    printf("NEG\n");
    return true;
}

bool objectExpAdd(Object* a, Object* b, Object* out) {
    if (out->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(Uint64ToFloat(a->value) + Uint64ToFloat(b->value));
    } else {
        out->value = a->value + b->value;
    }
    printf("ADD\n");
    return true;
}

bool objectExpSub(Object* a, Object* b, Object* out) {
    if (out->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(Uint64ToFloat(a->value) - Uint64ToFloat(b->value));
    } else {
        out->value = a->value - b->value;
    }
    printf("SUB\n");
    return true;
}

bool objectExpMul(Object* a, Object* b, Object* out) {
    if (out->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(Uint64ToFloat(a->value) * Uint64ToFloat(b->value));
    } else {
        out->value = a->value * b->value;
    }
    printf("MUL\n");
    return true;
}

bool objectExpDiv(Object* a, Object* b, Object* out) {
    if (out->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(Uint64ToFloat(a->value) / Uint64ToFloat(b->value));
    } else {
        out->value = a->value / b->value;
    }
    printf("DIV\n");
    return true;
}

bool objectExpRem(Object* a, Object* b, Object* out) {
    out->value = a->value % b->value;
    printf("REM\n");
    return true;
}

bool objectExpShr(Object* a, Object* b, Object* out) {
    out->value = a->value >> b->value;
    printf("SHR\n");
    return true;
}

bool objectExpShl(Object* a, Object* b, Object* out) {
    out->value = a->value << b->value;
    printf("SHL\n");
    return true;
}

bool objectExpBoolean(const char* op, Object* a, Object* b, Object* out) {
    bool isDone;
    out->type = OBJECT_TYPE_BOOL;
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
    out->value = !a->value;
    printf("NOT\n");
    return true;
}

bool objectExpBoolAnd(Object* a, Object* b, Object* out) {
    out->value = a->value && b->value;
    printf("LAN\n");
    return true;
}

bool objectExpBoolOr(Object* a, Object* b, Object* out) {
    out->value = a->value || b->value;
    printf("LOR\n");
    return true;
}

bool objectExpBoolEq(Object* a, Object* b, Object* out) {
    out->value = a->value == b->value;
    printf("EQL\n");
    return true;
}

bool objectExpBoolNeq(Object* a, Object* b, Object* out) {
    out->value = a->value != b->value;
    printf("NEQ\n");
    return true;
}

bool objectExpGtr(Object* a, Object* b, Object* out) {
    bool isDone = false;
    out->value = a->value > b->value;
    printf("GTR\n");
    return true;
}

bool objectExpLes(Object* a, Object* b, Object* out) {
    out->value = a->value < b->value;
    printf("LES\n");
    return true;
}

bool objectExpGeq(Object* a, Object* b, Object* out) {
    out->value = a->value >= b->value;
    printf("GEQ\n");
    return true;
}

bool objectExpLeq(Object* a, Object* b, Object* out) {
    out->value = a->value <= b->value;
    printf("LEQ\n");
    return true;
}

bool objectExpBinary(const char* op, Object* a, Object* b, Object* out) {
    bool isDone;
    ExpTypeCheck(a, b, out);
    switch (op[0]) {
        case '~':
            isDone = objectExpBinNot(a, out);
            break;
        case '|':
            isDone = objectExpBinOr(a, b, out);
            break;
        case '^':
            isDone = objectExpBinXor(a, b, out);
            break;
        case '&':
            isDone = objectExpBinAnd(a, b, out);
            break;
        default:
            isDone = false;
            break;
    }
    return isDone;
}

bool objectExpBinNot(Object* dest, Object* out) {
    out->value = ~dest->value;
    printf("BNT\n");
    return true;
}

bool objectExpBinOr(Object* a, Object* b, Object* out) {
    out->value = a->value | b->value;
    printf("BOR\n");
    return true;
}

bool objectExpBinXor(Object* a, Object* b, Object* out) {
    out->value = a->value ^ b->value;
    printf("BXO\n");
    return true;
}

bool objectExpBinAnd(Object* a, Object* b, Object* out) {
    out->value = a->value & b->value;
    printf("BAN\n");
    return true;
}

bool objectExpAssign(const char* op, Object* dest, Object* val, Object* out) {
    bool isDone;
    ExpAssignTypeCheck(dest, val, out);
    if (strcmp(op, "=") == 0) {
        isDone = objectValueAssign(dest, val, out);
    } else if (strcmp(op, "+=") == 0) {
        isDone = objectExpAddAssign(dest, val, out);
    } else if (strcmp(op, "-=") == 0) {
        isDone = objectExpSubAssign(dest, val, out);
    } else if (strcmp(op, "*=") == 0) {
        isDone = objectExpMulAssign(dest, val, out);
    } else if (strcmp(op, "/=") == 0) {
        isDone = objectExpDivAssign(dest, val, out);
    } else if (strcmp(op, "%=") == 0) {
        isDone = objectExpRemAssign(dest, val, out);
    } else if (strcmp(op, "&=") == 0) {
        isDone = objectExpBanAssign(dest, val, out);
    } else if (strcmp(op, "|=") == 0) {
        isDone = objectExpBorAssign(dest, val, out);
    } else if (strcmp(op, "^=") == 0) {
        isDone = objectExpBxoAssign(dest, val, out);
    } else if (strcmp(op, ">>=") == 0) {
        isDone = objectExpShrAssign(dest, val, out);
    } else if (strcmp(op, "<<=") == 0) {
        isDone = objectExpShlAssign(dest, val, out);
    } else {
        isDone = false;
    }

    if (isDone) {
        Object* target = findVariable(dest->symbol->name);
        if (target != NULL) {
            target->value = out->value;
        }
    }
    return isDone;
}

bool objectValueAssign(Object* dest, Object* val, Object* out) {
    objectCast(dest->type, val, dest);
    out->value = dest->value;
    printf("EQL_ASSIGN\n");
    return true;
}

bool objectExpAddAssign(Object* dest, Object* val, Object* out) {
    Object val_casted = *val;
    objectCast(dest->type, val, &val_casted);
    out->value = dest->value += val_casted.value;
    printf("ADD_ASSIGN\n");
    return true;
}

bool objectExpSubAssign(Object* dest, Object* val, Object* out) {
    Object val_casted = *val;
    objectCast(dest->type, val, &val_casted);
    out->value = dest->value -= val->value;
    printf("SUB_ASSIGN\n");
    return true;
}

bool objectExpMulAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value *= val->value;
    printf("MUL_ASSIGN\n");
    return true;
}

bool objectExpDivAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value /= val->value;
    printf("DIV_ASSIGN\n");
    return true;
}

bool objectExpRemAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value %= val->value;
    printf("REM_ASSIGN\n");
    return true;
}

bool objectExpBanAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value &= val->value;
    printf("BAN_ASSIGN\n");
    return true;
}

bool objectExpBorAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value |= val->value;
    printf("BOR_ASSIGN\n");
    return true;
}

bool objectExpBxoAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value ^= val->value;
    printf("BXO_ASSIGN\n");
    return true;
}

bool objectExpShrAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value >>= val->value;
    printf("SHR_ASSIGN\n");
    return true;
}

bool objectExpShlAssign(Object* dest, Object* val, Object* out) {
    out->value = dest->value <<= val->value;
    printf("SHL_ASSIGN\n");
    return true;
}

bool objectIncAssign(Object* a, Object* out) {
    return false;
}

bool objectDecAssign(Object* a, Object* out) {
    return false;
}

bool objectCast(ObjectType variableType, Object* dest, Object* out) {
    bool isDone = false;
    out->type = variableType;
    if (dest->type == variableType) {
        out->value = dest->value;
        isDone = true;
    } else {
        if (dest->type == OBJECT_TYPE_INT && variableType == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64((float)dest->value);
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_INT && variableType == OBJECT_TYPE_BOOL) {
            out->value = dest->value != 0;
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_FLOAT && variableType == OBJECT_TYPE_INT) {
            out->value = (int)Uint64ToFloat(dest->value);
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_FLOAT && variableType == OBJECT_TYPE_BOOL) {
            out->value = Uint64ToFloat(dest->value) != 0.0;
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_BOOL && variableType == OBJECT_TYPE_INT) {
            out->value = dest->value;
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_BOOL && variableType == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64((float)dest->value);
            isDone = true;
        }
    }

#ifdef DEBUG
    if (isDone) {
        if (variableType == OBJECT_TYPE_FLOAT) {
            printf("CAST to %s. Value: %f\n", objectTypeName[variableType], Uint64ToFloat(out->value));
        } else if (variableType == OBJECT_TYPE_INT | variableType == OBJECT_TYPE_BOOL) {
            printf("CAST to %s. Value: %ld\n", objectTypeName[variableType], out->value);
        }
    }
#else
    if (isDone) {
        printf("Cast to %s\n", objectTypeName[variableType]);
    }
#endif

    return isDone;
}

Object* findVariable(char* variableName) {
    if (variableName == NULL) {
        return NULL;
    }

    TableListNode* curr_table_node = FindCurrTableNode(&table_list);
    ObjectNode* obj_node = curr_table_node->object_list->head;
    while (obj_node != NULL) {
        if (strcmp(obj_node->object->symbol->name, variableName) == 0) {
            return obj_node->object;
        }
        obj_node = obj_node->next;
    }
    return NULL;
}

void pushFunInParm(Object* variable) {
    FunctionParmTypeNode* new_node = (FunctionParmTypeNode*)malloc(sizeof(FunctionParmTypeNode));
    new_node->type = variable->type;
    new_node->value = variable->value;
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
#ifdef DEBUG
        if (curr->type == OBJECT_TYPE_FLOAT)
            printf(" -> %f", Uint64ToFloat(curr->value));
        else if (curr->type == OBJECT_TYPE_INT)
            printf(" -> %ld", curr->value);
        else if (curr->type == OBJECT_TYPE_BOOL)
            printf(" -> %s", curr->value ? "true" : "false");
        else if (curr->type == OBJECT_TYPE_STR)
            printf(" -> \"%s\"", Uint64ToString(curr->value));
#endif
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
    table_list.size = 0;
    cout_parm_list.head = NULL;

    // Start parsing
    yyparse();
    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    yylex_destroy();
    return 0;
}
