#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ObjectList.h"

void ExpTypeCheck(Object* a, Object* b, Object* out);

void ExpAssignTypeCheck(Object* dest, Object* val, Object* out);

uint64_t Float2Uint64(float f);

float Uint64ToFloat(uint64_t i);

uint64_t StringLiteral2Uint64(char* str);

char* Uint64ToString(uint64_t i);

#endif  // UTIL_H