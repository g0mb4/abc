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
%token <w>INT
%token <s>STR
%token <s>NAME

%token EXTRN
%token AUTO
%token RETURN
%token IF
%token ELSE
%token WHILE

%token <w>'+'
%token <w>'-'
%token <w>'*'
%token <w>'/'
%token <w>'%'
%token <w>'&'
%token <w>'|'
%token <w>'!'
%token <w>'<'
%token <w>'>'
%token <w>'='
%token <w>LESSEQU
%token <w>GREATEQU
%token <w>EQU
%token <w>NOTEQU
%token <w>SHR
%token <w>SHL

%token <w>ASEQU
%token <w>ASOREQU
%token <w>ASLESSEQU
%token <w>ASGREATEQU
%token <w>ASSHL
%token <w>ASSHR
%token <w>ASOR
%token <w>ASAND
%token <w>ASLESS
%token <w>ASGREAT
%token <w>ASPLUS
%token <w>ASMINUS
%token <w>ASMOD
%token <w>ASMUL
%token <w>ASDIV

%token <w>INC
%token <w>DEC

/* precedence - lowest is in top */
%right '=' ASEQU ASOREQU ASLESSEQU ASGREATEQU ASSHL ASSHR ASOR ASAND ASLESS ASGREAT ASPLUS ASMINUS ASMOD ASMUL ASDIV
%left '|'
%left '&'
%left EQU NOTEQU
%left '<' '>' LESSEQU GREATEQU
%left SHL SHR
%left '+' '-'
%left '*' '/' '%'

%type <w>BINARY <w>INC-DEC <w>UNARY <w>ASSIGN
%type <n>EXTRNLIST <n>AUTOLIST
%type <n>CONSTANT <n>LVALUE <n>CALLIST <n>RVALUE
%type <n>DEFLIST <n>STATEMENT <n>STATEMENTS <n>DEFINITION <n>DEFINITIONS
%start PROGRAM

%%

PROGRAM
    : DEFINITIONS   { codegen($1); }
    ;

DEFINITIONS
    :                           { $$ = NULL; }
    | DEFINITION DEFINITIONS    { $$ = listfront($2, $1); }
    ;

DEFINITION
    : NAME '(' DEFLIST ')' STATEMENT  { $$ = mkfundef(mkname($1), $3, $5); }
    | NAME ';'                        { $$ = mkvardef(mkname($1), NULL); }
    | NAME CONSTANT ';'               { $$ = mkvardef(mkname($1), $2); }
    | NAME '[' CONSTANT ']' ';'       { $$ = mkvecdef(mkname($1), $3); } /* TODO: initlist forr vectors*/
    ;

DEFLIST
    :                           { $$ = NULL; }
    | NAME                      { $$ = mklist(mkname($1)); }
    | NAME ',' DEFLIST          { $$ = listfront($3, mkname($1)); }
    ;

STATEMENTS
    : STATEMENT                 { $$ = mklist($1); }
    | STATEMENT STATEMENTS      { $$ = listfront($2, $1); }
    ;

STATEMENT
    : EXTRN EXTRNLIST ';'                           { $$ = $2; }
    | AUTO AUTOLIST ';'                             { $$ = $2; };
    | IF '(' RVALUE ')' STATEMENT                   { $$ = mkif($3, $5, NULL); }
    | IF '(' RVALUE ')' STATEMENT ELSE STATEMENT    { $$ = mkif($3, $5, $7); }
    | WHILE '(' RVALUE ')' STATEMENT                { $$ = mkwhile($3, $5); }
    | '{' STATEMENTS '}'                            { $$ = $2; }
    | RETURN ';'                                    { $$ = mkreturn(NULL); }
    | RETURN '(' RVALUE ')' ';'                     { $$ = mkreturn($3); }
    | RVALUE ';'                                    { $$ = $1; }
    | ';'                                           { $$ = mkempty(); }
    ;

EXTRNLIST
    : NAME                      { $$ = mklist(mkdecl(mkextrn($1), NULL));          }
    | NAME ',' EXTRNLIST        { $$ = listfront($3, mkdecl(mkextrn($1), NULL)); }
    ;

AUTOLIST
    : NAME                          { $$ = mklist(mkdecl(mkauto($1), NULL));  }
    | NAME CONSTANT                 { $$ = mklist(mkdecl(mkauto($1), $2));    } /* TODO: vector */
    | NAME ',' AUTOLIST             { $$ = listfront($3, mkdecl(mkauto($1), NULL)); }
    | NAME CONSTANT ',' AUTOLIST    { $$ = listfront($4, mkdecl(mkauto($1), $2)); }
    ;

RVALUE
    : '(' RVALUE ')'            { $$ = $2; };
    | LVALUE                    { $$ = $1; }
    | CONSTANT                  { $$ = $1; }
    | LVALUE ASSIGN RVALUE      { $$ = mkassign($2, $1, $3); }
    | INC-DEC LVALUE            { $$ = mkunary($1, $2, 1); }
    | LVALUE INC-DEC            { $$ = mkunary($2, $1, 0); }
    | UNARY RVALUE              { $$ = mkunary($1, $2, 0); }
    | RVALUE BINARY RVALUE      { $$ = mkbinary($2, $1, $3); }
    | RVALUE '(' CALLIST ')'    { $$ = mkcall($1, $3); }
    ;

ASSIGN
    : '='           { $$ = $1; }
    | ASEQU         { $$ = $1; }
    | ASOREQU       { $$ = $1; }
    | ASLESSEQU     { $$ = $1; }
    | ASGREATEQU    { $$ = $1; }
    | ASSHL         { $$ = $1; }
    | ASSHR         { $$ = $1; }
    | ASOR          { $$ = $1; }
    | ASAND         { $$ = $1; }
    | ASLESS        { $$ = $1; }
    | ASGREAT       { $$ = $1; }
    | ASPLUS        { $$ = $1; }
    | ASMINUS       { $$ = $1; }
    | ASMOD         { $$ = $1; }
    | ASMUL         { $$ = $1; }
    | ASDIV         { $$ = $1; }
    ;

INC-DEC
    : INC       { $$ = $1; }
    | DEC       { $$ = $1; }
    ;

UNARY
    : '-'       { $$ = $1; }
    | '!'       { $$ = $1; }
    ;

BINARY
    : '|'       { $$ = $1; }
    | '&'       { $$ = $1; }
    | EQU       { $$ = $1; }
    | NOTEQU    { $$ = $1; }
    | '<'       { $$ = $1; }
    | LESSEQU   { $$ = $1; }
    | '>'       { $$ = $1; }
    | GREATEQU  { $$ = $1; }
    | SHL       { $$ = $1; }
    | SHR       { $$ = $1; }
    | '-'       { $$ = $1; }
    | '+'       { $$ = $1; }
    | '%'       { $$ = $1; }
    | '*'       { $$ = $1; }
    | '/'       { $$ = $1; }
    ;

CALLIST
    :                       { $$ = NULL; }
    | RVALUE                { $$ = mklist($1); }
    | RVALUE ',' CALLIST    { $$ = listfront($3, $1); }
    ;

LVALUE
    : NAME                  { $$ = mkname($1); }
    | RVALUE '[' RVALUE ']' { $$ = mkvecelem($1, $3); }
    ;

CONSTANT
    : INT   { $$ = mkint($1); }
    | STR   { $$ = mkstr($1); }
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
