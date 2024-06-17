/* Definition section */
%{
    #include "compiler_common.h"
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;

    int arr_init_element = 0;
    Object *pTmpObj = NULL;

    ObjectType curr_type;    
%}

/* Variable or self-defined structure */
%union {
    ObjectType var_type;

    bool b_var;
    char c_var;
    int i_var;
    float f_var;
    char *s_var;

    Object object_val;

    // LinkList<Object*>
    // LinkedList* array_subscript;
}

/* Token without return */
%token COUT
%token SHR SHL BAN BOR BNT BXO ADD SUB MUL DIV REM NOT GTR LES GEQ LEQ EQL NEQ LAN LOR
%token EQL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
%token IF ELSE FOR WHILE RETURN BREAK CONTINUE 

/* Token with return, which need to sepcify type */
%token <var_type> VARIABLE_T
%token <b_var> BOOL_LIT
%token <c_var> CHAR_LIT
%token <i_var> INT_LIT
%token <f_var> FLOAT_LIT
%token <s_var> STR_LIT
%token <s_var> IDENT

/* Nonterminal with return, which need to sepcify type */
%type <object_val> DefineVariable DeclareVariable
%type <object_val> Expression ExprAssign ExprArrAssign ExprLor ExprLan ExprBor ExprBxo ExprBan ExprEqlNeq ExprGtrLesGeqLeq ExprShlShr ExprAddSub ExprMulDivRem ExprNotBntNeg ExprFinal
%type <i_var> ExpressionList ArrayExpressionList ArrayInitExpression

%right NOT BNT 
%left ADD SUB
%left MUL DIV REM
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
    : DefineVariableStmt ';'
    | FunctionDefStmt
;

DefineVariableStmt
    : VARIABLE_T { curr_type = $<var_type>1; } DefineVariableList
;

DefineVariableList
    : DefineVariableList ',' DefineVariable 
    | DefineVariableList ',' DeclareVariable
    | DefineVariable 
    | DeclareVariable
;

