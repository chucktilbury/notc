%debug
%defines
%locations

%{

#include <stdio.h>

#include "scanner.h"
#include "memory.h"
#include "ast.h"
#include "import.h"

Expression* tmp_expr;

//extern const char* file_name;//
//extern Module* module;

%}

%union {
    char* str;
    char* symbol;
    double fnum;
    uint64_t unum;
    int64_t inum;
    int type;

    /* Union objects for the AST. */
    void* ptr;
    Module* mod;
    ModuleList* modlst;
    TypeDefinition* typdef;
    ConstantExpression* conexp;
    SymbolIntro* symint;
    SymbolIntroList* symintlis;
    Expression* exp;
    ExpressionList* explis;
    ExpressionFactor* expfac;
    FunctionReference* funref;

    TraceStatement* trasta;
    SingleStatement* sinsta;

    FunctionDefinition* fundef;
    FuncBodyStatementList* funbodstalis;

    WhileStatement* whista;
    DoStatement* dosta;

    IfStatement* ifsta;
    ElseClause* elscla;
    ElseClauseList* elsclalis;
    Assignment* ass;
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

    /* token list automatically generated by boilerplate */
%type<ptr> module_item func_body_statement
%type<modlst> module_list
%type<typdef> type_definition type_spec cast_spec
%type<conexp> constant_expression
%type<symint> symbol_intro symbol_decl
%type<exp> expression
%type<symintlis> symbol_intro_list func_decl_parameter_list
%type<expfac> expression_factor
%type<explis> expression_list
%type<funref> function_reference
%type<trasta> trace_statement
%type<sinsta> return_statement print_statement exit_statement type_statement
%type<sinsta> break_statement continue_statement yield_statement
%type<fundef> func_definition
%type<funbodstalis> func_body_statement_list func_body
%type<whista> while_statement
%type<dosta> do_statement
%type<ifsta> if_statement if_clause
%type<elscla> else_clause else_clause_final
%type<elsclalis> else_clause_list
%type<ass> assignment

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
    : module_list {
            module = createModule($1);
        }
    ;

module_list
    : module_item {
            $$ = createModuleList();
            addModuleList($$, $1);
        }
    | module_list module_item {
            addModuleList($1, $2);
        }
    ;

module_item
    : symbol_decl {
            $$ = $1;
        }
    | func_definition { $$ = $1; }
    | IMPORT STRG {
            doImport($2);
            $$ = NULL;
        }
    | error {}
    ;

type_definition
    : UINT {
            $$ = createTypeDefinition(UINT);
        }
    | INT {
            $$ = createTypeDefinition(INT);
        }
    | FLOAT {
            $$ = createTypeDefinition(FLOAT);
        }
    | STRING {
            $$ = createTypeDefinition(STRING);
        }
    | BOOLEAN {
            $$ = createTypeDefinition(BOOLEAN);
        }
    ;

type_spec
    : type_definition {
            $$ = $1;
        }
    | CONST type_definition {
            $2->is_const = true;
            $$ = $2;
        }
    | NOTHING {
            $$ = createTypeDefinition(NOTHING);
        }
    ;

cast_spec
    : '(' type_spec ')' {
            $$ = $2;
        }
    ;

constant_expression
    : UNUM {
            $$ = createUNUMConstantExpression($1);
        }
    | FNUM {
            $$ = createFNUMConstantExpression($1);
        }
    | INUM {
            $$ = createINUMConstantExpression($1);
        }
    | STRG {
            $$ = createSTRConstantExpression($1);
        }
    | TRUE {
            $$ = createBOOLConstantExpression(true);
        }
    | FALSE {
            $$ = createBOOLConstantExpression(false);
        }
    ;

symbol_intro
    : type_spec SYMBOL {
            $$ = createSymbolIntro($1, $2);
        }
    ;

symbol_decl
    : symbol_intro {
            $$ = $1;
        }
    | symbol_intro '=' {
            tmp_expr = createExpression();
        } expression {
            $$->assign = tmp_expr;
            $$ = $1;
        }
    ;

symbol_intro_list
    : symbol_intro {
            $$ = createSymbolIntroList();
            addSymbolIntroList($$, $1);
        }
    | symbol_intro_list ',' symbol_intro {
            addSymbolIntroList($1, $3);
        }
    ;

expression_factor
    : constant_expression {
            $$ = createExpressionFactor(AST_CONSTANT_EXPRESSION, (void*)$1);
        }
    | SYMBOL {
            $$ = createExpressionFactor(AST_SYMBOL, (void*)$1);
        }
    | function_reference {
            $$ = createExpressionFactor(AST_FUNCTION_REFERENCE, (void*)$1);
        }
    ;

expression
    : expression_factor {
            addExpression(tmp_expr, $1);
        }
    | expression '+' expression {
            addExpression(tmp_expr, createOperator(OP_ADD));
        }
    | expression '-' expression {
            addExpression(tmp_expr, createOperator(OP_SUB));
        }
    | expression '*' expression {
            addExpression(tmp_expr, createOperator(OP_MUL));
        }
    | expression '/' expression {
            addExpression(tmp_expr, createOperator(OP_DIV));
        }
    | expression '%' expression {
            addExpression(tmp_expr, createOperator(OP_MOD));
        }
    | expression EQU expression {
            addExpression(tmp_expr, createOperator(OP_EQU));
        }
    | expression NEQU expression {
            addExpression(tmp_expr, createOperator(OP_NEQU));
        }
    | expression LORE expression {
            addExpression(tmp_expr, createOperator(OP_LORE));
        }
    | expression GORE expression {
            addExpression(tmp_expr, createOperator(OP_GORE));
        }
    | expression OR expression {
            addExpression(tmp_expr, createOperator(OP_OR));
        }
    | expression AND expression {
            addExpression(tmp_expr, createOperator(OP_AND));
        }
    | expression '<' expression {
            addExpression(tmp_expr, createOperator(OP_LT));
        }
    | expression '>' expression {
            addExpression(tmp_expr, createOperator(OP_GT));
        }
    | expression ADD_ASSIGN expression {
            addExpression(tmp_expr, createOperator(OP_ADD_ASSIGN));
        }
    | expression SUB_ASSIGN expression {
            addExpression(tmp_expr, createOperator(OP_SUB_ASSIGN));
        }
    | expression MUL_ASSIGN expression {
            addExpression(tmp_expr, createOperator(OP_MUL_ASSIGN));
        }
    | expression DIV_ASSIGN expression {
            addExpression(tmp_expr, createOperator(OP_DIV_ASSIGN));
        }
    | expression MOD_ASSIGN expression {
            addExpression(tmp_expr, createOperator(OP_MOD_ASSIGN));
        }
    | '-' expression %prec NEGATE {
            addExpression(tmp_expr, createOperator(OP_UNNEG));
        }
    | NOT expression %prec NEGATE {
            addExpression(tmp_expr, createOperator(OP_UNNOT));
        }
    | cast_spec expression %prec CAST {
        }
    | '(' expression ')' {
            //$$ = $2;
        }
    | error {}
    ;

expression_list
    : expression {
            $$ = createExpressionList();
            addExpressionList($$, $1);
        }
    | expression_list ',' expression {
            addExpressionList($1, $3);
        }
    ;

function_reference
    : SYMBOL '(' expression_list ')' {
            $$ = createFunctionReference($1, $3);
        }
    | SYMBOL '(' ')' {
            $$ = createFunctionReference($1, NULL);
        }
    ;

func_decl_parameter_list
    : '(' symbol_intro_list ')' {
            $$ = $2;
        }
    | '(' ')' {
            $$ = NULL;
        }
    ;

func_definition
    : type_spec SYMBOL func_decl_parameter_list func_body {
            $$ = createFunctionDefinition($1, $2, $3, $4);
        }
    ;

func_body_statement_list
    : func_body_statement {
            $$ = createFuncBodyStatementList();
            addFuncBodyStatementList($$, $1);
        }
    | func_body_statement_list func_body_statement {
            addFuncBodyStatementList($1, $2);
        }
    ;

func_body
    : '{' func_body_statement_list '}' {
            $$ = $2;
        }
    | '{' '}' {
            $$ = NULL;
        }
    ;

else_clause
    : ELSE '('  {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' func_body {
            $$ = createElseClause(tmp_expr, $6);
        }
    ;

else_clause_final
    : {
            $$ = NULL; // empty
        }
    | ELSE '(' ')' func_body {
            $$ = createElseClause(NULL, $4);
        }
    | ELSE func_body {
            $$ = createElseClause(NULL, $2);
        }
    ;

else_clause_list
    : else_clause {
            $$ = createElseClauseList();
            addElseClauseList($$, $1);
        }
    | else_clause_list else_clause {
            //$$ = $1;
            addElseClauseList($1, $2);
        }
    ;

    /* expression stacks? */
if_clause
    : IF '(' {
            //printf("start\n");
            tmp_expr = createExpression();
            $<ifsta>$ = createIfStatement();
        } expression ')' {
            $<ifsta>$->expr = tmp_expr;
        } func_body {
            $<ifsta>$->fbsl = $7;
            //$$ = createIfStatement(tmp_expr, $6, NULL);
        }
    ;

if_statement
    : if_clause else_clause_final {
            $$ = $1;
            if($2 != NULL) {
                $$->list = createElseClauseList();
                addElseClauseList($$->list, $2);
            }
        }
    | if_clause else_clause_list else_clause_final {
            $$ = $1;
            $$->list = $2;
            if($3 != NULL)
                addElseClauseList($$->list, $3);
        }
    | error {}
    ;

while_statement
    : WHILE '(' {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' func_body {
            $$ = createWhileStatement(tmp_expr, $6);
        }
    | WHILE '(' ')' func_body {
            $$ = createWhileStatement(NULL, $4);
        }
    | WHILE func_body {
            $$ = createWhileStatement(NULL, $2);
        }
    ;

do_statement
    : DO func_body WHILE '(' {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createDoStatement(tmp_expr, $2);
        }
    | DO func_body WHILE '(' ')' {
            $$ = createDoStatement(NULL, $2);
        }
    | DO func_body WHILE {
            $$ = createDoStatement(NULL, $2);
        }
    ;

break_statement
    : BREAK {
            $$ = createSingleStatement(BREAK, NULL);
        }
    ;

continue_statement
    : CONTINUE {
            $$ = createSingleStatement(CONTINUE, NULL);
        }
    ;

yield_statement
    : YIELD {
            $$ = createSingleStatement(YIELD, NULL);
        }
    | YIELD '('  {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createSingleStatement(YIELD, tmp_expr);
        }
    ;

trace_statement
    : TRACE {
            $$ = createTraceStatement(NULL);
        }
    | TRACE '(' ')' {
            $$ = createTraceStatement(NULL);
        }
    | TRACE '(' STRG ')' {
            $$ = createTraceStatement($3);
        }
    ;

return_statement
    : RETURN {
            $$ = createSingleStatement(RETURN, NULL);
        }
    | RETURN '(' ')' {
            $$ = createSingleStatement(RETURN, NULL);
        }
    | RETURN '('  {
            printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createSingleStatement(RETURN, tmp_expr);
        }
    ;

print_statement
    : PRINT {
            $$ = createSingleStatement(PRINT, NULL);
        }
    | PRINT '(' ')' {
            $$ = createSingleStatement(PRINT, NULL);
        }
    | PRINT '('  {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createSingleStatement(PRINT, tmp_expr);
        }
    ;

exit_statement
    : EXIT {
            $$ = createSingleStatement(EXIT, NULL);
        }
    | EXIT '('  {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createSingleStatement(EXIT, tmp_expr);
        }
    ;

type_statement
    : TYPE '('  {
            //printf("start\n");
            tmp_expr = createExpression();
        } expression ')' {
            $$ = createSingleStatement(TYPE, tmp_expr);
        }
    ;

assignment
    : SYMBOL '=' {
            tmp_expr = createExpression();
        } expression {
            $$ = createAssignment($1, tmp_expr);
        }
    ;

func_body_statement
    : symbol_decl { $$ = (void*)$1; }
    | assignment { $$ = (void*)$1; }
    | if_statement { $$ = (void*)$1; }
    | while_statement { $$ = (void*)$1; }
    | do_statement { $$ = (void*)$1; }
    | function_reference { $$ = (void*)$1; }
    | trace_statement { $$ = (void*)$1; }
    | return_statement { $$ = (void*)$1; }
    | print_statement { $$ = (void*)$1; }
    | exit_statement { $$ = (void*)$1; }
    | type_statement { $$ = (void*)$1; }
    | break_statement { $$ = (void*)$1; }
    | continue_statement { $$ = (void*)$1; }
    | yield_statement { $$ = (void*)$1; }
    | func_body { $$ = $1; }
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
