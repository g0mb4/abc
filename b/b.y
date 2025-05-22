%{

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ast.h"
#include "helpers.h"
#include "typecheck.h"
#include "codegen.h"

extern int yylex(void);                 /* yyparse() needs it */
extern int yyparse(void);               /* because we define a custom "main()" */
void yyerror(const char * s, ...);      /* custom error report */

extern const char *infile;              /* from main.c */
extern int yylineno;                    /* global variable for error report */

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
%token GOTO
%token SWITCH
%token CASE

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

%type <n>EXTRNLIST <n>AUTOLIST <n>DEFLIST <n>CALLIST
%type <n>CONSTANT <n>LVALUE <n>RVALUE
%type <n>STATEMENT <n>STATEMENTS <n>DEFINITION <n>DEFINITIONS

/* lowest precedence */
%right '=' ASEQU ASOREQU ASLESSEQU ASGREATEQU ASSHL ASSHR ASOR ASAND ASLESS ASGREAT ASPLUS ASMINUS ASMOD ASMUL ASDIV
%right '?' ':'
%left '|'
%left '&'
%left EQU NOTEQU
%left '<' '>' LESSEQU GREATEQU
%left SHL SHR
%left '+' '-'
%left '*' '/' '%'
%precedence ADDR
%precedence UNARY
%precedence PRE
%precedence POST
/* highest precedence */

%start PROGRAM

%%

PROGRAM
    : DEFINITIONS   {
                        #ifdef DEBUG
                            print($1);
                        #endif
                        typecheck($1);
                        codegen($1);
                    }
    ;

DEFINITIONS
    :                           { $$ = NULL;              }
    | DEFINITION DEFINITIONS    { $$ = listfront($2, $1); }
    ;

DEFINITION
    : NAME '(' DEFLIST ')' STATEMENT  { $$ = mkfundef(mkname($1), $3, $5); }
    | NAME ';'                        { $$ = mkvardef(mkname($1), NULL);   }
    | NAME CONSTANT ';'               { $$ = mkvardef(mkname($1), $2);     }
    | NAME '[' CONSTANT ']' ';'       { $$ = mkvecdef(mkname($1), $3);     } /* TODO: lintializer list */
    ;

DEFLIST
    :                           { $$ = NULL;                      }
    | NAME                      { $$ = mklist(mkname($1));        }
    | NAME ',' DEFLIST          { $$ = listback($3, mkname($1)); }
    ;

STATEMENTS
    : STATEMENT                 { $$ = mklist($1);        }
    | STATEMENT STATEMENTS      { $$ = listfront($2, $1); }
    ;

STATEMENT
    : AUTO AUTOLIST ';'                             { $$ = $2;                 }
    | EXTRN EXTRNLIST ';'                           { $$ = $2;                 }
    | NAME ':' STATEMENT                            { $$ = mklabel($1, $3);    }
    | CASE CONSTANT ':' STATEMENT                   { $$ = mkcase($2, $4);     }
    | '{' STATEMENTS '}'                            { $$ = $2;                 }
    | IF '(' RVALUE ')' STATEMENT                   { $$ = mkif($3, $5, NULL); }
    | IF '(' RVALUE ')' STATEMENT ELSE STATEMENT    { $$ = mkif($3, $5, $7);   }
    | WHILE '(' RVALUE ')' STATEMENT                { $$ = mkwhile($3, $5);    }
    | SWITCH RVALUE STATEMENT                       { $$ = mkswitch($2, $3);   }
    | GOTO RVALUE ';'                               { $$ = mkgoto($2);         }
    | RETURN ';'                                    { $$ = mkreturn(NULL);     }
    | RETURN '(' RVALUE ')' ';'                     { $$ = mkreturn($3);       }
    | RVALUE ';'                                    { $$ = $1;                 }
    | ';'                                           { $$ = mkempty();          }
    ;

EXTRNLIST
    : NAME                      { $$ = mklist(mkdecl(mkextrn($1), NULL));        }
    | NAME ',' EXTRNLIST        { $$ = listfront($3, mkdecl(mkextrn($1), NULL)); }
    ;

AUTOLIST
    : NAME                          { $$ = mklist(mkdecl(mkauto($1), NULL));        }
    | NAME CONSTANT                 { $$ = mklist(mkdecl(mkauto($1), $2));          }
    | NAME ',' AUTOLIST             { $$ = listfront($3, mkdecl(mkauto($1), NULL)); }
    | NAME CONSTANT ',' AUTOLIST    { $$ = listfront($4, mkdecl(mkauto($1), $2));   }
    ;

RVALUE
    : '(' RVALUE ')'            { $$ = $2; }
    | LVALUE                    { $$ = $1; }
    | CONSTANT                  { $$ = $1; }
    /* ASSIGN */
    | LVALUE '='        RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASEQU      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASOREQU    RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASLESSEQU  RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASGREATEQU RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASSHL      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASSHR      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASOR       RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASAND      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASLESS     RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASGREAT    RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASPLUS     RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASMINUS    RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASMOD      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASMUL      RVALUE  { $$ = mkassign($2, $1, $3); }
    | LVALUE ASDIV      RVALUE  { $$ = mkassign($2, $1, $3); }
    /* ----------- */
    /* INC-DEC */
    | INC RVALUE %prec PRE      { $$ = mkunary($1, $2, 1); }
    | DEC RVALUE %prec PRE      { $$ = mkunary($1, $2, 1); }
    | RVALUE INC %prec POST     { $$ = mkunary($2, $1, 0); }
    | RVALUE DEC %prec POST     { $$ = mkunary($2, $1, 0); }
    /* ----------- */
    /* UNARY */
    | '-' RVALUE %prec UNARY    { $$ = mkunary($1, $2, 0); }
    | '!' RVALUE %prec UNARY    { $$ = mkunary($1, $2, 0); }
    /* ----------- */
    | '&' RVALUE %prec ADDR     { $$ = mkunary($1, $2, 0); }
    /* BINARY */
    | RVALUE '|'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '&'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE EQU      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE NOTEQU   RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '<'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE LESSEQU  RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '>'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE GREATEQU RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE SHL      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE SHR      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '-'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '+'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '%'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '*'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    | RVALUE '/'      RVALUE    { $$ = mkbinary($2, $1, $3); }
    /* ----------- */
    | RVALUE '?' RVALUE ':' RVALUE  { $$ = mkternary($1, $3, $5); }
    | RVALUE '(' CALLIST ')'        { $$ = mkcall($1, $3);        }
    ;

CALLIST
    :                       { $$ = NULL;              }
    | RVALUE                { $$ = mklist($1);        }
    | RVALUE ',' CALLIST    { $$ = listfront($3, $1); }
    ;

LVALUE
    : NAME                  { $$ = mkname($1);          }
    | '*' RVALUE            { $$ = mkunary($1, $2, 0);  }
    | RVALUE '[' RVALUE ']' { $$ = mkvecelem($1, $3);   }
    ;

CONSTANT
    : INT   { $$ = mkint($1); }  /* NOTE: char is an int */
    | STR   { $$ = mkstr($1); }
    ;

%%

void yyerror(const char * fmt, ...)
{
    va_list args;

    fprintf(stderr, "%s: %d: ", infile, yylineno);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}
