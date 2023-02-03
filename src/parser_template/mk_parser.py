# This small program parses a "working" bison parser and converts it into a
# scaffold for creating a hand-written RD parser from it. No attempt is made
# to actually create a working parser, so the output will require extensive
# changes to make it actually work.
import sys, os, re
from pprint import pprint as pp

def mk_camel(name):
    return ''.join([x.capitalize() for x in name.split('_')])

def get_rules(fp) :

    lst = []
    for line in fp:
        if len(line) > 0:
            tmp = line.strip().split()
            if len(tmp) and tmp[0] == ";":
                break
            else:
                lst.append(line)
    return lst


def get_func_lst(fp) :

    lines = []

    for line in fp:
        line = line.strip()
        if line == "%%":
            for line in fp:
                s = re.search(r"^[a-z_]+$", line)
                if s:
                    tmp = {}
                    tmp['name'] = s.group(0)
                    tmp['rules'] = get_rules(fp)
                    tmp['camel'] = mk_camel(s.group(0))
                    tmp['ast_type'] = 'AST_'+s.group(0).upper()
                    lines.append(tmp)
                if line == "%%":
                    break;
    return lines

if __name__ == "__main__":

    flst = {}
    with open("tmp.y", "r") as fp :
        flst = get_func_lst(fp)
        #pp(flst)

    with open("parser.c", "w") as fp :
        for tmp in flst:
            fp.write("%s\n"%(tmp['name']))

    with open("parser.h", "w") as fp :
        for tmp in flst:
            fp.write("%s\n"%(tmp['name']))

    with open("ast.c", "w") as fp :
        fp.write("\n#include \"ast.h\"\n\n")
        fp.write("void init_ast(Ast* ast, AstType type) {\n")
        fp.write("    ast->type = type;\n")
        fp.write("}\n\n")

    with open("ast.h", "w") as fp :
        fp.write("#ifndef _AST_H\n")
        fp.write("#define _AST_H\n\n")

        fp.write("typedef enum {\n")
        for tmp in flst:
            fp.write("    %s,\n"%(tmp['ast_type']))
        fp.write("} AstType;\n\n")

        fp.write("typedef struct {\n")
        fp.write("    AstType type;\n")
        fp.write("} Ast;\n\n")

        fp.write("void init_ast(Ast* ast, AstType type);\n\n")

        fp.write("#endif\n")
