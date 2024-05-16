#ifndef EXP_OP_H
#define EXP_OP_H

#include "ObjectList.h"
#include "TableList.h"
#include "Util.h"

// bool objectExp(const char* op, Object* a, Object* b, Object* out);
bool objectNeg(Object* dest, Object* out);
bool objectAdd(Object* a, Object* b, Object* out);
bool objectSub(Object* a, Object* b, Object* out);
bool objectMul(Object* a, Object* b, Object* out);
bool objectDiv(Object* a, Object* b, Object* out);
bool objectRem(Object* a, Object* b, Object* out);
bool objectShr(Object* a, Object* b, Object* out);
bool objectShl(Object* a, Object* b, Object* out);

// bool objectExpBool(const char* op, Object* a, Object* b, Object* out);
bool objectBoolNot(Object* a, Object* out);
bool objectBoolAnd(Object* a, Object* b, Object* out);
bool objectBoolOr(Object* a, Object* b, Object* out);
bool objectBoolEq(Object* a, Object* b, Object* out);
bool objectBoolNeq(Object* a, Object* b, Object* out);
bool objectGtr(Object* a, Object* b, Object* out);
bool objectLes(Object* a, Object* b, Object* out);
bool objectGeq(Object* a, Object* b, Object* out);
bool objectLeq(Object* a, Object* b, Object* out);

// bool objectExpBin(const char* op, Object* a, Object* b, Object* out);
bool objectBinNot(Object* dest, Object* out);
bool objectBinOr(Object* a, Object* b, Object* out);
bool objectBinXor(Object* a, Object* b, Object* out);
bool objectBinAnd(Object* a, Object* b, Object* out);

// bool objectExpAssign(const char* op, Object* dest, Object* val, Object* out);
bool objectAssign(Object* dest, Object* val, Object* out);
bool objectAddAssign(Object* a, Object* b, Object* out);
bool objectSubAssign(Object* a, Object* b, Object* out);
bool objectMulAssign(Object* a, Object* b, Object* out);
bool objectDivAssign(Object* a, Object* b, Object* out);
bool objectRemAssign(Object* a, Object* b, Object* out);
bool objectBanAssign(Object* a, Object* b, Object* out);
bool objectBorAssign(Object* a, Object* b, Object* out);
bool objectBxoAssign(Object* a, Object* b, Object* out);
bool objectShrAssign(Object* a, Object* b, Object* out);
bool objectShlAssign(Object* a, Object* b, Object* out);

bool objectIncAssign(Object* a, Object* out);
bool objectDecAssign(Object* a, Object* out);

bool objectCast(ObjectType variableType, Object* dest, Object* out);

#endif