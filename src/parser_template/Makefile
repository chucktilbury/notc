
TARGET 	=	parser.c
OTHER	=	parser.h ast.c ast.h
TMPS	=	tmp.c tmp.h tmp.output

all: $(TARGET)

$(TARGET): $(TMPS)
	python mk_parser.py

$(TMPS) : tmp.y
	bison -Wcounterexamples -tdvo tmp.c tmp.y
	flex -o scanner.c scanner.l
	gcc -Wall -Wextra -o parse main.c scanner.c tmp.c -Wno-sign-compare -Wno-unused-variable

clean:
	-rm -f $(TARGET) $(TMPS) $(OTHER) scanner.c parse