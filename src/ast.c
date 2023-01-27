
#include "ast.h"
#include "memory.h"
#include "scanner.h"
#include "parser.h"

#ifdef ENABLE_PARSER_TRACE
//#define TRACE   printf("%s() %s:%d:%d\n", __func__, get_file_name(), get_line_no(), get_col_no());
#define TRACE   printf("%s()\n", __func__);
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

FunctionReference* createFunctionReference(const char* name, ExpressionList* list) {

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

ExpressionFactor* createExpressionFactor(ExpressionFactorType type, void* item) {

    TRACE
    ExpressionFactor* ptr = _alloc_ds(ExpressionFactor);
    initAst(&ptr->ast, AST_EXPRESSION_FACTOR);
    ptr->type = type;
    ptr->item = item;

    return ptr;
}


