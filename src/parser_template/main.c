
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "tmp.h"
#include "scanner.h"

extern int errors;

int main(int argc, char** argv) {

    if(argc < 2) {
        if(isatty(fileno(stdin))) {
            fprintf(stderr, "Nothing to input!\n");
            return 1;
        }
    }
    else {
        open_file(argv[1]);
    }

    yyparse();

    printf("\nerrors: %d\n", errors);
    return 0;
}
