#ifndef AST_H
#define AST_H

#define ASNODE(n) ((struct node *)n)
#define ASSTR(n) ((struct string_node *)n)
#define ASINT(n) ((struct int_node *)n)
#define ASNAME(n) ((struct name_node *)n)
#define ASLIST(n) ((struct list_node *)n)
#define ASEXTERN(n) ((struct extern_node *)n)
#define ASAUTO(n) ((struct auto_node *)n)

typedef long long int word;
#define WORD_SIZE   sizeof(word)

enum {
    N_EMPTY = 0,
    N_STRING,
    N_INT,
    N_NAME,
    N_LIST,
    N_CALL,
    N_DEF,
    N_EXTERN,
    N_AUTO,
    N_ASSIGN,
    N_BINARY,
    N_RETURN,
    N_IF,
    N_WHILE,
};

struct node {
    int type;
    int id;
};

struct string_node {
    int type;
    int id;
    
    const char *val;
};

struct int_node {
    int type;
    int id;

    word val;
};

struct name_node {
    int type;
    int id;
    
    const char *val;
};

struct list_node {
    int type;
    int id;

    struct node *val;
    struct list_node *next;
};

struct call_node {
    int type;
    int id;

    struct node *name;
    struct node *args;
};

struct def_node {
    int type;
    int id;

    struct node *name;
    struct node *args;
    struct node *decls;
    struct node *body;

    /* for codegen */
    word stack_size;
};

struct extern_node {
    int type;
    int id;
    
    const char *val;
};

struct auto_node {
    int type;
    int id;
    
    const char *val;
    struct node *init;

    /* for codegen */
    word offset;    
};

struct assign_node {
    int type;
    int id;
    
    struct node *left;
    struct node *right;
};

struct binary_node {
    int type;
    int id;

    int op;
    
    struct node *left;
    struct node *right;
};

struct return_node {
    int type;
    int id;
    
    struct node *val;
};

struct if_node {
    int type;
    int id;
    
    struct node *cond;
    struct node *truee;
    struct node *falsee;
};

struct while_node {
    int type;
    int id;
    
    struct node *cond;
    struct node *body;
};

struct node *empty(void);

struct node *string(const char *s);
struct node *integer(word w);
struct node *namen(const char *s);

struct node *list(struct node *e);
struct node *listback(struct node *l, struct node *n);
struct node *listfront(struct node *l, struct node *n);
struct node *decl(struct node *n, struct node *i);

struct node *call(struct node *name, struct node *args);
struct node *def(struct node *name, struct node *args, struct node *body);

struct node *externn(const char *s);
struct node *auton(const char *s);

struct node *assignn(struct node *l, struct node *r);
struct node *binaryn(int op, struct node *l, struct node *r);

struct node *returnnn(struct node *v);
struct node *ifn(struct node *c, struct node *t, struct node *f);
struct node *whilen(struct node *c, struct node *b);

#endif /* AST_H */
