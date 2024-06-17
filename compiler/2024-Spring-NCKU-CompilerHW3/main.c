// #define WJCL_LINKED_LIST_IMPLEMENTATION
#include "main.h"
// #define WJCL_HASH_MAP_IMPLEMENTATION
// #include "value_operation.h"

#define debug printf("%s:%d: ############### debug\n", __FILE__, __LINE__)

char* yyInputFileName;
bool compileError;

bool isMain = false;
int indent = 0;
int funcLineNo = 0;
int variableAddress = 0;
ObjectType variableIdentType;

TableList table_list;
FunctionParmList cout_parm_list;
FunctionParmList func_parm_list;
LabelNumList if_label_list, loop_label_list;

// code gen
int ifLabelCount = 0;
int loopLabelCount = 0;

void pushScope() {
    // New scope will be indexed by the size of the table list
    printf("> Create symbol table (scope level %ld)\n", table_list.size);
    scopeLevel++;
    PushTable(&table_list);
}

void dumpScope() {
    printf("\n");
    printf("> Dump symbol table (scope level: %ld)\n", table_list.size - 1);
    TableListNode* curr_table_node = FindCurrTableNode(&table_list);
    ObjectNode* curr_object_node = curr_table_node->object_list->head;
    // Index     Name                Type      Addr      Lineno    Func_sig
    printf("%-10s%-20s%-10s%-10s%-10s%-10s\n", "Index", "Name", "Type", "Addr", "Lineno", "Func_sig");
    if (curr_object_node != NULL) {
        // Loop through the object list
        while (curr_object_node->next != NULL) {
            curr_object_node = curr_object_node->next;
        }

        char* func_sig;
        while (curr_object_node != NULL) {
            if (curr_object_node->object->type == OBJECT_TYPE_FUNCTION) {
                func_sig = curr_object_node->object->symbol->func_sig;
            } else {
                func_sig = "-";
            }
            printf("%-10d%-20s%-10s%-10ld%-10d%-10s\n",
                   curr_object_node->object->symbol->index,
                   curr_object_node->object->symbol->name,
                   objectTypeName[curr_object_node->object->type],
                   curr_object_node->object->symbol->addr,
                   curr_object_node->object->symbol->lineno,
                   func_sig);
            curr_object_node = curr_object_node->prev;
        }
    }
    PopTalble(&table_list);
    scopeLevel--;
}

Object* findVariable_mainTable(char* variableName) {
    return findVariable(variableName, &table_list);
}

Object* findVariable_mainTable_func(char* variableName) {
    return findVariable_func(variableName, &table_list);
}

Object* createVariable(ObjectType variableType, char* variableName, int variableFlag) {
    int tmpAddr = variableAddress;
    if (variableType == OBJECT_TYPE_FUNCTION) {
        tmpAddr = -1;
    }
    printf("> Insert `%s` (addr: %d) to scope level %ld\n", variableName, tmpAddr, table_list.size - 1);
    // Populate the object
    Object* new_object = (Object*)malloc(sizeof(Object));
    new_object->type = variableType;
    new_object->value = 0;
    new_object->flag = variableFlag;
    new_object->symbol = (SymbolData*)malloc(sizeof(SymbolData));
    new_object->symbol->name = variableName;
    new_object->symbol->index = table_list.head->object_list->size;
    new_object->symbol->addr = tmpAddr;
    new_object->symbol->lineno = yylineno;
    if (variableType != OBJECT_TYPE_FUNCTION) {
        variableAddress++;
    }
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
        Object* curr = findVariable_mainTable(ident_name);
        if (curr != NULL) {
            obj_type = curr->type;
            addr = curr->symbol->addr;
        }
    }

    printf("IDENT (name=%s, address=%d)\n", ident_name, addr);
    return obj_type;
}

bool identExp(char* ident_name, Object* out) {
    if (strcmp(ident_name, "endl") != 0) {
        *out = *findVariable_mainTable(ident_name);

        // code gen
        if (out->flag == VAR_FLAG_ARRAY) {
            code("aload %d", out->symbol->addr);
        } else if (out->type == OBJECT_TYPE_INT) {
            code("iload %d", out->symbol->addr);
        } else if (out->type == OBJECT_TYPE_FLOAT) {
            code("fload %d", out->symbol->addr);
        } else if (out->type == OBJECT_TYPE_BOOL) {
            code("iload %d", out->symbol->addr);
        } else if (out->type == OBJECT_TYPE_STR) {
            code("aload %d", out->symbol->addr);
        }
    } else {
        out->value = StringLiteral2Uint64("\\n");
        ldt(out);
    }
    out->type = PrintIdent(ident_name);

    return true;
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
    bool isDone = objectNeg(dest, out);
    printf("NEG\n");
    return isDone;
}

