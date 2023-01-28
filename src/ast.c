
#include "ast.h"
#include "memory.h"
#include "scanner.h"
#include "parser.h"

#ifdef ENABLE_PARSER_TRACE
#define TRACE \
    printf("%s() %s:%d:%d\n", __func__, \
            get_file_name(), get_line_no(), get_col_no());
//#define TRACE   printf("%s()\n", __func__);
#else
#define TRACE
#endif

Module* module = NULL;

void initAst(Ast* ast, AstType type) {

    /*
    ast->fname = _copy_str(get_file_name());
    ast->line = get_line_no();
    ast->col = get_col_no();
    */
    ast->type = type;
}

Module* createModule(ModuleList* ml) {

    TRACE
    Module* mod = _alloc_ds(Module);
    initAst(&mod->ast, AST_MODULE);
    mod->items = ml;

    return mod;
}

ModuleList* createModuleList() {

    TRACE
    ModuleList* ptr = _alloc_ds(ModuleList);
    initAst(&ptr->ast, AST_MODULE_LIST);
    ptr->list = createPtrLst();

    return ptr;
}

void addModuleList(ModuleList* ptr, void* item) {

    TRACE
    if(item != NULL) {
        //printf("add item: %d\n", ((Ast*)item)->type);
        appendPtrLst(ptr->list, item);
    }
}

TypeDefinition* createTypeDefinition(int type) {

    TRACE
    TypeDefinition* ptr = _alloc_ds(TypeDefinition);
    initAst(&ptr->ast, AST_TYPE_DEFINITION);
    ptr->type = type;
    ptr->is_const = false;

    return ptr;
}

ConstantExpression* createUNUMConstantExpression(const uint64_t val) {

    TRACE
    ConstantExpression* ptr = _alloc_ds(ConstantExpression);
    initAst(&ptr->ast, AST_CONSTANT_EXPRESSION);
    ptr->type = UINT;
    ptr->data.unum = val;

    return ptr;
}

ConstantExpression* createINUMConstantExpression(const int64_t val) {

    TRACE
    ConstantExpression* ptr = _alloc_ds(ConstantExpression);
    initAst(&ptr->ast, AST_CONSTANT_EXPRESSION);
    ptr->type = INT;
    ptr->data.inum = val;

    return ptr;
}

ConstantExpression* createFNUMConstantExpression(const double val) {

    TRACE
    ConstantExpression* ptr = _alloc_ds(ConstantExpression);
    initAst(&ptr->ast, AST_CONSTANT_EXPRESSION);
    ptr->type = FLOAT;
    ptr->data.fnum = val;

    return ptr;
}

ConstantExpression* createSTRConstantExpression(const char* val) {

    TRACE
    ConstantExpression* ptr = _alloc_ds(ConstantExpression);
    initAst(&ptr->ast, AST_CONSTANT_EXPRESSION);
    ptr->type = STRG;
    ptr->data.str = _copy_str(val);

    return ptr;
}

ConstantExpression* createBOOLConstantExpression(const bool val) {

    TRACE
    ConstantExpression* ptr = _alloc_ds(ConstantExpression);
    initAst(&ptr->ast, AST_CONSTANT_EXPRESSION);
    ptr->type = BOOLEAN;
    ptr->data.bval = val;

    return ptr;
}

SymbolIntro* createSymbolIntro(TypeDefinition* type, const char* symbol) {

    TRACE
    SymbolIntro* ptr = _alloc_ds(SymbolIntro);
    initAst(&ptr->ast, AST_SYMBOL_INTRO);
    ptr->type = type;
    ptr->str = _copy_str(symbol);
    ptr->assign = NULL;

    return ptr;
}

Expression* createExpression() {

    TRACE
    Expression* ptr = _alloc_ds(Expression);
    initAst(&ptr->ast, AST_EXPRESSION);
    ptr->list = createPtrLst();

    return ptr;
}

void addExpression(Expression* ptr, void* item) {

    TRACE
    //printf("ptr = %p\n", ptr);
    appendPtrLst(ptr->list, item);
}

ExpressionList* createExpressionList() {

    TRACE
    ExpressionList* ptr = _alloc_ds(ExpressionList);
    initAst(&ptr->ast, AST_EXPRESSION_LIST);
    ptr->list = createPtrLst();

    return ptr;
}

void addExpressionList(ExpressionList* ptr, void* item) {

    TRACE
    //printf("ptr = %p\n", ptr);
    appendPtrLst(ptr->list, item);
}

FunctionReference* createFunctionReference(const char* name,
                                           ExpressionList* list) {

    TRACE
    FunctionReference* ptr = _alloc_ds(FunctionReference);
    initAst(&ptr->ast, AST_FUNCTION_REFERENCE);
    ptr->name = _copy_str(name);
    ptr->list = list;

    return ptr;
}

