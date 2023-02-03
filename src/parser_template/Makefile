
TARGET 	=	parser.c
OTHER	=	parser.h ast.c ast.h
TMPS	=	tmp.c tmp.h tmp.output

all: $(TARGET)

$(TARGET): $(TMPS)
	python mk_parser.py

$(TMPS) : tmp.y
	bison -tdvo tmp.c tmp.y

clean:
	-rm -f $(TARGET) $(TMPS) $(OTHER)