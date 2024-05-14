/* Definition section */
%{
    #include "compiler_common.h"
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;

    ObjectType curr_type;    
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
%type <object_val> DefineVariable
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
    : VARIABLE_T { curr_type = $<var_type>1; } DefineVariableList ';'
;

DefineVariableList
    : DefineVariableList ',' DefineVariable 
    | DefineVariable
;

DefineVariable
    : IDENT EQL_ASSIGN Expression {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_DEFAULT); if(!objectExpAssign("=", findVariable($<s_var>1), &$<object_val>3, &$$)) YYABORT; }
    | IDENT {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_DEFAULT); }
    | '*' IDENT EQL_ASSIGN Expression {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER); if(!objectExpAssign("=", findVariable($<s_var>1), &$<object_val>3, &$$)) YYABORT; }
    | '*' IDENT {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER); }
    | IDENT '[' INT_LIT ']' {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_ARRAY); }
    | IDENT '[' ']' {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_ARRAY); }
    | '*' IDENT '[' ']' {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    | '*' IDENT '[' INT_LIT ']' {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    // | IDENT '[' ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>2, VAR_FLAG_ARRAY); }
    // | '*' IDENT '[' ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>3, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    // | IDENT '[' INT_LIT ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>2, VAR_FLAG_ARRAY); }
    // | '*' IDENT '[' INT_LIT ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>3, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
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
    : SHL ExprAddSub {  /* printf("ExprAddSub: %d\n", $<object_val>2.value); */ pushFunInParm(&$<object_val>2); } CoutParmListStmt
    | SHL ExprAddSub { /* printf("ExprAddSub: %d\n", $<object_val>2.value); */ pushFunInParm(&$<object_val>2); }
;

/// Expression
Expression
    : ExprAssign { $$ = $1;}
;

/// Right associative
ExprAssign
    : ExprLor EQL_ASSIGN ExprAssign { if(!objectExpAssign("=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor ADD_ASSIGN ExprAssign { if(!objectExpAssign("+=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor SUB_ASSIGN ExprAssign { if(!objectExpAssign("-=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT;}
    | ExprLor MUL_ASSIGN ExprAssign { if(!objectExpAssign("*=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor DIV_ASSIGN ExprAssign { if(!objectExpAssign("/=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor REM_ASSIGN ExprAssign { if(!objectExpAssign("%=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor BAN_ASSIGN ExprAssign { if(!objectExpAssign("&=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor BOR_ASSIGN ExprAssign { if(!objectExpAssign("|=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor BXO_ASSIGN ExprAssign { if(!objectExpAssign("^=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor SHR_ASSIGN ExprAssign { if(!objectExpAssign(">>=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprLor SHL_ASSIGN ExprAssign { if(!objectExpAssign("<<=", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
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
    : ExprBor BOR ExprBxo { if(!objectExpBinary("|", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprBxo { $$ = $1;}
;

ExprBxo
    : ExprBxo BXO ExprBan { if(!objectExpBinary("^", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
    | ExprBan { $$ = $1;}
;

ExprBan
    : ExprBan BAN ExprEqlNeq { if(!objectExpBinary("&", &$<object_val>1, &$<object_val>3, &$$)) YYABORT; }
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
    | BNT ExprNotBntNeg { if(!objectExpBinary("~", &$<object_val>2, &$<object_val>2, &$$)) YYABORT; }
    | SUB ExprNotBntNeg { if(!objectExpNeg(&$<object_val>2, &$$)) YYABORT; }
    | '(' VARIABLE_T ')' ExprNotBntNeg { if(!objectCast($<var_type>2, &$4, &$$)) YYABORT; }
    | ExprFinal {$$ = $1;}
;

ExprFinal
    : '(' Expression ')' { $$ = $2;}
    | '[' Expression ']' { $$ = $2;}
    | IDENT { if(strcmp($<s_var>1, "endl") != 0) $$ = *findVariable($<s_var>1); $$.type = PrintIdent($<s_var>1); }
    | FLOAT_LIT { $$.value = Float2Uint64($1); $$.type = OBJECT_TYPE_FLOAT; printf("FLOAT_LIT %f\n", $<f_var>1); } 
    | INT_LIT { $$.value = $1; $$.type = OBJECT_TYPE_INT; printf("INT_LIT %d\n", $<i_var>1); }  
    | BOOL_LIT { $$.value = $1; $$.type = OBJECT_TYPE_BOOL; printf("BOOL_LIT %s\n", $<b_var>1 == 0 ? "FALSE" : "TRUE"); } 
    | STR_LIT { $$.value = StringLiteral2Uint64($1); $$.type = OBJECT_TYPE_STR; printf("STR_LIT \"%s\"\n", $<s_var>1); }
;

%%
/* C code section */