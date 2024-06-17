#include "ExpOp.h"

static int BoolNotLabelNum = 0;
static int BoolOrLabelNum = 0;
static int BoolAndLabelNum = 0;
static int BoolCmpLabelNum = 0;

bool objectNeg(Object* dest, Object* out) {
    out->type = dest->type;
    if (dest->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(-Uint64ToFloat(dest->value));
        codeRaw("fneg");
    } else {
        out->value = -dest->value;
        codeRaw("ineg");
    }
    return true;
}

bool objectAdd(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) + b->value);
        } else {
            out->value = Uint64ToFloat(a->value) + b->value;
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(a->value + Uint64ToFloat(b->value));
        } else {
            out->value = a->value + Uint64ToFloat(b->value);
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) + Uint64ToFloat(b->value));
            codeRaw("fadd");
        } else {
            out->value = Uint64ToFloat(a->value) + Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value + b->value;
        isDone = true;
        codeRaw("iadd");
    }
    return isDone;
}

bool objectSub(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) - b->value);
        } else {
            out->value = Uint64ToFloat(a->value) - b->value;
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(a->value - Uint64ToFloat(b->value));
        } else {
            out->value = a->value - Uint64ToFloat(b->value);
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) - Uint64ToFloat(b->value));
            codeRaw("fsub");
        } else {
            out->value = Uint64ToFloat(a->value) - Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value - b->value;
        isDone = true;
        codeRaw("isub");
    }
    return isDone;
}

bool objectMul(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) * b->value);
        } else {
            out->value = Uint64ToFloat(a->value) * b->value;
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(a->value * Uint64ToFloat(b->value));
        } else {
            out->value = a->value * Uint64ToFloat(b->value);
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) * Uint64ToFloat(b->value));
            codeRaw("fmul");
        } else {
            out->value = Uint64ToFloat(a->value) * Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value * b->value;
        isDone = true;
        codeRaw("imul");
    }
    return isDone;
}

bool objectDiv(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) / b->value);
        } else {
            out->value = Uint64ToFloat(a->value) / b->value;
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(a->value / Uint64ToFloat(b->value));
        } else {
            out->value = a->value / Uint64ToFloat(b->value);
        }
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        if (out->type == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64(Uint64ToFloat(a->value) / Uint64ToFloat(b->value));
            codeRaw("fdiv");
        } else {
            out->value = Uint64ToFloat(a->value) / Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value - b->value;
        isDone = true;
        codeRaw("idiv");
    }
    return isDone;
}

bool objectRem(Object* a, Object* b, Object* out) {
    if (b->value == 0) {
        b->value = 1;
    }
    if (a->value == 0) {
        a->value = 1;
    }
    out->value = a->value % b->value;
    codeRaw("irem");
    return true;
}

bool objectShr(Object* a, Object* b, Object* out) {
    out->value = a->value >> b->value;
    codeRaw("iushr");
    return true;
}

bool objectShl(Object* a, Object* b, Object* out) {
    out->value = a->value << b->value;
    codeRaw("ishl");
    return true;
}

bool objectBoolNot(Object* a, Object* out) {
    out->value = !a->value;
    code("ifne\tBoolNot0_%d", BoolNotLabelNum);
    codeRaw("iconst_1");
    code("goto\tBoolNotExit_%d", BoolNotLabelNum);
    code("BoolNot0_%d:", BoolNotLabelNum);
    codeRaw("iconst_0");
    code("BoolNotExit_%d:", BoolNotLabelNum);
    BoolNotLabelNum++;
    return true;
}

bool objectBoolAnd(Object* a, Object* b, Object* out) {
    out->value = a->value && b->value;
    code("ifeq\tBoolAnd0_%d", BoolAndLabelNum);
    codeRaw("dup");
    code("ifeq\tBoolAnd0_%d", BoolAndLabelNum);
    codeRaw("pop");
    codeRaw("iconst_1");
    code("goto\tBoolAndExit_%d", BoolAndLabelNum);
    code("BoolAnd0_%d:", BoolAndLabelNum);
    codeRaw("pop");
    codeRaw("iconst_0");
    code("BoolAndExit_%d:", BoolAndLabelNum);
    BoolAndLabelNum++;
    return true;
}

bool objectBoolOr(Object* a, Object* b, Object* out) {
    out->value = a->value || b->value;
    code("ifne\tBoolOr1_%d", BoolOrLabelNum);
    codeRaw("dup");
    code("ifeq\tBoolOr0_%d", BoolOrLabelNum);
    code("BoolOr1_%d:", BoolOrLabelNum);
    codeRaw("pop");
    codeRaw("iconst_1");
    code("goto\tBoolOrExit_%d", BoolOrLabelNum);
    code("BoolOr0_%d:", BoolOrLabelNum);
    codeRaw("pop");
    codeRaw("iconst_0");
    code("BoolOrExit_%d:", BoolOrLabelNum);
    BoolOrLabelNum++;
    return true;
}