DeclareVariable
    : IDENT {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_DEFAULT); }
    | '*' IDENT {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER); }
    | IDENT '[' Expression ']' {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_ARRAY);  if(!arrayCreate(&$$)) YYABORT; }
    | IDENT '[' Expression ']' '[' Expression ']' {$$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_ARRAY); }
    | '*' IDENT '[' ']' {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    | '*' IDENT '[' Expression ']' {$$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    ;

DefineVariable
    : IDENT EQL_ASSIGN Expression { $$ = *createVariable(curr_type, $<s_var>1, VAR_FLAG_DEFAULT); if(!defineVariable(findVariable_mainTable($<s_var>1), &$<object_val>3)) YYABORT; }
    | '*' IDENT EQL_ASSIGN Expression { $$ = *createVariable(curr_type, $<s_var>2, VAR_FLAG_POINTER); if(!defineVariable( findVariable_mainTable($<s_var>1), &$<object_val>3)) YYABORT; }
    | IDENT '[' Expression ']' EQL_ASSIGN '{' { Object *pObj = createVariable(curr_type, $<s_var>1, VAR_FLAG_ARRAY); if(!arrayCreate(pObj)) YYABORT; if(!identExp($1, findVariable_mainTable($1))) YYABORT; } ArrayExpressionList '}' { printf("create array: %d\n", $<i_var>8); }
    // | IDENT '[' ']' EQL_ASSIGN '{' ExpressionList '}' ';' { $$ = *createVariable($<var_type>1, $<s_var>2, VAR_FLAG_ARRAY); }
    // | '*' IDENT '[' ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>3, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
    // | '*' IDENT '[' Expression ']' EQL_ASSIGN '{' ExpressionList '}' ';' {$$ = *createVariable($<var_type>1, $<s_var>3, VAR_FLAG_POINTER | VAR_FLAG_ARRAY); }
;

ArrayExpressionList
    : ArrayInitExpression ',' ArrayExpressionList { arr_init_element = 0; $$ = 1 + $3;}
    | ArrayInitExpression
    ;
ArrayInitExpression
    : { codeRaw("dup"); code("ldc %d", arr_init_element); } Expression { arr_init_element += 1; codeRaw("iastore"); /* there are only int type. */ $$ = 1; } 
    ;

/* Function */
FunctionDefStmt
    :  VARIABLE_T IDENT { createFunction($<var_type>1, $<s_var>2); pushScope(); }'(' FunctionParameterStmtList ')' { clearMainFunParm($<s_var>2); } '{' StmtList '}' { dumpScope(); if(!functionEnd($<var_type>1)) YYABORT; }
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
    | CompoundStmt
    | DefineVariableStmt ';'
    | Expression ';'
    | COUT CoutParmListStmt ';' { stdoutPrint(); }
    | SelectionStmt
    | WhileStmt
    | ForStmt
    | BREAK ';' { if(!breakLoop()) YYABORT; }
    | RETURN Expression ';' { /* if(!returnObject(&$<object_val>2)) YYABORT; */ printf("RETURN\n"); }
    | RETURN ';' { /* codeRaw("return"); */ printf("RETURN\n"); }
;

CompoundStmt
    : '{' { pushScope(); } BlockItemList '}'{ dumpScope(); }
    ;

BlockItemList
    : BlockItemList BlockItem
    | BlockItem
    ;

BlockItem
    : Stmt
    ;

SelectionStmt
    : IfStmt { if(!ifOnlyEnd()) YYABORT; }
    | IfStmt ElseStmt 
    ;

IfStmt
    : IF '(' Expression ')' { if(!ifBegin(&$<object_val>3)) YYABORT; } CompoundStmt { if(!ifEnd()) YYABORT; }
    | IF '(' Expression ')' { if(!ifBegin(&$<object_val>3)) YYABORT; } Stmt { if(!ifEnd()) YYABORT; }
    ;

ElseStmt
    : ELSE { if(!elseBegin()) YYABORT; } CompoundStmt { if(!elseEnd()) YYABORT; }
    // | ELSE { printf("ELSE\n"); pushScope(); } Stmt
    ;

WhileStmt
    : WHILE { if(!whileBegin()) YYABORT; } '(' Expression ')' { if(!whileBodyBegin()) YYABORT; } CompoundStmt { if(!whileEnd()) YYABORT; }
    ;

ForEvalEleStmt
    : Expression 
    | /* Empty */
    ;

ForEvalStmt
    : VARIABLE_T IDENT EQL_ASSIGN Expression { createVariable($<var_type>1, $<s_var>2, VAR_FLAG_DEFAULT); if(!defineVariable(findVariable_mainTable($<s_var>2), &$<object_val>4)) YYABORT; if(!forInitEnd()) YYABORT; } ';' ForEvalEleStmt { if(!forConditionEnd()) YYABORT; } ';' ForEvalEleStmt { if(!forHeaderEnd()) YYABORT; }
    | { if(!forInitEnd()) YYABORT; } ';' ForEvalEleStmt { if(!forConditionEnd()) YYABORT; } ';' ForEvalEleStmt { if(!forHeaderEnd()) YYABORT; }
    | VARIABLE_T IDENT ':' IDENT { createVariable($<var_type>1, $<s_var>2, VAR_FLAG_DEFAULT); if(!defineVariable(findVariable_mainTable($<s_var>2), findVariable_mainTable($<s_var>4))) YYABORT; PrintIdent($<s_var>4); }
    ; 

ForStmt
    : FOR { if(!forBegin()) YYABORT; pushScope(); } '(' ForEvalStmt ')' '{' StmtList '}' { if(!forEnd()) YYABORT; dumpScope(); } 
    ;

CoutParmListStmt
    : SHL { codeRaw("getstatic java/lang/System/out Ljava/io/PrintStream;"); } ExprAddSub { /* printf("ExprAddSub: %d\n", $<object_val>2.value); */ pushFunInParm(&$<object_val>3); } CoutParmListStmt
    | /* Empty */
;

ExpressionList
    : ExpressionList ',' Expression { $$ = $1 + 1; }
    | Expression { $$ = 1; }
    | /* Empty */ { $$ = 0; }
    ;

/// Expression
Expression
    : ExprAssign { $$ = $1; }
    | ExprLor { $$ = $1; }
    | ExprArrAssign { $$ = $1; }
;

/// Right associative
ExprAssign
    : IDENT EQL_ASSIGN Expression { if(!objectExpAssign("=", findVariable_mainTable($<s_var>1), &$<object_val>3, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } ADD_ASSIGN Expression { if(!objectExpAssign("+=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } SUB_ASSIGN Expression { if(!objectExpAssign("-=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT;}
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } MUL_ASSIGN Expression { if(!objectExpAssign("*=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } DIV_ASSIGN Expression { if(!objectExpAssign("/=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } REM_ASSIGN Expression { if(!objectExpAssign("%=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } BAN_ASSIGN Expression { if(!objectExpAssign("&=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } BOR_ASSIGN Expression { if(!objectExpAssign("|=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } BXO_ASSIGN Expression { if(!objectExpAssign("^=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } SHR_ASSIGN Expression { if(!objectExpAssign(">>=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
    | IDENT { if(!objectIdentLoad($1)) YYABORT; } SHL_ASSIGN Expression { if(!objectExpAssign("<<=", findVariable_mainTable($<s_var>1), &$<object_val>4, &$$)) YYABORT; }
;

ExprArrAssign
    : ArrayIdent '[' Expression ']' EQL_ASSIGN Expression { $$ = *pTmpObj; if(!objectExpAssign("=", &$$, &$<object_val>6, &$$)) YYABORT; }
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
    | ExprMulDivRem {$$ = $1 ; }

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
    | '(' VARIABLE_T ')' ExprNotBntNeg { if(!objectExpCast($<var_type>2, &$4, &$$)) YYABORT; }
    | INC_ASSIGN ExprFinal { if(!objectExpAssign("++", &$<object_val>2, &$<object_val>2, &$$)) YYABORT; }
    | DEC_ASSIGN ExprFinal { if(!objectExpAssign("--", &$<object_val>2, &$<object_val>2, &$$)) YYABORT; }
    | ExprFinal {$$ = $1; }
;

ExprFinal
    : '(' Expression ')' { $$ = $2; }
    | ArrayIdent '[' Expression ']' { $$ = *pTmpObj; PrintIdent($<s_var>1); if(!objectArrayGet(&$$)) YYABORT; }
    | ArrayIdent '[' Expression ']' '[' Expression ']' { $$ = *findVariable_mainTable($<s_var>1); PrintIdent($<s_var>1); }
    | IDENT '(' ExpressionList ')' { $$ = *callFunction($<s_var>1); }
    | ExprFinal INC_ASSIGN { if(!objectExpAssign("++", &$<object_val>1, &$<object_val>1, &$$)) YYABORT; }
    | ExprFinal DEC_ASSIGN { if(!objectExpAssign("--", &$<object_val>1, &$<object_val>1, &$$)) YYABORT; }
    | IDENT { if(!identExp($<s_var>1, &$$)) YYABORT; }
    | FLOAT_LIT { $$.value = Float2Uint64($1); $$.type = OBJECT_TYPE_FLOAT; printf("FLOAT_LIT %f\n", $<f_var>1); ldf(&$$); } 
    | INT_LIT { $$.value = $1; $$.type = OBJECT_TYPE_INT; printf("INT_LIT %d\n", $<i_var>1); ldi(&$$); }  
    | BOOL_LIT { $$.value = $1; $$.type = OBJECT_TYPE_BOOL; printf("BOOL_LIT %s\n", $<b_var>1 == 0 ? "FALSE" : "TRUE"); ldz(&$$); } 
    | STR_LIT { $$.value = StringLiteral2Uint64($1); $$.type = OBJECT_TYPE_STR; printf("STR_LIT \"%s\"\n", $<s_var>1); ldt(&$$); }
    | CHAR_LIT { $$.value = $1; $$.type = OBJECT_TYPE_CHAR; printf("CHAR_LIT %c", $<c_var>1); ldc(&$$); }
;

ArrayIdent
    : IDENT { pTmpObj = findVariable_mainTable($<s_var>1); if(!identExp($<s_var>1, pTmpObj)) YYABORT; }
;

%%
/* C code section */