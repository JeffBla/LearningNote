#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ObjectList.h"
#include "TableList.h"

#define VAR_FLAG_DEFAULT 0
#define VAR_FLAG_ARRAY 0b00000001
#define VAR_FLAG_POINTER 0b00000010

#define code(format, ...) \
    fprintf(yyout, "%*s" format "\n", scopeLevel << 2, "", __VA_ARGS__)
#define codeRaw(code) \
    fprintf(yyout, "%*s" code "\n", scopeLevel << 2, "")

#define getBool(val) (*(int8_t*)&((val)->value))
#define getByte(val) (*(int8_t*)&((val)->value))
#define getChar(val) (*(int8_t*)&((val)->value))
#define getShort(val) (*(int16_t*)&((val)->value))
#define getInt(val) (*(int32_t*)&((val)->value))
#define getLong(val) (*(int64_t*)&((val)->value))
#define getFloat(val) (*(float*)&((val)->value))
#define getDouble(val) (*(double*)&((val)->value))
#define getString(val) (*(char**)&((val)->value))

#define asVal(val) (*(int64_t*)&(val))

#define iload(var) code("iload %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define lload(var) code("lload %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define fload(var) code("fload %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define dload(var) code("dload %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define aload(var) code("aload %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)

#define istore(var) code("istore %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define lstore(var) code("lstore %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define fstore(var) code("fstore %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define dstore(var) code("dstore %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)
#define astore(var) code("astore %" PRId64 " ; %s", (var)->symbol->addr, (var)->symbol->name)

#define ldz(val) code("ldc %d", getBool(val))
#define ldb(val) code("ldc %d", getByte(val))
#define ldc(val) code("ldc %d", getChar(val))
#define lds(val) code("ldc %d", getShort(val))
#define ldi(val) code("ldc %d", getInt(val))
#define ldl(val) code("ldc_w %" PRId64, getLong(val))
#define ldf(val) code("ldc %.6f", getFloat(val))
#define ldd(val) code("ldc_w %lf", getDouble(val))
#define ldt(val) code("ldc \"%s\"", getString(val))

typedef struct labelNumNode {
    struct labelNumNode* next;
    int labelNum;
} LabelNumNode;

typedef struct labelNumList {
    LabelNumNode* head;
} LabelNumList;

extern FILE* yyout;
extern FILE* yyin;
extern bool compileError;
extern int scopeLevel;
int yyparse();
int yylex();
int yylex_destroy();

extern const char* objectTypeName[];
extern const char* objectJavaTypeName[];

void pushLabelNum(int labelNum, LabelNumList* labelNumList);

void popLabelNum(LabelNumList* labelNumList);

Object* findVariable(char* variableName, TableList* table_list);

Object* findVariable_func(char* variableName, TableList* table_list);

ObjectType funcReturnType(Object* func);

char* strcat_copy(const char* str1, const char* str2);

void ExpTypeCheck(Object* a, Object* b, Object* out);

void ExpAssignTypeCheck(Object* dest, Object* val, Object* out);

uint64_t Float2Uint64(float f);

float Uint64ToFloat(uint64_t i);

uint64_t StringLiteral2Uint64(char* str);

char* Uint64ToString(uint64_t i);

#endif  // UTIL_H