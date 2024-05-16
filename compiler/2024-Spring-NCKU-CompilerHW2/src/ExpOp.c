#include "ExpOp.h"

bool objectNeg(Object* dest, Object* out) {
    out->type = dest->type;
    if (dest->type == OBJECT_TYPE_FLOAT) {
        out->value = Float2Uint64(-Uint64ToFloat(dest->value));
    } else {
        out->value = -dest->value;
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
        } else {
            out->value = Uint64ToFloat(a->value) + Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value + b->value;
        isDone = true;
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
        } else {
            out->value = Uint64ToFloat(a->value) - Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value - b->value;
        isDone = true;
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
        } else {
            out->value = Uint64ToFloat(a->value) * Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value * b->value;
        isDone = true;
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
        } else {
            out->value = Uint64ToFloat(a->value) / Uint64ToFloat(b->value);
        }
        isDone = true;
    } else {
        out->value = a->value - b->value;
        isDone = true;
    }
    return isDone;
}

bool objectRem(Object* a, Object* b, Object* out) {
    out->value = a->value % b->value;
    return true;
}

bool objectShr(Object* a, Object* b, Object* out) {
    out->value = a->value >> b->value;
    return true;
}

bool objectShl(Object* a, Object* b, Object* out) {
    out->value = a->value << b->value;
    return true;
}

bool objectBoolNot(Object* a, Object* out) {
    out->value = !a->value;
    return true;
}

bool objectBoolAnd(Object* a, Object* b, Object* out) {
    out->value = a->value && b->value;
    return true;
}

bool objectBoolOr(Object* a, Object* b, Object* out) {
    out->value = a->value || b->value;
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
    } else {
        out->value = a->value > b->value;
        isDone = true;
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
    } else {
        out->value = a->value < b->value;
        isDone = true;
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
    return true;
}

bool objectBinOr(Object* a, Object* b, Object* out) {
    out->value = a->value | b->value;
    return true;
}

bool objectBinXor(Object* a, Object* b, Object* out) {
    out->value = a->value ^ b->value;
    return true;
}

bool objectBinAnd(Object* a, Object* b, Object* out) {
    out->value = a->value & b->value;
    return true;
}

bool objectAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectCast(a->type, b, a);
    out->value = a->value;
    return true;
}

bool objectAddAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectAdd(a, b, out);
    a->value = out->value;
    return true;
}

bool objectSubAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectSub(a, b, out);
    a->value = out->value;
    return true;
}

bool objectMulAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectMul(a, b, out);
    a->value = out->value;
    return true;
}

bool objectDivAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectDiv(a, b, out);
    a->value = out->value;
    return true;
}

bool objectRemAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectRem(a, b, out);
    a->value = out->value;
    return true;
}

bool objectBanAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinAnd(a, b, out);
    a->value = out->value;
    return true;
}

bool objectBorAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinOr(a, b, out);
    a->value = out->value;
    return true;
}

bool objectBxoAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectBinXor(a, b, out);
    a->value = out->value;
    return true;
}

bool objectShrAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShr(a, b, out);
    a->value = out->value;
    return true;
}

bool objectShlAssign(Object* a, Object* b, Object* out) {
    bool isDone = objectShl(a, b, out);
    a->value = out->value;
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

    return isDone;
}