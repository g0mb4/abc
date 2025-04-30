#ifndef HELPERS_H
#define HELPERS_H

#include "ast.h"

struct node *finddecl(struct node *dlist, struct node *name);
struct node *listback(struct node *l, struct node *n);
struct node *listfront(struct node *l, struct node *n);

void print(struct node *n, int indent);

#endif /* HELPERS_H */