bool objectExpAdd(Object* a, Object* b, Object* out) {
    bool isDone = objectAdd(a, b, out);
    printf("ADD\n");
    return isDone;
}

bool objectExpSub(Object* a, Object* b, Object* out) {
    bool isDone = objectSub(a, b, out);
    printf("SUB\n");
    return isDone;
}

bool objectExpMul(Object* a, Object* b, Object* out) {
    bool isDone = objectMul(a, b, out);
    printf("MUL\n");
    return isDone;
}

bool objectExpDiv(Object* a, Object* b, Object* out) {
    bool isDone = objectDiv(a, b, out);
    printf("DIV\n");
    return isDone;
}

bool objectExpRem(Object* a, Object* b, Object* out) {
    bool isDone = objectRem(a, b, out);
    printf("REM\n");
    return isDone;
}

bool objectExpShr(Object* a, Object* b, Object* out) {
    bool isDone = objectShr(a, b, out);
    printf("SHR\n");
    return isDone;
}

bool objectExpShl(Object* a, Object* b, Object* out) {
    bool isDone = objectShl(a, b, out);
    printf("SHL\n");
    return isDone;
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
    bool isDone = objectBoolNot(a, out);
    printf("NOT\n");
    return isDone;
}

bool objectExpBoolAnd(Object* a, Object* b, Object* out) {
    bool isDone = objectBoolAnd(a, b, out);
    printf("LAN\n");
    return isDone;
}

bool objectExpBoolOr(Object* a, Object* b, Object* out) {
    bool isDone = objectBoolOr(a, b, out);
    printf("LOR\n");
    return isDone;
}

bool objectExpBoolEq(Object* a, Object* b, Object* out) {
    bool isDone = objectBoolEq(a, b, out);
    printf("EQL\n");
    return isDone;
}

bool objectExpBoolNeq(Object* a, Object* b, Object* out) {
    bool isDone = objectBoolNeq(a, b, out);
    printf("NEQ\n");
    return isDone;
}

bool objectExpGtr(Object* a, Object* b, Object* out) {
    bool isDone = objectGtr(a, b, out);
    printf("GTR\n");
    return isDone;
}

bool objectExpLes(Object* a, Object* b, Object* out) {
    bool isDone = objectLes(a, b, out);
    printf("LES\n");
    return isDone;
}

bool objectExpGeq(Object* a, Object* b, Object* out) {
    bool isDone = objectGeq(a, b, out);
    printf("GEQ\n");
    return isDone;
}

bool objectExpLeq(Object* a, Object* b, Object* out) {
    bool isDone = objectLeq(a, b, out);
    printf("LEQ\n");
    return isDone;
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
    bool isDone = objectBinNot(dest, out);
    printf("BNT\n");
    return isDone;
}

bool objectExpBinOr(Object* a, Object* b, Object* out) {
    bool isDone = objectBinOr(a, b, out);
    printf("BOR\n");
    return isDone;
}

bool objectExpBinXor(Object* a, Object* b, Object* out) {
    bool isDone = objectBinXor(a, b, out);
    printf("BXO\n");
    return isDone;
}

bool objectExpBinAnd(Object* a, Object* b, Object* out) {
    bool isDone = objectBinAnd(a, b, out);
    printf("BAN\n");
    return isDone;
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
    } else if (strcmp(op, "++") == 0) {
        isDone = objectExpIncAssign(dest, out);
    } else if (strcmp(op, "--") == 0) {
        isDone = objectExpDecAssign(dest, out);
    } else {
        isDone = false;
    }

    if (isDone) {
        Object* target = findVariable_mainTable(dest->symbol->name);
        if (target != NULL) {
            target->value = dest->value;
        }
    }
    return isDone;
}

bool objectIdentLoad(char* ident_name) {
    Object* target = findVariable_mainTable(ident_name);
    if (target != NULL) {
        if (target->type == OBJECT_TYPE_INT) {
            code("iload %d", target->symbol->addr);
        } else if (target->type == OBJECT_TYPE_FLOAT) {
            code("fload %d", target->symbol->addr);
        } else if (target->type == OBJECT_TYPE_BOOL) {
            code("iload %d", target->symbol->addr);
        } else if (target->type == OBJECT_TYPE_STR) {
            code("aload %d", target->symbol->addr);
        }
        return true;
    }
    return false;
}

