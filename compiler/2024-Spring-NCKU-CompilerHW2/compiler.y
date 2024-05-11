/* Definition section */
%{
    #include "compiler_common.h"
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;
%}

/* Variable or self-defined structure */
%union {
    ObjectType var_type;

    bool b_var;
    int i_var;
    float f_var;
    char *s_var;

    Object object_val;
}

/* Token without return */
%token COUT
%token SHR SHL BAN BOR BNT BXO ADD SUB MUL DIV REM NOT GTR LES GEQ LEQ EQL NEQ LAN LOR
%token EQL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
%token IF ELSE FOR WHILE RETURN BREAK CONTINUE 

/* Token with return, which need to sepcify type */
%token <var_type> VARIABLE_T
%token <b_var> BOOL_LIT
%token <i_var> INT_LIT
%token <f_var> FLOAT_LIT
%token <s_var> STR_LIT
%token <s_var> IDENT

/* Nonterminal with return, which need to sepcify type */
%type <object_val> DefineVariableStmt
%type <object_val> Expression ExprAssign ExprLor ExprLan ExprBor ExprBxo ExprBan ExprEqlNeq ExprGtrLesGeqLeq ExprShlShr ExprAddSub ExprMulDivRem ExprNotBntNeg ExprFinal

%right NOT BNT
%left MUL DIV REM
%left ADD SUB
%left SHL SHR
%left GTR LES GEQ LEQ 
%left EQL NEQ
%left BAN
%left BXO
%left BOR
%left LAN
%left LOR
%right EQL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN


/* Yacc will start at this nonterminal */
%start Program

%%
/* Grammar section */

Program
    : { pushScope(); } GlobalStmtList { dumpScope(); }
    | /* Empty file */
;

GlobalStmtList 
    : GlobalStmtList GlobalStmt
    | GlobalStmt
;

GlobalStmt
    : DefineVariableStmt
    | FunctionDefStmt
;

DefineVariableStmt
    : VARIABLE_T IDENT EQL_ASSIGN Expression ';' {$$ = *createVariable($<var_type>1, $<s_var>2, VAR_FLAG_DEFAULT); }
    | VARIABLE_T IDENT ';' {$$ = *createVariable($<var_type>1, $<s_var>2, VAR_FLAG_DEFAULT); }
;

/* Function */
FunctionDefStmt
    :  VARIABLE_T IDENT { createFunction($<var_type>1, $<s_var>2); pushScope(); }'(' FunctionParameterStmtList ')' '{' StmtList '}' { dumpScope(); }
;
FunctionParameterStmtList 
    : FunctionParameterStmtList ',' FunctionParameterStmt
    | FunctionParameterStmt
    | /* Empty function parameter */
;
FunctionParameterStmt
    : VARIABLE_T IDENT { pushFunParm($<var_type>1, $<s_var>2, VAR_FLAG_DEFAULT); }
    | VARIABLE_T '*' IDENT { pushFunParm($<var_type>1, $<s_var>3, VAR_FLAG_POINTER); }
    | VARIABLE_T IDENT '['  ']' { pushFunParm($<var_type>1, $<s_var>2, VAR_FLAG_ARRAY); }
;

/* Scope */
StmtList 
    : StmtList Stmt
    | Stmt
;
Stmt
    : ';'
    | DefineVariableStmt
    | Expression ';'
    | COUT CoutParmListStmt ';' { stdoutPrint(); }
    | RETURN Expression ';' { printf("RETURN\n"); }
;

CoutParmListStmt
    : SHL ExprAddSub { /* printf("ExprAddSub: %d\n", $<object_val>2.value); */ pushFunInParm(&$<object_val>2); } CoutParmListStmt
    | SHL ExprAddSub { pushFunInParm(&$<object_val>2); }
;

/// Expression
Expression
    : ExprAssign { $$ = $1;}
;

/// Right associative
ExprAssign
    : ExprLor EQL_ASSIGN ExprAssign {printf("%s", "EQL_ASSIGN\n"); $$.value = $1.value = $3.value;}
    | ExprLor ADD_ASSIGN ExprAssign {printf("%s", "ADD_ASSIGN\n"); $$.value  = $1.value += $3.value;}
    | ExprLor SUB_ASSIGN ExprAssign {printf("%s", "SUB_ASSIGN\n"); $$.value  = $1.value -= $3.value;}
    | ExprLor MUL_ASSIGN ExprAssign {printf("%s", "MUL_ASSIGN\n"); $$.value  = $1.value *= $3.value;}
    | ExprLor DIV_ASSIGN ExprAssign {printf("%s", "DIV_ASSIGN\n"); $$.value  = $1.value  /= $3.value ;}
    | ExprLor REM_ASSIGN ExprAssign {printf("%s", "REM_ASSIGN\n"); $$.value  = $1.value  %= $3.value ;}
    | ExprLor BAN_ASSIGN ExprAssign {printf("%s", "BAN_ASSIGN\n"); $$.value  = $1.value  &= $3.value ;}
    | ExprLor BOR_ASSIGN ExprAssign {printf("%s", "BOR_ASSIGN\n"); $$.value  = $1.value  |= $3.value ;}
    | ExprLor BXO_ASSIGN ExprAssign {printf("%s", "BXO_ASSIGN\n"); $$.value  = $1.value  ^= $3.value ;}
    | ExprLor SHR_ASSIGN ExprAssign {printf("%s", "SHR_ASSIGN\n"); $$.value  = $1.value  >>= $3.value ;}
    | ExprLor SHL_ASSIGN ExprAssign {printf("%s", "SHL_ASSIGN\n"); $$.value  = $1.value  <<= $3.value ;}
    | ExprLor { $$ = $1;}
