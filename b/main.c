#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "codegen.h"

extern FILE *yyin;
extern int yyparse(void);

const char *infile;
FILE *out;

extern int yydebug;

int main(int argc, char ** argv) {
    FILE *in;
    const char *outfile = "b.out.s";
    int ret;

    yydebug = 1;

    if (argc != 2) {
        fprintf(stderr, "usage: b <file>\n");
        return 1;
    }

    infile = argv[1];

    in = fopen(infile, "r");
    if (!in) {
        fprintf(stderr, "unable to open `%s`: %s\n", infile, strerror(errno));
        return 1;
    }

    out = fopen(outfile, "w");
    if (!out) {
        fprintf(stderr, "unable to open `%s`: %s\n", outfile, strerror(errno));
        return 1;
    }

    yyin = in;
    ret = yyparse();
    if (ret != 0)
        return 1;

    fclose(in);
    fclose(out);

    return 0;
}


