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

%token BREAK CASE CONTINUE CONST DEFAULT
%token DO ELSE IF RETURN SWITCH IMPORT
%token TRUE FALSE YIELD EXIT STRUCT WHILE
%token EQU NEQU LORE GORE OR AND NOT INCREMENT DECREMENT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token UINT INT FLOAT NOTHING STRING BOOLEAN
%token PRINT TRACE TYPE

%token<type> TYPE_NAME
%token<symbol> SYMBOL
%token<fnum> FNUM
%token<inum> INUM
%token<unum> UNUM
%token<str> STRG

%define parse.error verbose
%locations

%right '='
%right ADD_ASSIGN SUB_ASSIGN
%right MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%left CAST
%left OR
%left AND
%left EQU NEQU
%left LORE GORE '<' '>'
%left '+' '-'
%left '*' '/' '%'
%left NEGATE

%%

module
    : module_list {}
    ;

module_list
    : module_item {}
    | module_list module_item {}
    ;

module_item
    : symbol_decl {}
    | func_definition {}
    | IMPORT STRG {}
    | error {}
    ;

type_definition
    : UINT {}
    | INT {}
    | FLOAT {}
    | STRING {}
    | BOOLEAN {}
    ;

type_spec
    : type_definition {}
    | CONST type_definition {}
    | NOTHING {}
    ;

cast_spec
    : '(' type_spec ')' {}
    ;

constant_expression
    : UNUM {}
    | FNUM {}
    | INUM {}
    | STRG {}
    | TRUE {}
    | FALSE {}
    ;

symbol_intro
    : type_spec SYMBOL {}
    ;

symbol_decl
    : symbol_intro {}
    | symbol_intro '=' expression {}
    ;

symbol_intro_list
    : symbol_intro {}
    | symbol_intro_list ',' symbol_intro {}
    ;

expression_factor
    : constant_expression {}
    | SYMBOL {}
    | function_reference {}
    ;

expression
    : expression_factor {}
    | expression '+' expression {}
    | expression '-' expression {}
    | expression '*' expression {}
    | expression '/' expression {}
    | expression '%' expression {}
    | expression EQU expression {}
    | expression NEQU expression {}
    | expression LORE expression {}
    | expression GORE expression {}
    | expression OR expression {}
    | expression AND expression {}
    | expression '<' expression {}
    | expression '>' expression {}
    | expression ADD_ASSIGN expression {}
    | expression SUB_ASSIGN expression {}
    | expression MUL_ASSIGN expression {}
    | expression DIV_ASSIGN expression {}
    | expression MOD_ASSIGN expression {}
    | '-' expression %prec NEGATE {}
    | NOT expression %prec NEGATE {}
    | cast_spec expression %prec CAST {}
    | '(' expression ')' {}
    | error {}
    ;

expression_list
    : expression {}
    | expression_list ',' expression {}
    ;

function_reference
    : SYMBOL '(' expression_list ')' {}
    | SYMBOL '(' ')' {}
    ;

func_decl_parameter_list
    : '(' symbol_intro_list ')' {}
    | '(' ')' {}
    ;

func_definition
    : type_spec SYMBOL func_decl_parameter_list func_body {}
    ;

func_body_statement_list
    : func_body_statement {}
    | func_body_statement_list func_body_statement {}
    ;

func_body
    : '{' func_body_statement_list '}' {}
    | '{' '}' {}
    ;

else_clause
    : ELSE '(' expression ')' func_body {}
    ;

else_clause_final
    : {}
    | ELSE '(' ')' func_body {}
    | ELSE func_body {}
    ;

else_clause_list
    : else_clause {}
    | else_clause_list else_clause {}
    ;

    /* expression stacks? */
if_clause
    : IF '(' expression ')' func_body {}
    ;

if_statement
    : if_clause else_clause_final {}
    | if_clause else_clause_list else_clause_final {}
    | error {}
    ;

while_statement
    : WHILE '(' expression ')' func_body {}
    | WHILE '(' ')' func_body {}
    | WHILE func_body {}
    ;

do_statement
    : DO func_body WHILE '(' expression ')' {}
    | DO func_body WHILE '(' ')' {}
    | DO func_body WHILE {}
    ;

break_statement
    : BREAK {}
    ;

continue_statement
    : CONTINUE {}
    ;

yield_statement
    : YIELD {}
    | YIELD '(' expression ')' {}
    ;

trace_statement
    : TRACE {}
    | TRACE '(' ')' {}
    | TRACE '(' STRG ')' {}
    ;

return_statement
    : RETURN {}
    | RETURN '(' ')' {}
    | RETURN '(' expression ')' {}
    ;

print_statement
    : PRINT {}
    | PRINT '(' ')' {}
    | PRINT '(' expression ')' {}
    ;

exit_statement
    : EXIT {}
    | EXIT '(' expression ')' {}
    ;

type_statement
    : TYPE '(' expression ')' {}
    ;

assignment
    : SYMBOL '=' expression {}
    ;

func_body_statement
    : symbol_decl {}
    | assignment {}
    | if_statement {}
    | while_statement {}
    | do_statement {}
    | function_reference {}
    | trace_statement {}
    | return_statement {}
    | print_statement {}
    | exit_statement {}
    | type_statement {}
    | break_statement {}
    | continue_statement {}
    | yield_statement {}
    | func_body {}
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
