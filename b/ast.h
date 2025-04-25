#ifndef AST_H
#define AST_H

#define ASSTR(n) ((struct string_node *)n)
#define ASINT(n) ((struct int_node *)n)
#define ASNAME(n) ((struct name_node *)n)

enum {
    N_EMPTY,
    N_STRING,
    N_INT,
    N_NAME,
    N_LIST,
    N_CALL,
    N_DEF,
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

    int val;
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
    struct node *body;
};

struct node *empty(void);

struct node *string(const char *s);
struct node *integer(int i);
struct node *namen(const char *s);

struct node *list(struct node *e);
struct node *listback(struct node *l, struct node *n);
struct node *listfront(struct node *l, struct node *n);

struct node *call(struct node *name, struct node *args);
struct node *def(struct node *name, struct node *args, struct node *body);

void print(struct node *n, int indent);


#endif /* AST_H */
