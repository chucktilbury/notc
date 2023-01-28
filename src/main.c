
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ast.h"
#include "scanner.h"
#include "parser.h"
#include "cmdline.h"
#include "dump.h"

int errors = 0;

void syntax_error(const char* fmt, ...) {

    if(get_line_no() > 0)
        fprintf(stderr, "%s:%d:%d syntax error, ",
                get_file_name(), get_line_no(), get_col_no());
    else
        fprintf(stderr, "syntax error, ");

    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    errors++;
}

void __ferror(const char* func, int line, const char* fmt, ...) {

    fprintf(stderr, "%s:%d: fatal error, ", func, line);

    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    errors++;
    exit(1);
}

/**
 * @file main.c
 *
 * @brief Main entry point to the program.
 *
 */
int main(int argc, char** argv) {

    initCmdLine(CL_FL_ONE,
                "This is the assembler. It reads the assembly language input\n"
                "and converts it to a binary for use by the virtual machine.\n");
    addStrParam("-o", "ofile", "output file name", "output.bin", CL_REQD);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "show the help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    resetCLFileList();
    const char* name = iterateCLFileList();
    if(name != NULL && strlen(name) > 0)
        open_file(name);
    else
        showUseCmdLine();

    if(notc_parse()) {
        printf("parse fail: %d error(s)\n", errors);
        return 1;
    }

    if(errors == 0)
        dumpModule(module);

    destroyCmdLine();

    return 0;
}