SymbolIntroList* createSymbolIntroList() {

    TRACE
    SymbolIntroList* ptr = _alloc_ds(SymbolIntroList);
    initAst(&ptr->ast, AST_SYMBOL_INTRO_LIST);
    ptr->list = NULL;

    return ptr;
}

void addSymbolIntroList(SymbolIntroList* ptr, void* item) {

    TRACE
    appendPtrLst(ptr->list, item);
}

Operator* createOperator(OperatorType type) {

    TRACE
    Operator* ptr = _alloc_ds(Operator);
    initAst(&ptr->ast, AST_OPERATOR);
    ptr->type = type;

    return ptr;
}

ExpressionFactor* createExpressionFactor(AstType type, void* item) {

    TRACE
    ExpressionFactor* ptr = _alloc_ds(ExpressionFactor);
    initAst(&ptr->ast, AST_EXPRESSION_FACTOR);
    ptr->type = type;
    ptr->item = item;

    return ptr;
}

SingleStatement* createSingleStatement(int type, Expression* item) {

    TRACE
    SingleStatement* ptr = _alloc_ds(SingleStatement);
    initAst(&ptr->ast, AST_SINGLE_STATEMENT);
    ptr->type = type;
    ptr->expr = item;

    return ptr;
}

TraceStatement* createTraceStatement(const char* msg) {

    TRACE
    TraceStatement* ptr = _alloc_ds(TraceStatement);
    initAst(&ptr->ast, AST_TRACE_STATEMENT);
    ptr->str = msg;

    return ptr;
}

FunctionDefinition* createFunctionDefinition(TypeDefinition* type,
                                             const char* symbol,
                                             SymbolIntroList* sil,
                                             FuncBodyStatementList* fbsl) {
    TRACE
    FunctionDefinition* ptr = _alloc_ds(FunctionDefinition);
    initAst(&ptr->ast, AST_FUNC_DEFINITION);
    ptr->type = type;
    ptr->symbol = _copy_str(symbol);
    ptr->sil = sil;
    ptr->fbsl = fbsl;

    return ptr;
}

FuncBodyStatementList* createFuncBodyStatementList() {

    TRACE
    FuncBodyStatementList* ptr = _alloc_ds(FuncBodyStatementList);
    initAst(&ptr->ast, AST_FUNC_BODY_STATEMENT_LIST);
    ptr->list = createPtrLst();

    return ptr;
}

void addFuncBodyStatementList(FuncBodyStatementList* ptr, void* item) {

    TRACE
    appendPtrLst(ptr->list, item);
}

WhileStatement* createWhileStatement(Expression* expr,
                                     FuncBodyStatementList* fbsl) {
    TRACE
    WhileStatement* ptr = _alloc_ds(WhileStatement);
    initAst(&ptr->ast, AST_WHILE_STATEMENT);
    ptr->expr = expr;
    ptr->fbsl = fbsl;

    return ptr;
}

DoStatement* createDoStatement(Expression* expr,
                               FuncBodyStatementList* fbsl) {
    TRACE
    DoStatement* ptr = _alloc_ds(DoStatement);
    initAst(&ptr->ast, AST_DO_STATEMENT);
    ptr->expr = expr;
    ptr->fbsl = fbsl;

    return ptr;
}

IfStatement* createIfStatement() { /*Expression* expr,
                               FuncBodyStatementList* fbsl,
                               ElseClauseList* list) { */
    TRACE
    IfStatement* ptr = _alloc_ds(IfStatement);
    initAst(&ptr->ast, AST_IF_STATEMENT);
/*    ptr->expr = expr;
    ptr->fbsl = fbsl;
    ptr->list = list; */
    ptr->expr = NULL;
    ptr->fbsl = NULL;
    ptr->list = NULL;

    return ptr;
}

ElseClause* createElseClause(Expression* expr,
                             FuncBodyStatementList* fbsl) {
    TRACE
    ElseClause* ptr = _alloc_ds(ElseClause);
    initAst(&ptr->ast, AST_ELSE_CLAUSE);
    ptr->expr = expr;
    ptr->fbsl = fbsl;

    return ptr;
}

ElseClauseList* createElseClauseList() {

    TRACE
    ElseClauseList* ptr = _alloc_ds(ElseClauseList);
    initAst(&ptr->ast, AST_ELSE_CLAUSE_LIST);
    ptr->list = createPtrLst();

    return ptr;
}

void addElseClauseList(ElseClauseList* ptr, void* item) {

    TRACE
    appendPtrLst(ptr->list, item);
}

Assignment* createAssignment(const char* name, Expression* expr) {

    TRACE
    Assignment* ptr = _alloc_ds(Assignment);
    initAst(&ptr->ast, AST_ASSIGNMENT);
    ptr->name = _copy_str(name);
    ptr->expr = expr;

    return ptr;
}
