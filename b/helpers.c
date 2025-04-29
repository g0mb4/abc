#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

struct node *finddecl(struct node *dlist, struct node *name)
{
    assert(name);
    struct list_node *curr = ASLIST(dlist);
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