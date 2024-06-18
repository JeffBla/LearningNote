#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ExpOp.h"
#include "TableList.h"
#include "Util.h"
#include "compiler_common.h"

typedef struct FunctionParmTypeNode {
    struct FunctionParmTypeNode* next;
    ObjectType type;
    int flag;
    int isNewLine;
    uint64_t value;
} FunctionParmTypeNode;

typedef struct FunctionParmList {
    FunctionParmTypeNode* head;
} FunctionParmList;

extern bool isMain;
extern int ifLabelCount;

void pushScope();
void dumpScope();

void pushFunParm(ObjectType variableType, char* variableName, int parmFlag);
void createFunction(ObjectType variableType, char* funcName);
void clearMainFunParm();
void clearFunParm(FunctionParmList* target_func_parm_list);
Object* callFunction(char* funcName);
void pushFunInParm(Object* variable);

bool defineVariable(Object* variable, Object* value);

Object* findVariable_mainTable(char* variableName);
Object* findVariable_mainTable_func(char* variableName);
Object* createVariable(ObjectType variableType, char* variableName, int variableFlag);
ObjectType PrintIdent(char* ident_name);

// bool initVariable(ObjectType variableType, LinkedList* arraySubscripts, char* variableName);

// void functionLocalsBegin();
void functionBegin(ObjectType returnType, char* funcName);
bool functionEnd(ObjectType returnType);

bool returnObject(Object* obj);
bool breakLoop();

// void functionArgsBegin();
// void functionArgPush(Object* obj);

void stdoutPrint();
bool stdoutPrint_obj(Object* obj);
void ClearCoutParm();

// Expressions
bool identExp(char* ident_name, Object* out);

bool objectExpression(const char* op, Object* a, Object* b, Object* out);
bool objectExpNeg(Object* dest, Object* out);
bool objectExpAdd(Object* a, Object* b, Object* out);
bool objectExpSub(Object* a, Object* b, Object* out);
bool objectExpMul(Object* a, Object* b, Object* out);
bool objectExpDiv(Object* a, Object* b, Object* out);
bool objectExpRem(Object* a, Object* b, Object* out);
bool objectExpShr(Object* a, Object* b, Object* out);
bool objectExpShl(Object* a, Object* b, Object* out);

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
bool objectIdentLoad(char* ident_name);
bool objectValueAssign(Object* dest, Object* val, Object* out);
bool objectExpAddAssign(Object* a, Object* b, Object* out);
bool objectExpSubAssign(Object* a, Object* b, Object* out);
bool objectExpMulAssign(Object* a, Object* b, Object* out);
bool objectExpDivAssign(Object* a, Object* b, Object* out);
bool objectExpRemAssign(Object* a, Object* b, Object* out);
bool objectExpBanAssign(Object* a, Object* b, Object* out);
bool objectExpBorAssign(Object* a, Object* b, Object* out);
bool objectExpBxoAssign(Object* a, Object* b, Object* out);
bool objectExpShrAssign(Object* a, Object* b, Object* out);
bool objectExpShlAssign(Object* a, Object* b, Object* out);

bool objectExpIncAssign(Object* a, Object* out);
bool objectExpDecAssign(Object* a, Object* out);

bool objectExpCast(ObjectType variableType, Object* dest, Object* out);

bool ifBegin(Object* a);
bool ifEnd();
bool ifOnlyEnd();
bool elseBegin();
bool elseEnd();

bool whileBegin();
bool whileBodyBegin();
bool whileEnd();

bool forBegin();
bool forInitEnd();
bool forConditionEnd();
bool forHeaderEnd();
// bool foreachHeaderEnd(Object* obj);
bool forEnd();

bool arrayCreate(Object* arr);
bool objectArrayGet(Object* arr);
bool multiArrayCreate(ObjectType type, int dim, Object* arr);
// LinkedList* arraySubscriptBegin(Object* index);
// bool arraySubscriptPush(LinkedList* arraySubscripts, Object* index);
// bool arraySubscriptEnd(LinkedList* arraySubscripts);

#endif