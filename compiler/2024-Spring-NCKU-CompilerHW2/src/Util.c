#include "Util.h"

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