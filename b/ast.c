#include "ast.h"
#include "helpers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int id = 0;
static struct node *decls = NULL;

extern void yyerror(const char * s, ...);

struct node *empty(void)
{
    struct node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_EMPTY;
    n->id = id++;

    return n;
}

struct node *string(const char *s)
{
    struct string_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_STRING;
    n->id = id++;
    n->val = strdup(s);

    return (struct node*)n;
}

struct node *integer(word w)
{
    struct int_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_INT;
    n->id = id++;
    n->val = w;

    return (struct node*)n;
}

struct node *namen(const char *s)
{
    struct name_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_NAME;
    n->id = id++;
    n->val = strdup(s);

    return (struct node*)n;
}

struct node *externn(const char *s)
{
    struct name_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_EXTERN;
    n->id = id++;
    n->val = strdup(s);

    return (struct node*)n;
}

struct node *auton(const char *s)
{
    struct name_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_AUTO;
    n->id = id++;
    n->val = strdup(s);

    return (struct node*)n;
}

struct node *assignn(struct node *l, struct node *r)
{
    struct assign_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    assert(l->type == N_NAME);
    struct node* d = finddecl(decls, l);
    if (!d)
        yyerror("unknown variable `%s`", ASNAME(l)->val);

    n->type = N_ASSIGN;
    n->id = id++;
    n->left = l;
    n->right = r;

    return (struct node*)n;
}

struct node *list(struct node *e)
{
    struct list_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_LIST;
    n->id = id++;
    n->val = e;

    return (struct node*)n;
}

struct node *decl(struct node *n, struct node *i)
{
    if (n->type == N_EXTERN)
        assert(i == NULL);

    if (i)
        ASAUTO(n)->init = i;

    decls = listback(decls, n);
    return empty();
}

struct node *call(struct node *name, struct node *args)
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
    
    struct call_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_CALL;
    n->id = id++;
    n->name = name;
    n->args = args;

    return (struct node*)n;
}

struct node *def(struct node *name, struct node *args, struct node *body)
{
    assert(name);
    assert(body);

    struct def_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_DEF;
    n->id = id++;

    assert(name->type == N_NAME);
    n->name = name;

    if (args)
        assert(args->type == N_LIST);
    n->args = args;

    if (body->type == N_LIST)
        n->body = body;
    else
        n->body = list(body);

    n->decls = decls;

    decls = NULL;

    return (struct node*)n;
}

struct node *binaryn(int op, struct node *l, struct node *r)
{
    struct binary_node *n;
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

struct node *unarynn(int op, struct node *v, int pre)
{
    struct unary_node *n;
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

struct node *returnnn(struct node *v)
{
    struct return_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_RETURN;
    n->id = id++;

    n->val = v;

    return (struct node*)n;
}

struct node *ifn(struct node *c, struct node *t, struct node *f)
{
    struct if_node *n;
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

struct node *whilen(struct node *c, struct node *b)
{
    struct while_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_WHILE;
    n->id = id++;

    n->cond = c;
    n->body = b;

    return (struct node*)n;
}
