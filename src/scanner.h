#ifndef _SCANNER_H
#define _SCANNER_H

typedef enum {
    // dummy token, just in case I want to use the low 8 bits.
    TOK_FIRST = 0xFF,

    // Keywords
    TOK_BREAK,      // "break"
    TOK_CONTINUE,   // "continue"
    TOK_CONST,      // "const"
    TOK_IMPORT,     // "import"
    TOK_DO,         // "do"
    TOK_ELSE,       // "else"
    TOK_IF,         // "if"
    TOK_RETURN,     // "return"
    TOK_WHILE,      // "while"
    TOK_TRUE,       // "true"
    TOK_FALSE,      // "false"
    TOK_YIELD,      // "yield"
    TOK_EXIT,       // "exit"
    TOK_STRUCT,     // "struct"
    TOK_TRACE,      // "trace"
    TOK_PRINT,      // "print"
    TOK_TYPE,       // "type"

    // Multi-keyword tokens
    TOK_INT,        // "int"|"integer"
    TOK_UINT,       // "uint"|"unsigned"
    TOK_NOTHING,    // "noth"|"nothing"
    TOK_STRING,     // "strg"|"string"
    TOK_BOOLEAN,    // "bool"|"boolean"
    TOK_FLOAT,      // "float"

    // Multi-character operators
    TOK_LORE,       // "<="
    TOK_GORE,       // ">="
    TOK_EQU,        // "=="|"equ"
    TOK_NEQU,       // "!="|"neq"
    TOK_OR,         // "or"
    TOK_AND,        // "and"
    TOK_ADD_ASSIGN, // "+="
    TOK_SUB_ASSIGN, // "-="
    TOK_MUL_ASSIGN, // "*="
    TOK_DIV_ASSIGN, // "/="
    TOK_MOD_ASSIGN, // "%="
    TOK_NOT,        // not

    // Single character operators
    TOK_ADD,        // "+"
    TOK_SUB,        // "-"
    TOK_MUL,        // "*"
    TOK_DIV,        // "/"
    TOK_MOD,        // "%"
    TOK_EQUAL,      // "="
    TOK_OPAREN,     // "("
    TOK_CPAREN,     // ")"
    TOK_OPBRACE,    // "<"
    TOK_CPBRACE,    // ">"
    TOK_OSBRACE,    // "["
    TOK_CSBRACE,    // "]"
    TOK_OCBRACE,    // "{"
    TOK_CCBRACE,    // "}"

    TOK_COMMA,      // ","
    TOK_DOT,        // "."

    // Generated tokens
    TOK_SYMBOL,     //  [a-zA-Z_][a-zA-Z_0-9]*
    TOK_INUM,       // [0-9]+
    TOK_FNUM,       // ([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)?
    TOK_UNUM,       // 0[Xx][0-9a-fA-F]+
    TOK_QSTRG,      // quoted string

    TOK_END_INPUT,
    TOK_ERROR,      // inform the parser that the token is no good
} TokenType;

#include "strptr.h"

typedef struct {
    TokenType type;
    String str;
} Token;

void init_scanner(const char* fname);
const char* tokToStr(TokenType tt);
Token* crnt_token();
void consume_token();

#endif /* _SCANNER_H */
