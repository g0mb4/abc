#ifndef AST_H
#define AST_H

#define ASNODE(n) ((struct node *)n)
#define ASSTR(n) ((struct strnode *)n)
#define ASINT(n) ((struct intnode *)n)
#define ASNAME(n) ((struct namenode *)n)
#define ASLIST(n) ((struct listnode *)n)
#define ASEXTERN(n) ((struct extrnnode *)n)
#define ASAUTO(n) ((struct autonode *)n)

typedef long long int word;
#define WORDSIZE    sizeof(word)

enum {
    N_EMPTY = 0,
    N_STRING,
    N_INT,
    N_NAME,
    N_LIST,
    N_CALL,
    N_FUNDEF,
    N_EXTERN,
    N_AUTO,
    N_ASSIGN,
    N_UNARY,
    N_BINARY,
    N_RETURN,
    N_IF,
    N_WHILE,
    N_VECELEM,
    N_VARDEF,
    N_VECDEF,
    N_TERNARY, 
};

struct node {
    int type;
    int id;
};

struct strnode {
    int type;
    int id;
    
    const char *val;
};

struct intnode {
    int type;
    int id;

    word val;
};

struct namenode {
    int type;
    int id;
    
    const char *val;
};

struct listnode {
    int type;
    int id;

    struct node *val;
    struct listnode *next;
};

struct callnode {
    int type;
    int id;

    struct node *name;
    struct node *args;
};

struct fundefnode {
    int type;
    int id;

    struct node *name;
    struct node *args;
    struct node *decls;
    struct node *body;

    /* for codegen */
    word stacksize;
};

struct extrnnode {
    int type;
    int id;
    
    const char *val;
};

struct autonode {
    int type;
    int id;
    
    const char *val;
    struct node *init;

    /* for codegen */
    word offset;    
};

struct assignnode {
    int type;
    int id;
    
    int op;
    struct node *left;
    struct node *right;
};

struct unarynode {
    int type;
    int id;

    int op;
    
    struct node *val;
    int pre;
};

struct binarynode {
    int type;
    int id;

    int op;
    
    struct node *left;
    struct node *right;
};

struct ternarynode  {
    int type;
    int id;

    struct node *cond;
    struct node *truee;
    struct node *falsee;
};

struct returnnode {
    int type;
    int id;
    
    struct node *val;
};

struct ifnode {
    int type;
    int id;
    
    struct node *cond;
    struct node *truee;
    struct node *falsee;
};

struct whilenode {
    int type;
    int id;
    
    struct node *cond;
    struct node *body;
};

struct vecelemnode {
    int type;
    int id;
    
    struct node *vec;
    struct node *index;
};

struct vardefnode {
    int type;
    int id;
    
    struct node *name;
    struct node *init;
};

struct vecdefnode {
    int type;
    int id;
    
    struct node *name;
    struct node *count;
};

struct node *mkempty(void);

struct node *mkstr(const char *s);
struct node *mkint(word w);
struct node *mkname(const char *s);

struct node *mklist(struct node *e);
struct node *listback(struct node *l, struct node *n);
struct node *listfront(struct node *l, struct node *n);
struct node *mkdecl(struct node *n, struct node *i);

struct node *mkcall(struct node *name, struct node *args);
struct node *mkfundef(struct node *name, struct node *args, struct node *body);

struct node *mkextrn(const char *s);
struct node *mkauto(const char *s);

struct node *mkassign(int op, struct node *l, struct node *r);
struct node *mkbinary(int op, struct node *l, struct node *r);
struct node *mkunary(int op, struct node *v, int pre);
struct node *mkternary(struct node *c, struct node *t, struct node *f);

struct node *mkreturn(struct node *v);
struct node *mkif(struct node *c, struct node *t, struct node *f);
struct node *mkwhile(struct node *c, struct node *b);

struct node *mkvecelem(struct node *v, struct node *i);

struct node *mkvardef(struct node *name, struct node *init);
struct node *mkvecdef(struct node *name, struct node *count);

#endif /* AST_H */
