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

%union { char *s; int i; struct node *n; }
%token <i>number
%token <s>str
%token <s>name

%token extrn

%type <n>constant <n>lvalue <n>call_args <n>rvalue
%type <n>args <n>statement <n>statements <n>definition <n>definitions
%start program

%%

program
    : definitions   { codegen($1); }
    ;

definitions
    : /* 0 or more */           { $$ = NULL; }
    | definition definitions    { $$ = listfront($2, $1); }
    ;

definition
    : name '(' args ')' statements  { $$ = def(namen($1), $3, $5); }
    ;

args
    : /* 0 or more */       { $$ = NULL; }
    | name                  { $$ = list(namen($1)); }
    | name ',' args         { $$ = listfront($3, namen($1)); }
    ;

statements
    : statement             { $$ = list($1); };
    | statement statements  { $$ = listfront($2, $1); };
    ;

statement
    : extrn name ';'     { $$ = empty(); } /* TODO: list */ /* NOTE: externs are handled by as*/
    | '{' statements '}' { $$ = $2; }
    | rvalue ';'         { $$ = $1; }
    ;

rvalue
    : lvalue                    { $$ = $1; }
    | constant                  { $$ = $1; }
    | rvalue '(' call_args ')'  { $$ = call($1, $3); }
    ;

call_args
    : /* 0 or more */           { $$ = NULL; }
    | rvalue                    { $$ = list($1); }
    | rvalue ',' call_args      { $$ = listfront($3, $1); }
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
