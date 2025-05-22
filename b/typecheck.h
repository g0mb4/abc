#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "ast.h"

void typecheck(struct node* root);
int calctype(struct node *n, const char *scope);

#endif /* TYPECHECK_H */
