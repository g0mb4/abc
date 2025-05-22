#include "typecheck.h"
#include "helpers.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct type {
    const char *scope;
    const char *name;
    int type;   /* 0 - val, 1 - ptr, 2 - ptr to ptr ... */
};

/* TODO: hasmap */
#define TYPESSIZE   1024
static struct type types[TYPESSIZE];
static int typesctr;

static struct fundefnode *currdef = NULL;

static void builddefs(struct node *n);
static void build(struct node *n);
static void check(struct node *n);

static int gettype(const char *scope, const char *name)
{
    int i;

    assert(name);

    for (i = 0; i < typesctr; ++i) {
        if (scope) {
            if (!types[i].scope)
                continue;

            if (!strcmp(scope, types[i].scope)
                && !strcmp(name, types[i].name)) {
                return types[i].type;
            }
        } else {
            if (!types[i].scope && !strcmp(name, types[i].name)) {
                return types[i].type;
            }
        }
    }

    return -1;
}

static void addtype(const char *scope, const char *name, int type)
{
    int t = -1;

    assert(name);
    assert(type >= 0);
    assert(typesctr < TYPESSIZE);

    if (scope) {
        t = gettype(NULL, name);
    }

    types[typesctr].scope = scope;
    types[typesctr].name = name;
    types[typesctr].type = t < 0 ? type : t;

    typesctr++;
}

static void settype(const char *scope, const char *name, int type)
{
    int i;

    assert(name);

    for (i = 0; i < typesctr; ++i) {
         if (scope) {
            if (!types[i].scope)
                continue;

            if (!strcmp(scope, types[i].scope)
                && !strcmp(name, types[i].name)) {
                types[i].type = type;
                return;
            }
        } else {
            if (!strcmp(name, types[i].name)) {
                types[i].type = type;
                return;
            }
        }
    }

    addtype(scope, name, type);
}

static void printtypes(void)
{
    int i;

    printf("types (%d): \n", typesctr);
    for (i = 0; i < typesctr; ++i) {
        if (types[i].scope) {
            printf("scope: `%s`, name: `%s`, type: %d\n",
                   types[i].scope, types[i].name, types[i].type);
        } else {
            printf("scope: -, name: `%s`, type: %d\n",
                   types[i].name, types[i].type);
        }
    }
}

static void builddefslist(struct node *n)
{
    if (!n)
        return;

    assert(n->type == N_LIST);
    struct listnode *curr = (struct listnode*)n;
    while (curr) {
        builddefs(curr->val);
        curr = curr->next;
    }
}

static void buildvardef(struct node *n)
{
    assert(n->type == N_VARDEF);

    struct vardefnode *def = (struct vardefnode*)n;

    addtype(NULL, ASNAME(def->name)->val, 0);
}

static void buildvecdef(struct node *n)
{
    assert(n->type == N_VECDEF);

    struct vecdefnode *def = (struct vecdefnode*)n;

    addtype(NULL, ASNAME(def->name)->val, 1);
}

static void builddefs(struct node *n) {
    assert(n);
    switch(n->type) {
    case N_LIST:
        builddefslist(n);
        break;

    case N_VARDEF:
        buildvardef(n);
        break;

    case N_VECDEF:
        buildvecdef(n);
        break;

    case N_EMPTY:
    case N_STRING:
    case N_INT:
    case N_NAME:
    case N_CALL:
    case N_FUNDEF:
    case N_EXTERN:
    case N_AUTO:
    case N_ASSIGN:
    case N_UNARY:
    case N_BINARY:
    case N_RETURN:
    case N_IF:
    case N_WHILE:
    case N_VECELEM:
    case N_TERNARY:
    case N_LABEL:
    case N_GOTO:
    case N_SWITCH:
    case N_CASE:
        /* do nothing */
        break;

    default:
        assert(0);
    }
}

static void buildextern(struct node *n)
{
    assert(currdef);
    assert(n->type == N_EXTERN);

    settype(ASNAME(currdef->name)->val, ASEXTERN(n)->val, 0);
}

static void buildauto(struct node *n)
{
    assert(currdef);
    assert(n->type == N_AUTO);

    settype(ASNAME(currdef->name)->val, ASAUTO(n)->val, 0);
}

static void buildfundef(struct node *n)
{
    assert(currdef == NULL);
    assert(n->type == N_FUNDEF);

    struct fundefnode *def = (struct fundefnode*)n;

    currdef = def;

    if (def->decls)
        build(def->decls);

    build(def->body);

    currdef = NULL;
}

static void buildlist(struct node *n)
{
    if (!n)
        return;

    assert(n->type == N_LIST);
    struct listnode *curr = (struct listnode*)n;
    while (curr) {
        build(curr->val);
        curr = curr->next;
    }
}

static int calcname(struct node *n, const char *scope)
{
    return gettype(scope, ASNAME(n)->val);
}

static int calcunary(struct node *n, const char *scope)
{
    struct unarynode *u = (struct unarynode *)n;

    int t = calctype(u->val, scope);

    switch (u->op) {
    case '*':
        assert(t > 0);
        return t - 1;
    case '&':
        return t + 1;
    default:
        return t;
    }
}

