#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef struct ObjectNode {
    struct ObjectNode* next;
    Object* object;
} ObjectNode;

typedef struct objectList {
    ObjectNode* head;
} ObjectList;

typedef struct TableListNode {
    struct TableListNode* next;
    ObjectList* object_list;
} TableListNode;

typedef struct TableLinkedList {
    TableListNode* head;
} TableList;

typedef struct FunctionParmTypeNode {
    struct FunctionParmTypeNode* next;
    ObjectType type;
} FunctionParmTypeNode;

typedef struct FunctionParmList {
    FunctionParmTypeNode* head;
} FunctionParmList;

TableListNode* FindCurrTableNode();
void AddObject(Object* object);
void AddTable();

void pushScope();
void dumpScope();

void pushFunParm(ObjectType variableType, char* variableName, int parmFlag);
void createFunction(ObjectType variableType, char* funcName);
void pushFunInParm(Object* variable);

Object* findVariable(char* variableName);
Object* createVariable(ObjectType variableType, char* variableName, int variableFlag);
ObjectType PrintIdent(char* ident_name);
bool objectExpression(char op, Object* a, Object* b, Object* out);
bool objectExpBinary(char op, Object* a, Object* b, Object* out);
bool objectExpBoolean(char op, Object* a, Object* b, Object* out);
bool objectExpAssign(char op, Object* dest, Object* val, Object* out);
bool objectValueAssign(Object* dest, Object* val, Object* out);
bool objectNotBinaryExpression(Object* dest, Object* out);
bool objectNotExpression(Object* dest, Object* out);
bool objectNegExpression(Object* dest, Object* out);
bool objectIncAssign(Object* a, Object* out);
bool objectDecAssign(Object* a, Object* out);
bool objectCast(ObjectType variableType, Object* dest, Object* out);

void stdoutPrint();
void ClearCoutParm();

#endif