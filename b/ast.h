#ifndef AST_H
#define AST_H

#define ASNODE(n) ((struct node *)n)
#define ASSTR(n) ((struct string_node *)n)
#define ASINT(n) ((struct int_node *)n)
#define ASNAME(n) ((struct name_node *)n)
#define ASLIST(n) ((struct list_node *)n)
#define ASEXTERN(n) ((struct extern_node *)n)
#define ASAUTO(n) ((struct auto_node *)n)

typedef unsigned long long int word;
#define WORD_SIZE   8

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
    const char *init;
    word offset;    /* for codegen */
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

void print(struct node *n, int indent);


#endif /* AST_H */
