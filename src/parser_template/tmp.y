%debug
%defines
%locations

%{

#include <stdio.h>
// TODO:
// 1. Structs with LH and RH syntax for references and assignment.
// 2. Init a struct with static data.
// 3. Structs can be nested.
// 4. Syntax for list and hash definition and access with weak types.
// 5. Syntax for string formatting? Revisit?
// 6. Namespaces? Scopes?
//
// Might happen:
// 1. Parsing imports for symbols instead of simple include.
// 2. Use the linker.
//
// Sky Pie:
// 1. Functions defined for structs.
// 2. Non-local GOTO.
//
// To NOT do:
// 1. Exceptions
// 2. Classes
// 3. Function pointers
//

%}

%union {
    char* str;
    char* symbol;
    double fnum;
    uint64_t unum;
    int64_t inum;
    int type;

};

%token BREAK CONTINUE CONST NAMESPACE LIST DICT
%token DO ELSE IF RETURN IMPORT
%token TRUE FALSE YIELD EXIT STRUCT WHILE
%token EQU NEQU LORE GORE OR AND NOT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token UINT INT FLOAT NOTHING STRING BOOLEAN
%token PRINT TRACE TYPE

%token<symbol> SYMBOL
%token<fnum> FNUM
%token<inum> INUM
%token<unum> UNUM
%token<str> STRG

%define parse.error verbose
%locations

%%

module
    : module_list
    ;

module_list
    : module_item
    | module_list module_item
    ;

module_item
    : symbol_decl
    | func_definition
    | struct_definition
    | namespace_definition
    | IMPORT STRG
    ;

namespace_definition
    : NAMESPACE SYMBOL '{' module_list '}'
    ;

array_reference
    : SYMBOL '[' expression ']'
    ;

compound_element
    : SYMBOL
    | array_reference
    ;

compound_name
    : compound_element
    | compound_name '.' compound_element
    ;

type_definition
    : UINT
    | INT
    | FLOAT
    | STRING
    | BOOLEAN
    ;

dict_init_element
    : SYMBOL ':' expression
    ;

dict_list
    : dict_init_element
    | dict_list ',' dict_init_element
    ;

type_spec
    : type_definition
    | CONST type_definition
    | NOTHING
    ;

cast_spec
    : '(' type_spec ')'
    ;

constant_expression
    : UNUM
    | FNUM
    | INUM
    | STRG
    | TRUE
    | FALSE
    ;

symbol_decl
    : type_spec SYMBOL
    | type_spec SYMBOL '=' expression
    | LIST SYMBOL
    | DICT SYMBOL
    | LIST SYMBOL '=' '[' expression_list ']'
    | DICT SYMBOL '=' '[' dict_list ']'
    ;

symbol_decl_list
    : symbol_decl
    | symbol_decl_list ',' symbol_decl
    ;

struct_element
    : symbol_decl_list
    | struct_definition
    ;

struct_list
    : struct_element
    | struct_list struct_element
    ;

struct_definition
    :   STRUCT SYMBOL '{' '}'
    |   STRUCT SYMBOL '{' struct_list '}'
    ;

    /*
     * Expressions are arranged from lowest precidence to the highest. Each
     * precidence level has its own rule that references the next higher
     * level.
     */
expression
    : expr_or
    ;

expr_or
    : expr_and
    | expr_or OR expr_and
    ;

expr_and
    : expr_equality
    | expr_and AND expr_equality
    ;

expr_equality
    : expr_compare
    | expr_equality EQU expr_compare
    | expr_equality NEQU expr_compare
    ;

expr_compare
    : expr_term
    | expr_compare LORE expr_term
    | expr_compare GORE expr_term
    | expr_compare '<' expr_term
    | expr_compare '>' expr_term
    ;

expr_term
    : expr_factor
    | expr_term '+' expr_factor
    | expr_term '-' expr_factor
    ;

expr_factor
    : expr_unary
    | expr_factor '*' expr_unary
    | expr_factor '/' expr_unary
    | expr_factor '%' expr_unary
    ;

expr_unary
    : expr_primary
    | '-' expr_unary
    | '+' expr_unary
    | NOT expr_unary
    | cast_spec expr_unary
    ;

expr_primary
    : constant_expression
    | compound_name
    | function_reference
    | '(' expression ')'
    ;

expression_list
    : expression
    | expression_list ',' expression
    ;

function_reference
    : SYMBOL '(' expression_list ')'
    | SYMBOL '(' ')'
    ;

func_definition
    : type_spec SYMBOL '(' symbol_decl_list ')' func_body
    | type_spec SYMBOL '(' ')' func_body
    | type_spec SYMBOL func_body
    ;

func_body_statement_list
    : func_body_statement
    | func_body_statement_list func_body_statement
    ;

func_body
    : '{' func_body_statement_list '}'
    | '{' '}'
    ;

    /*
     * Note that the final else clause would be syntattically determined instead
     * of symantically if this were a bison parser design.
     */
else_clause
    : ELSE '(' expression ')' func_body
    | ELSE '(' ')' func_body
    | ELSE func_body
    ;

else_clause_list
    : else_clause
    | else_clause_list else_clause
    ;

    /* expression stacks? */
if_clause
    : IF '(' expression ')' func_body
    ;

if_statement
    : if_clause
    | if_clause else_clause_list
    ;

while_statement
    : WHILE '(' expression ')' func_body
    | WHILE '(' ')' func_body
    | WHILE func_body
    ;

do_statement
    : DO func_body WHILE '(' expression ')'
    | DO func_body WHILE '(' ')'
    | DO func_body WHILE
    ;

break_statement
    : BREAK
    ;

continue_statement
    : CONTINUE
    ;

yield_statement
    : YIELD
    | YIELD '(' expression ')'
    ;

trace_statement
    : TRACE
    | TRACE '(' ')'
    | TRACE '(' STRG ')'
    ;

return_statement
    : RETURN
    | RETURN '(' ')'
    | RETURN '(' expression ')'
    ;

print_statement
    : PRINT
    | PRINT '(' ')'
    | PRINT '(' expression ')'
    ;

exit_statement
    : EXIT
    | EXIT '(' expression ')'
    ;

type_statement
    : TYPE '(' expression ')'
    ;

assignment
    : compound_name '=' expression
    | compound_name ADD_ASSIGN expression
    | compound_name SUB_ASSIGN expression
    | compound_name MUL_ASSIGN expression
    | compound_name DIV_ASSIGN expression
    | compound_name MOD_ASSIGN expression
    ;

func_body_statement
    : symbol_decl
    | assignment
    | if_statement
    | while_statement
    | do_statement
    | function_reference
    | trace_statement
    | return_statement
    | print_statement
    | exit_statement
    | type_statement
    | break_statement
    | continue_statement
    | yield_statement
    | struct_definition
    | func_body
    ;

%%

extern int errors;
void yyerror(const char* s) {

    fprintf(stderr, "%s:%d:%d %s\n",
            get_file_name(), get_line_no(), get_col_no(), s);
    errors++;
}

const char* tokenToStr(int tok) {

    return yysymbol_name(YYTRANSLATE(tok));
}
