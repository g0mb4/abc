#include "ast.h"
#include "helpers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int id = 0;
static struct node *decls = NULL;

extern void yyerror(const char * s, ...);

struct node *mkempty(void)
{
    struct node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_EMPTY;
    n->id = id++;

    return n;
}

struct node *mkstr(const char *s)
{
    struct strnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_STRING;
    n->id = id++;

    n->val = strdup(s);

    return (struct node*)n;
}

struct node *mkint(word w)
{
    struct intnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_INT;
    n->id = id++;

    n->val = w;

    return (struct node*)n;
}

struct node *mkname(const char *s)
{
    struct namenode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_NAME;
    n->id = id++;

    n->val = strdup(s);

    return (struct node*)n;
}

struct node *mkextrn(const char *s)
{
    struct namenode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_EXTERN;
    n->id = id++;

    n->val = strdup(s);

    return (struct node*)n;
}

struct node *mkauto(const char *s)
{
    struct namenode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_AUTO;
    n->id = id++;

    n->val = strdup(s);

    return (struct node*)n;
}

struct node *mkassign(int op, struct node *l, struct node *r)
{
    struct assignnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    struct node* d = finddecl(decls, l);
    if (!d)
        yyerror("unknown variable `%s`", ASNAME(l)->val);

    n->type = N_ASSIGN;
    n->id = id++;

    n->op = op;
    n->left = l;
    n->right = r;

    return (struct node*)n;
}

struct node *mklist(struct node *e)
{
    struct listnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_LIST;
    n->id = id++;

    n->val = e;

    return (struct node*)n;
}

struct node *mkdecl(struct node *n, struct node *i)
{
    if (n->type == N_EXTERN)
        assert(i == NULL);

    if (i)
        ASAUTO(n)->init = i;

    /* TODO: check if variable is not defined already */

    decls = listback(decls, n);
    return mkempty();
}

struct node *mkcall(struct node *name, struct node *args)
{
    assert(name);
    assert(name->type == N_NAME);

    /* TODO: this prevents recursion */
#if 0
    struct node *def = finddecl(decls, name);
    if (!def || def->type != N_EXTERN)
        yyerror("unknown extrn funcion `%s`", ASNAME(name)->val);
#endif

    if (args)
        assert(args->type == N_LIST);
    
    struct callnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_CALL;
    n->id = id++;

    n->name = name;
    n->args = args;

    return (struct node*)n;
}

struct node *mkfundef(struct node *name, struct node *args, struct node *body)
{
    int arglen;

    assert(name);
    assert(body);

    struct fundefnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_FUNDEF;
    n->id = id++;

    assert(name->type == N_NAME);
    n->name = name;

    /* TODO: check if every arg is unique and not already defined in body */
    if (args) {
        arglen = listlen(args);

        if (arglen > 6) {
            yyerror("maximum number of arguments is 6");
        }
    }

    n->args = args;

    if (body->type == N_LIST)
        n->body = body;
    else
        n->body = mklist(body);

    n->decls = decls;

    decls = NULL;

    return (struct node*)n;
}

struct node *mkbinary(int op, struct node *l, struct node *r)
{
    struct binarynode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_BINARY;
    n->id = id++;

    n->op = op;
    n->left = l;
    n->right = r;

    return (struct node*)n;
}

struct node *mkunary(int op, struct node *v, int pre)
{
    struct unarynode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_UNARY;
    n->id = id++;

    n->op = op;
    n->val = v;
    n->pre = pre;

    return (struct node*)n;
}

struct node *mkreturn(struct node *v)
{
    struct returnnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_RETURN;
    n->id = id++;

    n->val = v;

    return (struct node*)n;
}

struct node *mkif(struct node *c, struct node *t, struct node *f)
{
    struct ifnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_IF;
    n->id = id++;

    n->cond = c;
    n->truee = t;
    n->falsee = f;

    return (struct node*)n;
}

struct node *mkwhile(struct node *c, struct node *b)
{
    struct whilenode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_WHILE;
    n->id = id++;

    n->cond = c;
    n->body = b;

    return (struct node*)n;
}

struct node *mkvecelem(struct node *v, struct node *i)
{
    struct vecelemnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_VECELEM;
    n->id = id++;

    n->vec = v;
    n->index = i;

    return (struct node*)n;
}

struct node *mkvardef(struct node *name, struct node *init)
{
    struct vardefnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_VARDEF;
    n->id = id++;

    n->name = name;

    if (init && init->type != N_INT)
        yyerror("inital value must be an integer");

    n->init = init;

    return (struct node*)n;
}

struct node *mkvecdef(struct node *name, struct node *count)
{
    struct vecdefnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_VECDEF;
    n->id = id++;

    n->name = name;

    if (count->type != N_INT)
        yyerror("count must be an integer");

    n->count = count;

    return (struct node*)n;
}

struct node *mkternary(struct node *c, struct node *t, struct node *f)
{
    struct ternarynode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_TERNARY;
    n->id = id++;

    n->cond = c;
    n->truee = t;
    n->falsee = f;

    return (struct node*)n;
}

struct node *mklabel(const char *name, struct node *statement)
{
    struct labelnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_LABEL;
    n->id = id++;

    /* TODO: check if label is already defined */
    n->name = name;
    n->statement = statement;

    return (struct node*)n;
}

struct node *mkgoto(struct node *label)
{
    struct gotonode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_GOTO;
    n->id = id++;

    /* TODO: check if label is defined inside the current function */
    assert(label->type == N_NAME);

    n->label = label;

    return (struct node*)n;
}

struct node *mkswitch(struct node *val, struct node *statement)
{
    struct switchnode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_SWITCH;
    n->id = id++;

    n->val = val;
    n->statement = statement;

    return (struct node*)n;
}

struct node *mkcase(struct node *constant, struct node *statement)
{
    struct casenode *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_CASE;
    n->id = id++;

    if (constant->type != N_INT)
        yyerror("only integer constants are allowed");

    n->constant = constant;
    n->statement = statement;

    return (struct node*)n;
}
