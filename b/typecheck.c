#include "typecheck.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void terror(const char * fmt, ...)
{
    va_list args;

    fprintf(stderr, "type error: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}

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

static void build(struct node *n);
static void check(struct node *n);

static void resettypes(void) {
    typesctr = 0;
}

static void addtype(const char *scope, const char *name, int type)
{
    assert(name);
    assert(type >= 0);
    assert(typesctr < TYPESSIZE);

    types[typesctr].scope = scope;
    types[typesctr].name = name;
    types[typesctr].type = type;

    typesctr++;
}

static int gettype(const char *scope, const char *name)
{
    int i;

    assert(name);
    assert(scope);

    for (i = 0; i < typesctr; ++i) {
        if (scope) {
            if (!strcmp(scope, types[i].scope)
                && !strcmp(name, types[i].name)) {
                return types[i].type;
            }
        } else {
            if (!types[i].scope
                && !strcmp(name, types[i].name)) {
                return types[i].type;
            }
        }
    }

    assert(0);
    return -1;
}

static void settype(const char *scope, const char *name, int type)
{
    int i;

    assert(name);

    for (i = 0; i < typesctr; ++i) {
         if (scope) {
            if (!strcmp(scope, types[i].scope)
                && !strcmp(name, types[i].name)) {
                types[i].type = type;
            }
        } else {
            if (!types[i].scope
                && !strcmp(name, types[i].name)) {
                types[i].type = type;
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

    build(def->decls);

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

static void buildvardef(struct node *n)
{
    assert(n->type == N_VARDEF);

    struct vardefnode *def = (struct vardefnode*)n;

    settype(NULL, ASNAME(def->name)->val, 0);
}

static void buildvecdef(struct node *n)
{
    assert(n->type == N_VECDEF);

    struct vecdefnode *def = (struct vecdefnode*)n;

    settype(NULL, ASNAME(def->name)->val, 1);
}

static void build(struct node *n)
{
    assert(n);
    switch(n->type) {
    case N_EMPTY:
        /* do nothing */
        break;

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

    case N_VARDEF:
        buildvardef(n);
        break;

    case N_VECDEF:
        buildvecdef(n);
        break;

    default:
        /* TODO: enable this! */
        /*assert(0);*/
    };
}

/*
    Update types from defintions.
*/
static void updatedefs(void)
{
    int i, j;
    struct type *t1, *t2;

    for (i = 0; i < typesctr; ++i) {
        t1 = &types[i];

        if (t1->scope)
            continue;

        for (j = 0; j < typesctr; ++j) {
            t2 = &types[j];

            if (!t2->scope)
                continue;

            if (!strcmp(t1->name, t2->name))
                t2->type = t1->type;
        }
    }
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
        terror("`%s` is not a vector", ASNAME(v->vec)->val);
}

static void check(struct node *n)
{
    assert(n);
    switch(n->type) {
    case N_EMPTY:
    case N_EXTERN:
    case N_AUTO:
    case N_VARDEF:
    case N_VECDEF:
        /* do nothing */
        break;

    case N_FUNDEF:
        checkfundef(n);
        break;

    case N_LIST:
        checklist(n);
        break;

    case N_VECELEM:
        checkvecelem(n);
        break;

    default:
        /* TODO: enable this! */
        /*assert(0);*/
    };
}

void typecheck(struct node* root)
{
    build(root);
    updatedefs();
    check(root);

#ifdef DEBUG
    printtypes();
#endif
}
