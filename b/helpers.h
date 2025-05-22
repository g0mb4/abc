#ifndef HELPERS_H
#define HELPERS_H

#include "ast.h"

void nerror(struct node *n, const char * fmt, ...);

struct node *finddecl(struct node *dlist, struct node *name);
struct node *listfront(struct node *l, struct node *n);
struct node *listback(struct node *l, struct node *n);
int listlen(struct node *l);

void print(struct node *n);

#endif /* HELPERS_H */
