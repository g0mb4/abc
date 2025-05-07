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
%token whilee

%token <w>'+'
%token <w>'-'
%token <w>'*'
%token <w>'/'
%token <w>'%'
%token <w>'<'
%token <w>'>'
%token <w>lessequal
%token <w>greaterequal
%token <w>equal
%token <w>notequal

%token <w>inc
%token <w>dec

/* precedence */
%left equal notequal
%left '<' '>' lessequal greaterequal
%left '+' '-'
%left '*' '/' '%'

%type <w>binary <w>inc-dec

%type <n>extrnlist <n>autolist
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
    : extrn extrnlist ';'           { $$ = $2; }
    | autoo autolist ';'            { $$ = $2; };
    | iff '(' rvalue ')' statement  { $$ = ifn($3, $5, NULL); }
    | iff '(' rvalue ')' statement elsee statement { $$ = ifn($3, $5, $7); }
    | whilee '(' rvalue ')' statement { $$ = whilen($3, $5); }
    | '{' statements '}'            { $$ = $2; }
    | returnn ';'                   { $$ = returnnn(NULL); }
    | returnn '(' rvalue ')' ';'    { $$ = returnnn($3); }
    | rvalue ';'                    { $$ = $1; }
    | ';'                           { $$ = empty(); }
    ;

extrnlist
    : name                      { $$ = list(decl(externn($1), NULL));          }
    | name ',' extrnlist        { $$ = listfront($3, decl(externn($1), NULL)); }
    ;

autolist
    : name                          { $$ = list(decl(auton($1), NULL));  }
    | name constant                 { $$ = list(decl(auton($1), $2));    } /* TODO: vector */
    | name ',' autolist             { $$ = listfront($3, decl(auton($1), NULL)); }
    | name constant ',' autolist    { $$ = listfront($4, decl(auton($1), $2)); }
    ;

rvalue
    : '(' rvalue ')'            { $$ = $2; };
    | lvalue                    { $$ = $1; }
    | constant                  { $$ = $1; }
    | lvalue '=' rvalue         { $$ = assignn($1, $3); }
    | inc-dec lvalue            { $$ = unarynn($1, $2, 1); }
    | lvalue inc-dec            { $$ = unarynn($2, $1, 0); }
    | rvalue binary rvalue      { $$ = binaryn($2, $1, $3); }
    | rvalue '(' callargs ')'   { $$ = call($1, $3); }
    ;

inc-dec
    : inc   { $$ = $1; }
    | dec   { $$ = $1; }
    ;

binary
    : '-'   { $$ = $1; }
    | '+'   { $$ = $1; }
    | '*'   { $$ = $1; }
    | '/'   { $$ = $1; }
    | '%'   { $$ = $1; }
    | '<'   { $$ = $1; }
    | '>'   { $$ = $1; }
    | lessequal         { $$ = $1; }
    | greaterequal      { $$ = $1; }
    | equal             { $$ = $1; }
    | notequal          { $$ = $1; }
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
