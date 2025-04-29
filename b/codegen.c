#include "codegen.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

extern FILE *out;    /* from main.c */

// TODO: dynarr
static const char *data[1024];
static int data_ctr;

static struct node *decls = NULL;

static void gen(struct node *n);

static word gendecl(void)
{
    struct auto_node *a;
    word stack_offset = 0;

    if (!decls)
        return 0;

    struct list_node *curr = ASLIST(decls);
    while (curr) {
        assert(curr->val->type == N_EXTERN || curr->val->type == N_AUTO);
        if (curr->val->type == N_AUTO) {
            a = ASAUTO(curr->val);
            stack_offset += WORD_SIZE;
            ASAUTO(curr->val)->offset = stack_offset;   /* update the list element, not the copy */
            fprintf(out, "\tsubq $%d, %%rsp\n", WORD_SIZE);

            if (a->init) {
                assert(a->init->type == N_INT);
                fprintf(out, "\tmovq $%llu, -%llu(%%rbp)\n", ASINT(a->init)->val, stack_offset);
            }
        }
        curr = curr->next;
    }

    return stack_offset;
}

static void gendef(struct node *n)
{
    assert(n->type == N_DEF);

    struct def_node *def = (struct def_node*)n;
    word stack_offset;

    assert(def->args == NULL);
    assert(def->body);

    fprintf(out, "\t.global %s\n", ASSTR(def->name)->val);
    fprintf(out, "%s:\n", ASSTR(def->name)->val);

    // prolog
    fprintf(out, "\tpushq %%rbp\n");
    fprintf(out, "\tmovq %%rsp, %%rbp\n");

    decls = def->decls;
    stack_offset = gendecl();

    gen(def->body);

    // epilog
    fprintf(out, "\taddq $%llu, %%rsp\n", stack_offset);
    fprintf(out, "\tpopq %%rbp\n");
    fprintf(out, "\tret\n");
}

static void genlist(struct node *n)
{
    if (!n)
        return;

    assert(n->type == N_LIST);
    struct list_node *curr = (struct list_node*)n;
    while (curr) {
        gen(curr->val);
        curr = curr->next;
    }
}

static char *argreg(int index)
{
    switch(index) {
    case 0: return "%rdi";
    case 1: return "%rsi";
    case 2: return "%rdx";
    case 3: return "%rcx";
    case 4: return "%r8";
    case 5: return "%r9";
    default:
        assert(0);
    }
}

static void gencall(struct node *n)
{
    int argindex; 
    assert(n);
    assert(n->type == N_CALL);

    char buffer[128];

    struct call_node *call = (struct call_node*)n;

    if (call->args)
    {
        struct list_node *args = (struct list_node*)call->args;
        argindex = 0;
        while (args) {
            struct node *arg = args->val;

            if (arg->type == N_STRING) {
                fprintf(out, "\tmovq $str_%d, %s\n", arg->id, argreg(argindex));
                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "str_%d: .ascii \"%s\"\n", arg->id, ASSTR(arg)->val);
                data[data_ctr++] = strdup(buffer);
            } else if (arg->type == N_INT) {
                fprintf(out, "\tmovq $%llu, %s\n", ASINT(arg)->val, argreg(argindex));
            } else if (arg->type == N_NAME) {
                struct auto_node *a = (struct auto_node *)arg;
                struct node *d = finddecl(decls, a);
                assert(d);
                fprintf(out, "\tmovq -%llu(%%rbp), %s\n", ASAUTO(d)->offset, argreg(argindex));
            } else {
                assert(0);
            }

            args = args->next;
            argindex++;
        }
    }

    fprintf(out, "\tcall %s\n", ((struct name_node *)call->name)->val);
}

static void genassign(struct node *n)
{
    assert(n);
    assert(n->type == N_ASSIGN);

    struct assign_node *a = (struct assign_node *)n;

    assert(a->left);
    assert(a->left->type == N_NAME);
    assert(a->right);
    assert(a->right->type == N_INT);

    struct node *left = finddecl(decls, a->left);
    assert(left);

    assert(left->type == N_AUTO || left->type == N_EXTERN);
    if (left->type == N_AUTO) {
        fprintf(out, "\tmovq $%llu, -%llu(%%rbp)\n", ASINT(a->right)->val, ASAUTO(left)->offset);
    } else if (left->type == N_EXTERN) {
        fprintf(out, "\tmovq $%llu, %s\n", ASINT(a->right)->val, ASEXTERN(left)->val);
    } else {
        assert(0);
    }
}

static void gen(struct node *n)
{
    assert(n);
    switch(n->type) {
    case N_EMPTY:
        /* nothing */
        break;
    case N_STRING:
        /* nothing */
        break;
    case N_INT:
        /* nothing */
        break;
    case N_NAME:
        /* nothing */
        break;
    case N_EXTERN:
        /* nothing */
        break;
    case N_AUTO:
        /* nothing */
        break;
    case N_LIST:
        genlist(n);
        break;
    case N_CALL:
        gencall(n);
        break;
    case N_DEF:
        gendef(n);
        break;
    case N_ASSIGN:
        genassign(n);
        break;
    default:
        assert(0);
    };
}

void codegen(struct node* root)
{
#ifdef DEBUG
    print(root, 0);
#endif

    fprintf(out, "\t.text\n");
    gen(root);

    fprintf(out, "\t.data\n");
    for (int i = 0; i < data_ctr; ++i) {
        fprintf(out, "%s", data[i]);
    }
}
