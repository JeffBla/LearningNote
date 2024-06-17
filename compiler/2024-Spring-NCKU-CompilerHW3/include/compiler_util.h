#ifndef COMPILER_UTIL_H
#define COMPILER_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int yylineno;
extern int yycolumn;
extern int yyleng;
extern char* yytext;
extern char* yyInputFileName;
extern bool compileError;

#define ERROR_PREFIX "%s:%d:%d: error: "

#define yyerrorf(format, ...)                                                                         \
    {                                                                                                 \
        printf(ERROR_PREFIX format, yyInputFileName, yylineno, yycolumn - yyleng + 1, ##__VA_ARGS__); \
        compileError = true;                                                                          \
        YYABORT;                                                                                      \
    }

void yyerror(char const *msg) {
    printf(ERROR_PREFIX " %s\n", yyInputFileName, yylineno, yycolumn - yyleng + 1, msg);
    yytext[strlen(yytext)] = ' ';
    for(int i = 0; yytext[i]; i++)
        if (yytext[i] == '\r' || yytext[i] == '\n') {
            yytext[i] = 0;
            break;
        }
    printf("%6d |%s\n       |%*.s^\n", yylineno, yytext - yycolumn + yyleng, yycolumn - yyleng, "");
    compileError = true;
}

#endif