bool objectBoolEq(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) == b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value == Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) == Uint64ToFloat(b->value);
        isDone = true;
    } else {
        out->value = a->value == b->value;
        isDone = true;
        code("if_icmpne\tBoolEq0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolEqExit_%d", BoolCmpLabelNum);
        code("BoolEq0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolEqExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    }
    return isDone;
}

bool objectBoolNeq(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) != b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value != Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) != Uint64ToFloat(b->value);
        isDone = true;
    } else {
        out->value = a->value != b->value;
        isDone = true;
        code("if_icmpeq\tBoolNeq0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolNeqExit_%d", BoolCmpLabelNum);
        code("BoolNeq0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolNeqExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    }
    return isDone;
}

bool objectGtr(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) > b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value > Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) > Uint64ToFloat(b->value);
        isDone = true;
        codeRaw("fcmpl");
        code("ifle\tBoolGtr0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolGtrExit_%d", BoolCmpLabelNum);
        code("BoolGtr0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolGtrExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    } else {
        out->value = a->value > b->value;
        isDone = true;
        code("if_icmple\tBoolGtr0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolGtrExit_%d", BoolCmpLabelNum);
        code("BoolGtr0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolGtrExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    }
    return isDone;
}

bool objectLes(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) < b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value < Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) < Uint64ToFloat(b->value);
        isDone = true;
        codeRaw("fcmpg");
        code("ifge\tBoolLes0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolLesExit_%d", BoolCmpLabelNum);
        code("BoolLes0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolLesExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    } else {
        out->value = a->value < b->value;
        isDone = true;
        code("if_icmpge\tBoolLes0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolLesExit_%d", BoolCmpLabelNum);
        code("BoolLes0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolLesExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    }
    return isDone;
}

bool objectGeq(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) >= b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value >= Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) >= Uint64ToFloat(b->value);
        isDone = true;
    } else {
        out->value = a->value >= b->value;
        isDone = true;

        // code gen
        code("if_icmplt\tBoolGeq0_%d", BoolCmpLabelNum);
        codeRaw("iconst_1");
        code("goto\tBoolGeqExit_%d", BoolCmpLabelNum);
        code("BoolGeq0_%d:", BoolCmpLabelNum);
        codeRaw("iconst_0");
        code("BoolGeqExit_%d:", BoolCmpLabelNum);
        BoolCmpLabelNum++;
    }
    return isDone;
}

bool objectLeq(Object* a, Object* b, Object* out) {
    bool isDone = false;
    if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_INT) {
        out->value = Uint64ToFloat(a->value) <= b->value;
        isDone = true;
    } else if (a->type == OBJECT_TYPE_INT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = a->value <= Uint64ToFloat(b->value);
        isDone = true;
    } else if (a->type == OBJECT_TYPE_FLOAT && b->type == OBJECT_TYPE_FLOAT) {
        out->value = Uint64ToFloat(a->value) <= Uint64ToFloat(b->value);
        isDone = true;
    } else {
        out->value = a->value <= b->value;
        isDone = true;
    }
    return isDone;
}

bool objectBinNot(Object* dest, Object* out) {
    out->value = ~dest->value;
    codeRaw("iconst_m1");
    codeRaw("ixor");
    return true;
}

bool objectBinOr(Object* a, Object* b, Object* out) {
    out->value = a->value | b->value;
    codeRaw("ior");
    return true;
}

bool objectBinXor(Object* a, Object* b, Object* out) {
    out->value = a->value ^ b->value;
    codeRaw("ixor");
    return true;
}

bool objectBinAnd(Object* a, Object* b, Object* out) {
    out->value = a->value & b->value;
    codeRaw("iand");
    return true;
}

bool objectAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectCast(a->type, b, a);
    out->value = a->value;

    // code gen
    if (a->flag == VAR_FLAG_ARRAY) {
        if (a->type == OBJECT_TYPE_INT) {
            codeRaw("iastore");
        } else {
            fprintf(stderr, "Error: unsupported type for array\n");
        }
    } else if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_STR) {
        code("astore %d", a->symbol->addr);
    }
    return true;
}

bool objectAddAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectAdd(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectSubAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectSub(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectMulAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectMul(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectDivAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectDiv(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectRemAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectRem(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectBanAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinAnd(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectBorAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinOr(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectBxoAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinXor(a, b, out);
    a->value = out->value;

    // code gen
    if (a->type == OBJECT_TYPE_FLOAT) {
        code("fstore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectShrAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShr(a, b, out);
    a->value = out->value;
    if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectShlAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShl(a, b, out);
    a->value = out->value;
    if (a->type == OBJECT_TYPE_INT) {
        code("istore %d", a->symbol->addr);
    } else if (a->type == OBJECT_TYPE_BOOL) {
        code("istore %d", a->symbol->addr);
    }
    return true;
}

bool objectIncAssign(Object* a, Object* out) {
    Object b;
    b.type = OBJECT_TYPE_INT;
    b.value = 1;

    // code gen
    codeRaw("iconst_1");
    return objectAddAssign(a, &b, out);
}

bool objectDecAssign(Object* a, Object* out) {
    Object b;
    b.type = OBJECT_TYPE_INT;
    b.value = 1;

    // code gen
    codeRaw("iconst_1");
    return objectSubAssign(a, &b, out);
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

            // code gen
            codeRaw("i2f");
        } else if (dest->type == OBJECT_TYPE_INT && variableType == OBJECT_TYPE_BOOL) {
            out->value = dest->value != 0;
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_FLOAT && variableType == OBJECT_TYPE_INT) {
            out->value = (int)Uint64ToFloat(dest->value);
            isDone = true;

            // code gen
            codeRaw("f2i");
        } else if (dest->type == OBJECT_TYPE_FLOAT && variableType == OBJECT_TYPE_BOOL) {
            out->value = Uint64ToFloat(dest->value) != 0.0;
            isDone = true;

            // code gen
            codeRaw("f2i");
        } else if (dest->type == OBJECT_TYPE_BOOL && variableType == OBJECT_TYPE_INT) {
            out->value = dest->value;
            isDone = true;
        } else if (dest->type == OBJECT_TYPE_BOOL && variableType == OBJECT_TYPE_FLOAT) {
            out->value = Float2Uint64((float)dest->value);
            isDone = true;

            // code gen
            codeRaw("i2f");
        }
    }

    return isDone;
}