bool objectValueAssign(Object* dest, Object* val, Object* out) {
    bool isDone = objectAssign(dest, val, out);
    printf("EQL_ASSIGN\n");
    return isDone;
}

bool objectExpAddAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectAddAssign(a, b, out);
    printf("ADD_ASSIGN\n");
    return isDone;
}

bool objectExpSubAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectSubAssign(a, b, out);
    printf("SUB_ASSIGN\n");
    return isDone;
}

bool objectExpMulAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectMulAssign(a, b, out);
    printf("MUL_ASSIGN\n");
    return isDone;
}

bool objectExpDivAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectDivAssign(a, b, out);
    printf("DIV_ASSIGN\n");
    return isDone;
}

bool objectExpRemAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectRemAssign(a, b, out);
    printf("REM_ASSIGN\n");
    return isDone;
}

bool objectExpBanAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBanAssign(a, b, out);
    printf("BAN_ASSIGN\n");
    return isDone;
}

bool objectExpBorAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBorAssign(a, b, out);
    printf("BOR_ASSIGN\n");
    return isDone;
}

bool objectExpBxoAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBxoAssign(a, b, out);
    printf("BXO_ASSIGN\n");
    return isDone;
}

bool objectExpShrAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShrAssign(a, b, out);
    printf("SHR_ASSIGN\n");
    return isDone;
}

bool objectExpShlAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShlAssign(a, b, out);
    printf("SHL_ASSIGN\n");
    return isDone;
}

bool objectExpIncAssign(Object* a, Object* out) {
    bool isDone = objectIncAssign(a, out);
    printf("INC_ASSIGN\n");
    return isDone;
}
bool objectExpDecAssign(Object* a, Object* out) {
    bool isDone = objectDecAssign(a, out);
    printf("DEC_ASSIGN\n");
    return isDone;
}

