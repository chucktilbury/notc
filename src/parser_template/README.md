# PARSER

This is a bison parser for the language that the hand-coded TDRD parser implementes. It exists to use as a guide while writing the hand-coded parser and to make sure that the language stays "regular".

To test this parser use the following command line:

bison -tvdo parser.c parser.y
