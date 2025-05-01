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

%define parse.error verbose

%union { char *s; unsigned long long int w; struct node *n; }
%token <w>number
%token <s>str
%token <s>name

%token extrn
%token autoo
%token returnn
%token iff
%token elsee

%token <w>'+'
%token <w>'-'
%token <w>'*'
%token <w>'/'
%token <w>'%'

/* precedence */
%left '+' '-'
%left '*' '/' '%'

%type <w>binary

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
    : name '(' defargs ')' statement  { $$ = def(namen($1), $3, $5); }
    ;

defargs
    :                           { $$ = NULL; }
    | name                      { $$ = list(namen($1)); }
    | name ',' defargs          { $$ = listfront($3, namen($1)); }
    ;

statements
    : statement                 { $$ = list($1); }
    | statement statements      { $$ = listfront($2, $1); }
    ;

statement
    : extrn name ';'                { $$ = decl(externn($2), NULL); }    /* TODO: list */
    | autoo name ';'                { $$ = decl(auton($2), NULL); };     /* TODO: list */
    | autoo name constant ';'       { $$ = decl(auton($2), $3); };       /* TODO: list */
    | iff '(' rvalue ')' statement { $$ = ifn($3, $5, NULL); }
    | iff '(' rvalue ')' statement elsee statement { $$ = ifn($3, $5, $7); }
    | '{' statements '}'            { $$ = $2; }
    | returnn ';'                   { $$ = returnnn(NULL); }
    | returnn '(' rvalue ')' ';'    { $$ = returnnn($3); }
    | rvalue ';'                    { $$ = $1; }
    | ';'                           { $$ = empty(); }
    ;

rvalue
    : '(' rvalue ')'            { $$ = $2; };
    | lvalue                    { $$ = $1; }
    | constant                  { $$ = $1; }
    | lvalue '=' rvalue         { $$ = assignn($1, $3); }
    | rvalue binary rvalue      { $$ = binaryn($2, $1, $3); }
    | rvalue '(' callargs ')'   { $$ = call($1, $3); }
    ;

binary
    : '-'   { $$ = $1; };
    | '+'   { $$ = $1; };
    | '*'   { $$ = $1; };
    | '/'   { $$ = $1; };
    | '%'   { $$ = $1; };
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