bool objectExpCast(ObjectType variableType, Object* dest, Object* out) {
    bool isDone = objectCast(variableType, dest, out);

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

void pushFunParm(ObjectType variableType, char* variableName, int variableFlag) {
    createVariable(variableType, variableName, variableFlag);

    // Push the function parameter to the function parameter list
    FunctionParmTypeNode* new_node = (FunctionParmTypeNode*)malloc(sizeof(FunctionParmTypeNode));
    new_node->type = variableType;
    new_node->value = 0;
    new_node->flag = variableFlag;
    new_node->next = NULL;
    if (func_parm_list.head == NULL) {
        func_parm_list.head = new_node;
    } else {
        FunctionParmTypeNode* curr = func_parm_list.head;
        while (curr->next != NULL) {
            curr = curr->next;
        }

        curr->next = new_node;
    }
}

void clearMainFunParm(char* funcName) {
    Object* funcObj = findVariable_mainTable_func(funcName);

    FunctionParmTypeNode* curr = func_parm_list.head;
    char *func_sig = strdup("("), *new_func_sig;

    while (curr != NULL) {
        if (curr->flag & VAR_FLAG_ARRAY) {
            new_func_sig = strcat_copy(func_sig, "[");
            free(func_sig);
            func_sig = new_func_sig;
        }
        if (curr->type == OBJECT_TYPE_INT) {
            new_func_sig = strcat_copy(func_sig, "I");
        } else if (curr->type == OBJECT_TYPE_FLOAT) {
            new_func_sig = strcat_copy(func_sig, "F");
        } else if (curr->type == OBJECT_TYPE_BOOL) {
            new_func_sig = strcat_copy(func_sig, "Z");
        } else if (curr->type == OBJECT_TYPE_STR) {
            new_func_sig = strcat_copy(func_sig, "Ljava/lang/String;");
        } else {
            new_func_sig = strcat_copy(func_sig, "I");
        }

        free(func_sig);
        func_sig = new_func_sig;

        curr = curr->next;
    }

    new_func_sig = strcat_copy(func_sig, ")");
    free(func_sig);
    func_sig = new_func_sig;

    new_func_sig = strcat_copy(func_sig, funcObj->symbol->func_sig);
    free(func_sig);
    funcObj->symbol->func_sig = new_func_sig;

    clearFunParm(&func_parm_list);

    // code generate
    code(".method public static %s%s", funcName, funcObj->symbol->func_sig);
    if (strcmp(funcName, "main") == 0) {
        codeRaw(".limit stack 100");
        codeRaw(".limit locals 100");
    } else {
        codeRaw(".limit stack 20");
        codeRaw(".limit locals 20");
    }
}

void clearFunParm(FunctionParmList* target_func_parm_list) {
    // Clear the function parameter list
    FunctionParmTypeNode* curr = target_func_parm_list->head;
    while (curr != NULL) {
        FunctionParmTypeNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    target_func_parm_list->head = NULL;
}

void createFunction(ObjectType variableType, char* funcName) {
    printf("func: %s\n", funcName);

    Object* funcObj = createVariable(OBJECT_TYPE_FUNCTION, funcName, VAR_FLAG_DEFAULT);
    if (strcmp(funcName, "main") == 0) {
        funcObj->symbol->func_sig = strdup("V");
        isMain = true;
    } else {
        if (variableType == OBJECT_TYPE_INT)
            funcObj->symbol->func_sig = strdup("I");
        else if (variableType == OBJECT_TYPE_FLOAT)
            funcObj->symbol->func_sig = strdup("F");
        else if (variableType == OBJECT_TYPE_BOOL)
            funcObj->symbol->func_sig = strdup("B");
        else if (variableType == OBJECT_TYPE_STR)
            funcObj->symbol->func_sig = strdup("Ljava/lang/String;");
        else if (variableType == OBJECT_TYPE_VOID)
            funcObj->symbol->func_sig = strdup("V");
        else
            funcObj->symbol->func_sig = strdup("I");
    }
}

Object* callFunction(char* funcName) {
    Object* funcObj = findVariable_mainTable(funcName);
    PrintIdent(funcName);

    printf("call: %s%s\n", funcName, funcObj->symbol->func_sig);

    // code gen
    code("invokestatic %s/%s%s", "Main", funcName, funcObj->symbol->func_sig);
    return funcObj;
}

void debugPrintInst(char instc, Object* a, Object* b, Object* out) {
}

bool defineVariable(Object* variable, Object* value) {
    // TODO: implement implicit casting
    if (variable == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    variable->value = value->value;
    if (variable->type == OBJECT_TYPE_AUTO) {
        variable->type = value->type;
    }

    // code gen
    if (variable->type == OBJECT_TYPE_INT) {
        code("istore %d", variable->symbol->addr);
    } else if (variable->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", variable->symbol->addr);
    } else if (variable->type == OBJECT_TYPE_BOOL) {
        code("istore %d", variable->symbol->addr);
    } else if (variable->type == OBJECT_TYPE_STR) {
        code("astore %d", variable->symbol->addr);
    }
    return true;
}

void pushFunInParm(Object* variable) {
    FunctionParmTypeNode* new_node = (FunctionParmTypeNode*)malloc(sizeof(FunctionParmTypeNode));
    if (variable->type == OBJECT_TYPE_FUNCTION) {
        new_node->type = funcReturnType(variable);
    } else {
        new_node->type = variable->type;
    }
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

    // code gen
    code("invokevirtual java/io/PrintStream/print(%s)V", objectJavaTypeName[new_node->type]);
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
    clearFunParm(&cout_parm_list);
}

bool returnObject(Object* obj) {
    bool isDone = false;
    if (isMain) {
        isDone = true;
    }
    return isDone;
}

bool breakLoop() {
    bool isDone = false;
    printf("BREAK\n");

    isDone = true;

    // code gen
    code("goto loop_end_%d", loop_label_list.head->labelNum);

    return isDone;
}

bool ifBegin(Object* a) {
    bool isDone = false;
    printf("IF\n");

    isDone = true;

    // code gen
    ++ifLabelCount;
    pushLabelNum(ifLabelCount, &if_label_list);
    code("ifeq if_false_%d", if_label_list.head->labelNum);
    return isDone;
}

bool ifEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("goto if_end_%d", if_label_list.head->labelNum);
    code("if_false_%d:", if_label_list.head->labelNum);
    return isDone;
}

bool ifOnlyEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("if_end_%d:", if_label_list.head->labelNum);
    popLabelNum(&if_label_list);
    return isDone;
}

bool elseBegin() {
    bool isDone = false;
    printf("ELSE\n");

    isDone = true;
    return isDone;
}

bool elseEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    if (if_label_list.head == NULL) {
        fprintf(stderr, "Error: else without if\n");
        return false;
    }
    code("if_end_%d:", if_label_list.head->labelNum);
    popLabelNum(&if_label_list);
    return isDone;
}