static int calcbinary(struct node *n, const char *scope)
{
    struct binarynode *b = (struct binarynode *)n;

    int tl = calctype(b->left, scope);
    int tr = calctype(b->right, scope);

    switch (b->op) {
    case '+':
        if (tr == 0 && tl == 0)
            return 0;
        else if (tr > 0 && tl == 0)
            return tr;
        else if (tl > 0 && tr == 0)
            return tl;
        else
            nerror(n, "not supported pointer operation");

        break;

    case '-':
        if (tr == 0 && tl == 0)
            return 0;
        else if (tr > 0 && tl > 0)
            return 0;
        else
            nerror(n, "not supported pointer operation");

        break;

    default:
        if (tr > 0 || tr > 0)
            nerror(n, "not supported pointer operation");
        else
            return tr;
    }

    return -1;
}

static int calcvecelem(struct node *n, const char *scope)
{
    struct vecelemnode *v = (struct vecelemnode *)n;

    int t = calcname(v->vec, scope);
    assert(t > 0);

    return t - 1;
}

int calctype(struct node *n, const char *scope)
{
    switch(n->type) {
        case N_NAME:
            return calcname(n, scope);

        case N_UNARY:
            return calcunary(n, scope);

        case N_BINARY:
            return calcbinary(n, scope);

        case N_VECELEM:
            return calcvecelem(n, scope);

        case N_EMPTY:
        case N_STRING:
        case N_INT:
        case N_LIST:
        case N_CALL:
        case N_FUNDEF:
        case N_EXTERN:
        case N_AUTO:
        case N_ASSIGN:
        case N_RETURN:
        case N_IF:
        case N_WHILE:
        case N_VARDEF:
        case N_VECDEF:
        case N_TERNARY:
        case N_LABEL:
        case N_GOTO:
        case N_SWITCH:
        case N_CASE:
            return 0;

        default:
            assert(0);
    }
}

static void buildassign(struct node *n)
{
    assert(currdef);

    assert(n->type == N_ASSIGN);

    struct assignnode *a = (struct assignnode *)n;

    if (a->left->type == N_NAME) {
        int t = calctype(a->right, ASNAME(currdef->name)->val);
        settype(ASNAME(currdef->name)->val, ASNAME(a->left)->val, t);
    }
}

static void build(struct node *n)
{
    assert(n);
    switch(n->type) {
    case N_EXTERN:
        buildextern(n);
        break;

    case N_AUTO:
        buildauto(n);
        break;

    case N_FUNDEF:
        buildfundef(n);
        break;

    case N_LIST:
        buildlist(n);
        break;

    case N_EMPTY:
    case N_VECDEF:
    case N_VARDEF:
    case N_STRING:
    case N_INT:
    case N_CALL:
    case N_RETURN:
    case N_IF:    /* TODO: check this */
    case N_WHILE:  /* TODO: check this */
    case N_VECELEM:
    case N_TERNARY:  /* TODO: check this */
    case N_LABEL:    /* TODO: check this */
    case N_GOTO:
    case N_SWITCH:   /* TODO: check this */
    case N_CASE:     /* TODO: check this */
    case N_NAME:
    case N_UNARY:
    case N_BINARY:
        /* do nothing */
        break;

    case N_ASSIGN:
        buildassign(n);
        break;

    default:
        assert(0);
    };
}

static void checkfundef(struct node *n)
{
    assert(currdef == NULL);
    assert(n->type == N_FUNDEF);

    struct fundefnode *def = (struct fundefnode*)n;

    currdef = def;

    check(def->body);

    currdef = NULL;
}

static void checklist(struct node *n)
{
    if (!n)
        return;

    assert(n->type == N_LIST);
    struct listnode *curr = (struct listnode*)n;
    while (curr) {
        check(curr->val);
        curr = curr->next;
    }
}

static void checkvecelem(struct node *n)
{
    assert(currdef);
    assert(n->type == N_VECELEM);

    struct vecelemnode *v = (struct vecelemnode*)n;
    int t;

    t = gettype(ASNAME(currdef->name)->val, ASNAME(v->vec)->val);
    if (!t)
        nerror(n, "`%s` is not a vector", ASNAME(v->vec)->val);
}

static void check(struct node *n)
{
    assert(n);
    switch(n->type) {
    case N_FUNDEF:
        checkfundef(n);
        break;

    case N_LIST:
        checklist(n);
        break;

    case N_VECELEM:
        checkvecelem(n);
        break;

    case N_EMPTY:
    case N_EXTERN:
    case N_AUTO:
    case N_VARDEF:
    case N_VECDEF:
    case N_STRING:
    case N_INT:
    case N_NAME:
    case N_CALL:
    case N_ASSIGN:
    case N_UNARY:
    case N_BINARY:
    case N_RETURN:
    case N_IF:
    case N_WHILE:
    case N_TERNARY:
    case N_LABEL:
    case N_GOTO:
    case N_SWITCH:
    case N_CASE:
        break;

    default:
        assert(0);
    };
}

void typecheck(struct node* root)
{
    builddefs(root);
    build(root);
    check(root);

#ifdef DEBUG
    printtypes();
#endif
}
