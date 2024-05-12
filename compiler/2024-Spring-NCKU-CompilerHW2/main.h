#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TableList.h"
#include "compiler_common.h"

extern FILE* yyin;
extern bool compileError;
int yyparse();
int yylex();
int yylex_destroy();

#define VAR_FLAG_DEFAULT 0
#define VAR_FLAG_ARRAY 0b00000001
#define VAR_FLAG_POINTER 0b00000010

/**
 * Talbe List keep each scope's object list
 * Object List keep each object in the scope
 */

typedef struct FunctionParmTypeNode {
    struct FunctionParmTypeNode* next;
    ObjectType type;
} FunctionParmTypeNode;

typedef struct FunctionParmList {
    FunctionParmTypeNode* head;
} FunctionParmList;

void pushScope();
void dumpScope();

void pushFunParm(ObjectType variableType, char* variableName, int parmFlag);
void createFunction(ObjectType variableType, char* funcName);
void pushFunInParm(Object* variable);

Object* findVariable(char* variableName);
Object* createVariable(ObjectType variableType, char* variableName, int variableFlag);
ObjectType PrintIdent(char* ident_name);

bool objectExpression(const char* op, Object* a, Object* b, Object* out);
bool objectExpAdd(Object* a, Object* b, Object* out);
bool objectExpSub(Object* a, Object* b, Object* out);
bool objectExpMul(Object* a, Object* b, Object* out);
bool objectExpDiv(Object* a, Object* b, Object* out);
bool objectExpRem(Object* a, Object* b, Object* out);
bool objectExpShr(Object* a, Object* b, Object* out);
bool objectExpShl(Object* a, Object* b, Object* out);
bool objectExpNeg(Object* dest, Object* out);

bool objectExpBoolean(const char* op, Object* a, Object* b, Object* out);
bool objectExpBoolNot(Object* a, Object* out);
bool objectExpBoolAnd(Object* a, Object* b, Object* out);
bool objectExpBoolOr(Object* a, Object* b, Object* out);
bool objectExpBoolEq(Object* a, Object* b, Object* out);
bool objectExpBoolNeq(Object* a, Object* b, Object* out);
bool objectExpGtr(Object* a, Object* b, Object* out);
bool objectExpLes(Object* a, Object* b, Object* out);
bool objectExpGeq(Object* a, Object* b, Object* out);
bool objectExpLeq(Object* a, Object* b, Object* out);

bool objectExpBinary(const char* op, Object* a, Object* b, Object* out);
bool objectExpBinNot(Object* dest, Object* out);
bool objectExpBinOr(Object* a, Object* b, Object* out);
bool objectExpBinXor(Object* a, Object* b, Object* out);
bool objectExpBinAnd(Object* a, Object* b, Object* out);

bool objectExpAssign(const char* op, Object* dest, Object* val, Object* out);
bool objectValueAssign(Object* dest, Object* val, Object* out);
bool objectExpAddAssign(Object* dest, Object* val, Object* out);
bool objectExpSubAssign(Object* dest, Object* val, Object* out);
bool objectExpMulAssign(Object* dest, Object* val, Object* out);
bool objectExpDivAssign(Object* dest, Object* val, Object* out);
bool objectExpRemAssign(Object* dest, Object* val, Object* out);
bool objectExpBanAssign(Object* dest, Object* val, Object* out);
bool objectExpBorAssign(Object* dest, Object* val, Object* out);
bool objectExpBxoAssign(Object* dest, Object* val, Object* out);
bool objectExpShrAssign(Object* dest, Object* val, Object* out);
bool objectExpShlAssign(Object* dest, Object* val, Object* out);

bool objectIncAssign(Object* a, Object* out);
bool objectDecAssign(Object* a, Object* out);
bool objectCast(ObjectType variableType, Object* dest, Object* out);

void stdoutPrint();
void ClearCoutParm();

/// Util
// Object Num2Bool(Object* obj) {
//     Object out;
//     if (obj->type == OBJECT_TYPE_INT || obj->type == OBJECT_TYPE_FLOAT) {
//         out.type = OBJECT_TYPE_BOOL;
//         out.value = obj->value != 0;
//     } else if (obj->type == OBJECT_TYPE_BOOL) {
//         out.type = OBJECT_TYPE_BOOL;
//         out.value = obj->value;
//     } else {
//         out.type = OBJECT_TYPE_UNDEFINED;
//         out.value = 0;
//     }
//     return out;
// };

void ExpTypeCheck(Object* a, Object* b, Object* out);

void ExpAssignTypeCheck(Object* dest, Object* val, Object* out);

#endif