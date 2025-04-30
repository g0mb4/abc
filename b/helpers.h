#ifndef HELPERS_H
#define HELPERS_H

#include "ast.h"

struct node *finddecl(struct node *dlist, struct node *name);
struct node *listback(struct node *l, struct node *n);
struct node *listfront(struct node *l, struct node *n);

void printlist(struct node *n, int indent);
void printdef(struct node *f, int indent);
void printcall(struct node *n, int indent);
void printassign(struct node *n, int indent);
void printbinary(struct node *n, int indent);
void print(struct node *n, int indent);

#endif /* HELPERS_H */
