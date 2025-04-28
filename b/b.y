%{

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ast.h"
#include "codegen.h"

extern int yylex(void);                 /* yyparse() needs it */
extern int yyparse(void);               /* because we define a custom "main()" */
void yyerror(const char * s, ...);      /* custom error report */

extern const char *infile;              /* from main.c */
extern int yylineno;                    /* global variable for error riport */

%}

%error-verbose

%union { char *s; unsigned long long int w; struct node *n; }
%token <w>number
%token <s>str
%token <s>name

%token extrn

%type <n>constant <n>lvalue <n>callargs <n>rvalue
%type <n>defargs <n>statement <n>statements <n>definition <n>definitions
%start program

%%

program
    : definitions   { codegen($1); }
    ;

definitions
    :                           { $$ = NULL; }
    | definition definitions    { $$ = listfront($2, $1); }
    ;

definition
    : name '(' defargs ')' statements  { $$ = def(namen($1), $3, $5); }
    ;

defargs
    :                           { $$ = NULL; }
    | name                      { $$ = list(namen($1)); }
    | name ',' defargs          { $$ = listfront($3, namen($1)); }
    ;

statements
    : statement             { $$ = list($1); };
    | statement statements  { $$ = listfront($2, $1); };
    ;

statement
    : extrn name ';'     { $$ = decl(externn($2)); }    /* TODO: list */
    | '{' statements '}' { $$ = $2; }
    | rvalue ';'         { $$ = $1; }
    ;

rvalue
    : lvalue                    { $$ = $1; }
    | constant                  { $$ = $1; }
    | rvalue '(' callargs ')'   { $$ = call($1, $3); }
    ;

callargs
    :                          { $$ = NULL; }
    | rvalue                   { $$ = list($1); }
    | rvalue ',' callargs      { $$ = listfront($3, $1); }
    ;

lvalue
    : name                  { $$ = namen($1); }
    ;

constant
    : number                { $$ = integer($1); }
    | str                   { $$ = string($1); }
    ;

%%

void yyerror(const char * fmt, ...) {
    va_list args;

    fprintf(stderr, "%s: %d: ", infile, yylineno);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}
