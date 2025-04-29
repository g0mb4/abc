#include "ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int id = 0;
static struct node *decls = NULL;

extern void yyerror(const char * s, ...);
static void printlist(struct node *l, int indent);

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

struct node *listback(struct node *l, struct node *n)
{
    if (!l)
        return list(n);

    assert(l->type == N_LIST);
    struct list_node *curr = (struct list_node*)l;

    while (curr->next) {
        curr = curr->next;
    }

    curr->next = (struct list_node*)list(n);
    return l;
}

struct node *listfront(struct node *l, struct node *n)
{
    if (!l)
        return list(n);

    assert(l->type == N_LIST);

    struct list_node *new = (struct list_node*)list(n);
    new->next = (struct list_node*)l;

    return (struct node*)new;
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

static struct node *finddecl(struct node *name)
{
    struct list_node *curr = ASLIST(decls);
    while (curr) {
        assert(curr->val->type == N_EXTERN || curr->val->type == N_AUTO);
        if (curr->val->type == N_EXTERN) {
            if (!strcmp(ASEXTERN(curr->val)->val, ASNAME(name)->val))
                return ASNODE(curr->val);
        } else {
            if (!strcmp(ASAUTO(curr->val)->val, ASNAME(name)->val))
                return ASNODE(curr->val);
        }
        curr = curr->next;
    }

    return NULL;
}

struct node *call(struct node *name, struct node *args)
{
    assert(name);
    assert(name->type == N_NAME);

    struct node *def = finddecl(name);
    if (!def || def->type != N_EXTERN)
        yyerror("unknown extern funcion `%s`", ASNAME(name)->val);

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
    assert(name->type == N_NAME);
    assert(body);
    assert(body->type == N_LIST);

    if (args)
        assert(args->type == N_LIST);
    
    struct def_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_DEF;
    n->id = id++;

    n->name = name;
    n->args = args;
    n->body = body;
    n->decls = decls;

    decls = NULL;

    return (struct node*)n;
}

static void printlist(struct node *l, int indent)
{
    if (!l) {
        printf("<empty_list>\n");
        return;
    }

    assert(l->type == N_LIST);
    struct list_node *curr = (struct list_node*)l;
    printf("LIST(%d) begin: \n", l->id);
    while (curr) {
        print(curr->val, indent + 2);
        curr = curr->next;
    }
    printf("LIST(%d) end\n", l->id);
}

static void printdef(struct node *f, int indent)
{
    assert(f);
    assert(f->type == N_DEF);

    printf("%*sDEF(%d):\n", indent+1, "", f->id);
    struct def_node *func = (struct def_node*)f;
    printf("%*sNAME: ", indent+1, ""); 
    print(func->name, indent + 2);
    printf("%*sARGS: ", indent+1, ""); 
    printlist(func->args, indent + 2);
    printf("%*sDECLS: ", indent+1, ""); 
    printlist(func->decls, indent + 2);
    printf("%*sBODY:", indent+1, ""); 
    printlist(func->body, indent + 2);
}

static void printcall(struct node *f, int indent)
{
    assert(f);
    assert(f->type == N_CALL);

    printf("%*sCALL(%d):\n", indent, "", f->id);
    struct def_node *func = (struct def_node*)f;
    printf("%*sNAME: ", indent+1, ""); 
    print(func->name, indent + 2);
    printf("%*sARGS: ", indent+1, ""); 
    printlist(func->args, indent + 2);
}

void print(struct node *n, int indent)
{
    assert(n);
    switch(n->type) {
    case N_EMPTY:
        printf("%*sEMPTY(%d)\n", indent, "", n->id);
        break;
    case N_STRING:
        printf("%*sSTR(%d): `%s`\n", indent, "", n->id, ASSTR(n)->val);
        break;
    case N_INT:
        printf("%*sINT(%d): %llu\n", indent, "", n->id, ASINT(n)->val);
        break;
    case N_NAME:
        printf("%*sNAME(%d): `%s`\n", indent, "", n->id, ASNAME(n)->val);
        break;
    case N_EXTERN:
        printf("%*sEXTERN(%d): `%s`\n", indent, "", n->id, ASEXTERN(n)->val);
        break;
    case N_AUTO:
        printf("%*sAUTO(%d): `%s`", indent, "", n->id, ASAUTO(n)->val);
        if (ASAUTO(n)->init) {
            printf("=");
            print(ASAUTO(n)->init, 0);
        }
        printf("\n");
        break;
    case N_LIST:
        printlist(n, indent);
        break;
    case N_CALL:
        printcall(n, indent);
        break;
    case N_DEF:
        printdef(n, indent);
        break;
    default:
        printf("unknown type: %d\n", n->type);
        assert(0);
    };
}
