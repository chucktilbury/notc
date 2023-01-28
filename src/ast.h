#ifndef _AST_H_
#define _AST_H_

#include <stdbool.h>
#include <stdint.h>

#include "ptrlst.h"

typedef enum {
    AST_MODULE,
    AST_MODULE_LIST,
    AST_TYPE_DEFINITION,
    AST_TYPE_SPEC,
    AST_CAST_SPEC,
    AST_CONSTANT_EXPRESSION,
    AST_SYMBOL_INTRO,
    AST_SYMBOL_DECL,
    AST_SYMBOL_INTRO_LIST,
    AST_EXPRESSION_FACTOR,
    AST_EXPRESSION,
    AST_EXPRESSION_LIST,
    AST_FUNCTION_REFERENCE,
    AST_FUNC_DECL_PARAMETER_LIST,
    AST_FUNC_DEFINITION,
    AST_FUNC_BODY_STATEMENT_LIST,
    AST_FUNC_BODY,
    AST_BREAK_STATEMENT,
    AST_CONTINUE_STATEMENT,
    AST_YIELD_STATEMENT,
    AST_ELSE_CLAUSE,
    AST_ELSE_CLAUSE_INTERMEDIATE_LIST,
    AST_ELSE_CLAUSE_FINAL,
    AST_ELSE_CLAUSE_LIST,
    AST_IF_CLAUSE,
    AST_IF_STATEMENT,
    AST_WHILE_STATEMENT,
    AST_DO_STATEMENT,
    AST_MODULE_ITEM,
    AST_TRACE_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_PRINT_STATEMENT,
    AST_EXIT_STATEMENT,
    AST_TYPE_STATEMENT,
    AST_FUNC_BODY_STATEMENT,
    AST_OPERATOR,
    AST_SINGLE_STATEMENT,
    AST_ASSIGNMENT,
    AST_SYMBOL,
} AstType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_UNNEG,
    OP_UNNOT,
    OP_EQU,
    OP_NEQU,
    OP_LORE,
    OP_GORE,
    OP_LT,
    OP_GT,
    OP_AND,
    OP_OR,
    OP_ADD_ASSIGN,
    OP_SUB_ASSIGN,
    OP_MUL_ASSIGN,
    OP_DIV_ASSIGN,
    OP_MOD_ASSIGN,
} OperatorType;

typedef enum {
    EXPR_FACT_CONST,
    EXPR_FACT_SYM,
    EXPR_FACT_FUNC_REF,
} ExpressionFactorType;

typedef struct {
    AstType type;
    /*
    const char* fname;
    int line;
    int col;
    */
} Ast;

typedef struct {
    Ast ast;
    int type;
    union {
        uint64_t unum;
        int64_t inum;
        double fnum;
        char* str;
        bool bval;
    } data;
} ConstantExpression;

typedef struct {
    Ast ast;
    OperatorType type;
} Operator;

typedef struct {
    Ast ast;
    AstType type;
    void* item;
} ExpressionFactor;

typedef struct {
    Ast ast;
    int type;
    bool is_const;
} TypeDefinition;

typedef struct {
    Ast ast;
    PtrLst* list;
} Expression;

typedef struct {
    Ast ast;
    PtrLst* list;
} ExpressionList;

typedef struct {
    Ast ast;
    const char* name;
    ExpressionList* list;
} FunctionReference;

typedef struct {
    Ast ast;
    TypeDefinition* type;
    const char* str;
    Expression* assign;
} SymbolIntro;

typedef struct {
    Ast ast;
    int type;
    Expression* expr;
} SingleStatement;

typedef struct {
    Ast ast;
    const char* str;
} TraceStatement;

typedef struct {
    Ast ast;
    PtrLst* list;
} SymbolIntroList;

typedef struct {
    Ast ast;
    PtrLst* list;
} ModuleList;

typedef struct {
    Ast ast;
    ModuleList* items;
} Module;

typedef struct {
    Ast ast;
    PtrLst* list;
} FuncBodyStatementList;

typedef struct {
    Ast ast;
    TypeDefinition* type;
    const char* symbol;
    SymbolIntroList* sil;
    FuncBodyStatementList* fbsl;
} FunctionDefinition;

typedef struct {
    Ast ast;
    Expression* expr;
    FuncBodyStatementList* fbsl;
} WhileStatement;

typedef struct {
    Ast ast;
    Expression* expr;
    FuncBodyStatementList* fbsl;
} DoStatement;

typedef struct {
    Ast ast;
    PtrLst* list;
} ElseClauseList;

typedef struct {
    Ast ast;
    Expression* expr;
    FuncBodyStatementList* fbsl;
    ElseClauseList* list;
} IfStatement;

typedef struct {
    Ast ast;
    Expression* expr;
    FuncBodyStatementList* fbsl;
} ElseClause;

typedef struct {
    Ast ast;
    const char* name;
    Expression* expr;
} Assignment;

void initAst(Ast* ast, AstType type);
Module* createModule(ModuleList* ml);
ModuleList* createModuleList();
void addModuleList(ModuleList* lst, void* item);
TypeDefinition* createTypeDefinition(int type);
ConstantExpression* createUNUMConstantExpression(const uint64_t val);
ConstantExpression* createINUMConstantExpression(const int64_t val);
ConstantExpression* createFNUMConstantExpression(const double val);
ConstantExpression* createSTRConstantExpression(const char* val);
ConstantExpression* createBOOLConstantExpression(const bool val);
SymbolIntro* createSymbolIntro(TypeDefinition* type, const char* symbol);
SymbolIntroList* createSymbolIntroList();
void addSymbolIntroList(SymbolIntroList* ptr, void* item);
Expression* createExpression();
void addExpression(Expression* ptr, void* item);
ExpressionList* createExpressionList();
void addExpressionList(ExpressionList* ptr, void* item);
Operator* createOperator(OperatorType type);
ExpressionFactor* createExpressionFactor(AstType type, void* item);
FunctionReference* createFunctionReference(const char* name,
                                           ExpressionList* list);
SingleStatement* createSingleStatement(int type, Expression* item);
TraceStatement* createTraceStatement(const char* msg);
FunctionDefinition* createFunctionDefinition(TypeDefinition* type,
                                             const char* symbol,
                                             SymbolIntroList* sil,
                                             FuncBodyStatementList* fbsl);
FuncBodyStatementList* createFuncBodyStatementList();
void addFuncBodyStatementList(FuncBodyStatementList* ptr, void* item);

WhileStatement* createWhileStatement(Expression* expr,
                                     FuncBodyStatementList* fbsl);
DoStatement* createDoStatement(Expression* expr,
                               FuncBodyStatementList* fbsl);

IfStatement* createIfStatement(); /*Expression* expr,
                               FuncBodyStatementList* fbsl,
                               ElseClauseList* list); */

ElseClause* createElseClause(Expression* expr,
                             FuncBodyStatementList* fbsl);
ElseClauseList* createElseClauseList();
void addElseClauseList(ElseClauseList* ptr, void* item);

Assignment* createAssignment(const char* name, Expression* expr);

extern Module* module;


#endif
