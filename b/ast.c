#include "ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int id = 0;

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

struct node *integer(int i)
{
    struct int_node *n;
    n = malloc(sizeof(*n));
    assert(n);
    memset(n, 0, sizeof(*n));

    n->type = N_INT;
    n->id = id++;
    n->val = i;

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
    if (!l) {
        return list(n);
    }

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
    if (!l) {
        return list(n);
    }

    assert(l->type == N_LIST);

    struct list_node *new = (struct list_node*)list(n);
    new->next = (struct list_node*)l;

    return (struct node*)new;
}

struct node *call(struct node *name, struct node *args)
{
    assert(name);
    assert(name->type == N_NAME);

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

    return (struct node*)n;
}

static void printlist(struct node *l, int indent){
    if (!l) {
        printf("<empty_list>\n");
        return;
    }

    assert(l->type == N_LIST);
    struct list_node *curr = (struct list_node*)l;
    printf("LIST(%d): \n", l->id);
    while (curr) {
        print(curr->val, indent + 2);
        curr = curr->next;
    }
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
        printf("%*sINT(%d): %d\n", indent, "", n->id, ASINT(n)->val);
        break;
    case N_NAME:
        printf("%*sNAME(%d): `%s`\n", indent, "", n->id, ASNAME(n)->val);
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
