# This small program parses a "working" bison parser and converts it into a
# scaffold for creating a hand-written RD parser from it. No attempt is made
# to actually create a working parser, so the output will require extensive
# changes to make it actually work.
import sys, os, re
from pprint import pprint as pp

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

    lines = {}

    for line in fp:
        line = line.strip()
        if line == "%%":
            for line in fp:
                s = re.search(r"^[a-z_]+$", line)
                if s:
                    #lines.append(s.group(0))
                    lines[s.group(0)] = get_rules(fp)
                if line == "%%":
                    break;
    return lines

if __name__ == "__main__":

    flst = {}
    with open("parser.y", "r") as fp :
        flst = get_func_lst(fp)
        pp(flst)

    with open("nonterms.txt", "w") as fp :
        for name in flst:
            fp.write("%s\n"%(name))