#include "codegen.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


extern FILE *out;    /* from main.c */

// TODO: dynarr
static const char *data[1024];
static int data_ctr;

static void gen(struct node *n);

static void gendef(struct node *n)
{
    assert(n->type == N_DEF);

    struct def_node *def = (struct def_node*)n;
    int stack_offset = 0;

    assert(def->args == NULL);
    assert(def->body);

    fprintf(out, "\t.global %s\n", ASSTR(def->name)->val);
    fprintf(out, "%s:\n", ASSTR(def->name)->val);

    // prolog
    fprintf(out, "\tpushq %%rbp\n");
    fprintf(out, "\tmovq %%rsp, %%rbp\n");

    gen(def->body);

    // epilog
    fprintf(out, "\tadd %d, %%rsp\n", stack_offset);
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

            assert(arg->type == N_STRING || arg->type == N_INT);
            if (arg->type == N_STRING) {
                fprintf(out, "\tmov $str_%d, %s\n", arg->id, argreg(argindex));

                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "str_%d: .ascii \"%s\"\n", arg->id, ASSTR(arg)->val);
                data[data_ctr++] = strdup(buffer);
            } else {
                fprintf(out, "\tmov $%d, %s\n", ASINT(arg)->val, argreg(argindex));
            }

            args = args->next;
            argindex++;
        }
    }

    fprintf(out, "\tcall %s\n", ((struct name_node *)call->name)->val);
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
    case N_LIST:
        genlist(n);
        break;
    case N_CALL:
        gencall(n);
        break;
    case N_DEF:
        gendef(n);
        break;
    default:
        assert(0);
    };
}

void codegen(struct node* root)
{
    int i;
    //print(root, 0);

    fprintf(out, "\t.text\n");
    gen(root);

    fprintf(out, "\t.data\n");
    for (int i = 0; i < data_ctr; ++i) {
        fprintf(out, "%s", data[i]);
    }
}