;

ExprLor
    : ExprLor LOR ExprLan { if(!objectExpBoolean("||", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLan { $$ = $1;}
;

ExprLan
    : ExprLan LAN ExprBor { if(!objectExpBoolean("&&", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprBor { $$ = $1;}
;

ExprBor
    : ExprBor BOR ExprBxo {$$.value  = $1.value | $3.value ; printf("BOR\n");}
    | ExprBxo { $$ = $1;}
;

ExprBxo
    : ExprBxo BXO ExprBan {$$.value  = $1.value ^ $3.value ; printf("BXO\n");}
    | ExprBan { $$ = $1;}
;

ExprBan
    : ExprBan BAN ExprEqlNeq {$$.value  = $1.value & $3.value ; printf("BAN\n");}
    | ExprEqlNeq { $$ = $1;}
;

ExprEqlNeq
    : ExprEqlNeq EQL ExprGtrLesGeqLeq { if(!objectExpBoolean("==", &$<object_val>1, &$<object_val>3, &$$ )) YYABORT; }
    | ExprEqlNeq NEQ ExprGtrLesGeqLeq { if(!objectExpBoolean("!=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprGtrLesGeqLeq {$$ = $1;}
;

ExprGtrLesGeqLeq
    : ExprGtrLesGeqLeq GTR ExprShlShr { if(!objectExpBoolean(">", &$<object_val>1, &$<object_val>3, &$$ )) YYABORT; }
    | ExprGtrLesGeqLeq LES ExprShlShr { if(!objectExpBoolean("<", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprGtrLesGeqLeq GEQ ExprShlShr { if(!objectExpBoolean(">=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprGtrLesGeqLeq LEQ ExprShlShr { if(!objectExpBoolean("<=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprShlShr {$$= $1;}
;

ExprShlShr
    : ExprShlShr SHL ExprAddSub { if(!objectExpression("<<", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprShlShr SHR ExprAddSub { if(!objectExpression(">>", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprAddSub {$$ = $1;}
;

ExprAddSub
    : ExprAddSub ADD ExprMulDivRem { if(!objectExpression("+", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprAddSub SUB ExprMulDivRem  { if(!objectExpression("-", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprMulDivRem {$$ = $1 ;}

ExprMulDivRem
    : ExprMulDivRem MUL ExprNotBntNeg { if(!objectExpression("*", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprMulDivRem DIV ExprNotBntNeg { if(!objectExpression("/", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprMulDivRem REM ExprNotBntNeg { if(!objectExpression("%", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprNotBntNeg {$$ = $1;}
;

/// Right associative
ExprNotBntNeg
    : NOT ExprNotBntNeg { if(!objectExpBoolean("!", &$<object_val>2, &$<object_val>2, &$$)) YYABORT; }
    | BNT ExprNotBntNeg { if(!objectExpBinaryNot(&$<object_val>2, &$$)) YYABORT; }
    | SUB ExprNotBntNeg { if(!objectNegExpression(&$<object_val>2, &$$)) YYABORT; }
    | ExprFinal {$$= $1;}
;

ExprFinal
    : '(' Expression ')' { $$ = $2;}
    | '[' Expression ']' { $$ = $2;}
    | IDENT { $$.type = PrintIdent($<s_var>1);} | FLOAT_LIT {$$.value = $1; $$.type = OBJECT_TYPE_FLOAT; printf("FLOAT_LIT %f\n", $<f_var>1);} | INT_LIT {$$.value = $1; $$.type = OBJECT_TYPE_INT; printf("INT_LIT %d\n", $<i_var>1);}  | BOOL_LIT {$$.value = $1; $$.type = OBJECT_TYPE_BOOL; printf("BOOL_LIT %s\n", $<b_var>1 == 0 ? "FALSE" : "TRUE");} | STR_LIT {$$.type = OBJECT_TYPE_STR; printf("STR_LIT \"%s\"\n", $<s_var>1);}
;

%%
/* C code section */