bool whileBegin() {
    bool isDone = false;
    printf("WHILE\n");

    isDone = true;

    // code gen
    loopLabelCount++;
    pushLabelNum(loopLabelCount, &loop_label_list);
    code("while_begin_%d:", loop_label_list.head->labelNum);
    return isDone;
}

bool whileBodyBegin() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("ifeq loop_end_%d", loop_label_list.head->labelNum);
    return isDone;
}

bool whileEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("goto while_begin_%d", loop_label_list.head->labelNum);
    code("loop_end_%d:", loop_label_list.head->labelNum);
    popLabelNum(&loop_label_list);
    return isDone;
}

bool forBegin() {
    bool isDone = false;
    printf("FOR\n");

    isDone = true;

    // code gen
    loopLabelCount++;
    pushLabelNum(loopLabelCount, &loop_label_list);
    // code("for_begin_%d:", loop_label_list.head->labelNum);
    return isDone;
}

bool forInitEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("for_initEnd_%d:", loop_label_list.head->labelNum);
    return isDone;
}

bool forConditionEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("ifeq loop_end_%d", loop_label_list.head->labelNum);
    code("goto for_headerEnd_%d", loop_label_list.head->labelNum);
    code("for_conditionEnd_%d:", loop_label_list.head->labelNum);
    return isDone;
}

bool forHeaderEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("goto for_initEnd_%d", loop_label_list.head->labelNum);
    code("for_headerEnd_%d:", loop_label_list.head->labelNum);
    return isDone;
}

bool forEnd() {
    bool isDone = false;

    isDone = true;

    // code gen
    code("goto for_conditionEnd_%d", loop_label_list.head->labelNum);
    code("loop_end_%d:", loop_label_list.head->labelNum);
    popLabelNum(&loop_label_list);
    return isDone;
}

bool functionEnd(ObjectType returnType) {
    bool isDone = false;

    isDone = true;

    // code gen
    if (isMain) {
        codeRaw("return");
        isMain = false;
    } else {
        if (returnType == OBJECT_TYPE_INT) {
            codeRaw("ireturn");
        } else if (returnType == OBJECT_TYPE_FLOAT) {
            codeRaw("freturn");
        } else if (returnType == OBJECT_TYPE_BOOL) {
            codeRaw("ireturn");
        } else if (returnType == OBJECT_TYPE_STR) {
            codeRaw("areturn");
        } else {
            codeRaw("return");
        }
    }
    codeRaw(".end method");
    return isDone;
}

bool arrayCreate(Object* arr) {
    bool isDone = false;

    isDone = true;

    // code gen
    code("newarray %s", objectTypeName[arr->type]);
    code("astore %d", arr->symbol->addr);
    return isDone;
}

bool objectArrayGet(Object* arr) {
    bool isDone = false;

    isDone = true;

    // code gen
    if (arr->type == OBJECT_TYPE_INT)
        codeRaw("iaload");
    else if (arr->type == OBJECT_TYPE_FLOAT)
        codeRaw("faload");
    else if (arr->type == OBJECT_TYPE_BOOL)
        codeRaw("iaload");
    else if (arr->type == OBJECT_TYPE_STR)
        codeRaw("aaload");
    return isDone;
}

int main(int argc, char* argv[]) {
    char* outputFileName = NULL;
    if (argc == 3) {
        yyin = fopen(yyInputFileName = argv[1], "r");
        yyout = fopen(outputFileName = argv[2], "w");
    } else if (argc == 2) {
        yyin = fopen(yyInputFileName = argv[1], "r");
        yyout = stdout;
    } else {
        printf("require input file");
        exit(1);
    }
    if (!yyin) {
        printf("file `%s` doesn't exists or cannot be opened\n", yyInputFileName);
        exit(1);
    }
    if (!yyout) {
        printf("file `%s` doesn't exists or cannot be opened\n", outputFileName);
        exit(1);
    }

    if_label_list.head = NULL;
    loop_label_list.head = NULL;

    codeRaw(".source Main.j");
    codeRaw(".class public Main");
    codeRaw(".super java/lang/Object");
    codeRaw("");
    scopeLevel = -1;

    yyparse();
    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    yylex_destroy();
    return 0;
}