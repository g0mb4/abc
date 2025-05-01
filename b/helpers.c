#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

struct node *finddecl(struct node *dlist, struct node *name)
{
    assert(name);
    struct list_node *curr = ASLIST(dlist);

    assert(name->type == N_NAME);
    const char *namestr = ASNAME(name)->val;

    while (curr) {
        assert(curr->val->type == N_EXTERN || curr->val->type == N_AUTO);
        if (curr->val->type == N_EXTERN) {
            if (!strcmp(ASEXTERN(curr->val)->val, namestr))
                return ASNODE(curr->val);
        } else {
            if (!strcmp(ASAUTO(curr->val)->val, namestr))
                return ASNODE(curr->val);
        }
        curr = curr->next;
    }

    return NULL;
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

static void printlist(struct node *n, int indent)
{
    if (!n) {
        printf("<empty_list>\n");
        return;
    }

    assert(n->type == N_LIST);

    struct list_node *curr = (struct list_node*)n;

    printf("LIST(%d) begin: \n", n->id);
    while (curr) {
        print(curr->val, indent + 2);
        curr = curr->next;
    }
    printf("LIST(%d) end\n", n->id);
}

static void printdef(struct node *f, int indent)
{
    assert(f);
    assert(f->type == N_DEF);

    printf("%*sDEF(%d) begin:\n", indent+1, "", f->id);
    struct def_node *func = (struct def_node*)f;
    printf("%*sNAME: ", indent+1, ""); 
    print(func->name, indent + 2);
    printf("%*sARGS: ", indent+1, ""); 
    printlist(func->args, indent + 2);
    printf("%*sDECLS: ", indent+1, ""); 
    printlist(func->decls, indent + 2);
    printf("%*sBODY:", indent+1, ""); 
    printlist(func->body, indent + 2);
    printf("%*sDEF(%d) end:\n", indent+1, "", f->id);
}

static void printcall(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_CALL);

    struct def_node *f = (struct def_node*)n;

    printf("%*sCALL(%d):\n", indent, "", n->id);
    printf("%*sNAME: ", indent+1, ""); 
    print(f->name, indent + 2);
    printf("%*sARGS: ", indent+1, ""); 
    printlist(f->args, indent + 2);
}

static void printassign(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_ASSIGN);

    struct assign_node *a = (struct assign_node*)n;

    assert(a->left);
    assert(a->right);

    printf("%*sASSIGN(%d):\n", indent, "", a->id);
    printf("%*sLEFT:\n", indent, "");
    print(a->left, indent + 2);
    printf("%*sRIGHT:\n", indent, "");
    print(a->right, indent + 2);
}

static void printbinary(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_BINARY);

    struct binary_node *b = (struct binary_node*)n;

    printf("%*sBINARY(%d):\n", indent, "", b->id);
    printf("%*sOP: %c\n", indent, "", b->op);
    printf("%*sLEFT:\n", indent, "");
    print(b->left, indent + 2);
    printf("%*sRIGHT:\n", indent, "");
    print(b->right, indent + 2);
}

static void printreturn(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_RETURN);

    struct return_node *ret = (struct return_node*)n;

    printf("%*sRETURN(%d)\n", indent, "", ret->id);
    if (ret->val)
        print(ret->val, indent);
}

static void printif(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_IF);

    struct if_node *iff = (struct if_node*)n;

    assert(iff->cond);
    assert(iff->truee);

    printf("%*sIF(%d) begin:\n", indent, "", iff->id);
    printf("%*sCOND:\n", indent, "");
    print(iff->cond, indent + 2);
    printf("%*sTRUE:\n", indent, "");
    print(iff->truee, indent + 2);

    if (iff->falsee){
        printf("%*sFALSE:\n", indent, "");
        print(iff->falsee, indent + 2);
    }

    printf("%*sIF(%d) end\n", indent, "", iff->id);
}

static void printwhile(struct node *n, int indent)
{
    assert(n);
    assert(n->type == N_WHILE);

    struct while_node *w = (struct while_node*)n;

    assert(w->cond);
    assert(w->body);

    printf("%*sWHILE(%d) begin:\n", indent, "", w->id);
    printf("%*sCOND:\n", indent, "");
    print(w->cond, indent + 2);
    printf("%*sBODY:\n", indent, "");
    print(w->body, indent + 2);

    printf("%*sWHILE(%d) end\n", indent, "", w->id);
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
    case N_ASSIGN:
        printassign(n, indent);
        break;
    case N_BINARY:
        printbinary(n, indent);
        break;
    case N_RETURN:
        printreturn(n, indent);
        break;
    case N_IF:
        printif(n, indent);
        break;
    case N_WHILE:
        printwhile(n, indent);
        break;
    default:
        printf("unknown type: %d\n", n->type);
        assert(0);
    };
}