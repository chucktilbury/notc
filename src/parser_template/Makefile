
TARGETS	=	parser.c parser.h ast.c ast.h
TMPS	=	tmp.c tmp.h tmp.output

all: $(TARGETS)

$(TARGETS): $(TMPS)
	python mk_parser.py

$(TMPS) : tmp.y
	bison -tdvo tmp.c tmp.y

clean:
	-rm -f $(TARGETS) $(TMPS)