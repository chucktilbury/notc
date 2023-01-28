
#ifdef ENABLE_AST_DUMP

#include <stdio.h>
#include <stdarg.h>

#include "ast.h"
#include "dump.h"
#include "parser.h"

#define INDENT 4
static FILE* outfh;
static int indent = 0;
const char* tokenToStr(int tok);

static void inc_indent() {
    indent += INDENT;
}

static void dec_indent() {
    indent -= INDENT;
}

static void print(const char* fmt, ...) {

    for(int i = 0; i < indent; i++)
        fputc(' ', outfh);

    va_list args;

    va_start(args, fmt);
    vfprintf(outfh, fmt, args);
    va_end(args);

    fputc('\n', outfh);
}

const char* astTypeToStr(AstType t) {

    return (t == AST_MODULE)? "AST_MODULE":
        (t == AST_MODULE_LIST)? "AST_MODULE_LIST":
        (t == AST_TYPE_DEFINITION)? "AST_TYPE_DEFINITION":
        (t == AST_TYPE_SPEC)? "AST_TYPE_SPEC":
        (t == AST_CAST_SPEC)? "AST_CAST_SPEC":
        (t == AST_CONSTANT_EXPRESSION)? "AST_CONSTANT_EXPRESSION":
        (t == AST_SYMBOL_INTRO)? "AST_SYMBOL_INTRO":
        (t == AST_SYMBOL_DECL)? "AST_SYMBOL_DECL":
        (t == AST_SYMBOL_INTRO_LIST)? "AST_SYMBOL_INTRO_LIST":
        (t == AST_EXPRESSION_FACTOR)? "AST_EXPRESSION_FACTOR":
        (t == AST_EXPRESSION)? "AST_EXPRESSION":
        (t == AST_EXPRESSION_LIST)? "AST_EXPRESSION_LIST":
        (t == AST_FUNCTION_REFERENCE)? "AST_FUNCTION_REFERENCE":
        (t == AST_FUNC_DECL_PARAMETER_LIST)? "AST_FUNC_DECL_PARAMETER_LIST":
        (t == AST_FUNC_DEFINITION)? "AST_FUNC_DEFINITION":
        (t == AST_FUNC_BODY_STATEMENT_LIST)? "AST_FUNC_BODY_STATEMENT_LIST":
        (t == AST_FUNC_BODY)? "AST_FUNC_BODY":
        (t == AST_BREAK_STATEMENT)? "AST_BREAK_STATEMENT":
        (t == AST_CONTINUE_STATEMENT)? "AST_CONTINUE_STATEMENT":
        (t == AST_YIELD_STATEMENT)? "AST_YIELD_STATEMENT":
        (t == AST_ELSE_CLAUSE)? "AST_ELSE_CLAUSE":
        (t == AST_ELSE_CLAUSE_INTERMEDIATE_LIST)? "AST_ELSE_CLAUSE_INTERMEDIATE_LIST":
        (t == AST_ELSE_CLAUSE_FINAL)? "AST_ELSE_CLAUSE_FINAL":
        (t == AST_ELSE_CLAUSE_LIST)? "AST_ELSE_CLAUSE_LIST":
        (t == AST_IF_CLAUSE)? "AST_IF_CLAUSE":
        (t == AST_IF_STATEMENT)? "AST_IF_STATEMENT":
        (t == AST_WHILE_STATEMENT)? "AST_WHILE_STATEMENT":
        (t == AST_DO_STATEMENT)? "AST_DO_STATEMENT":
        (t == AST_MODULE_ITEM)? "AST_MODULE_ITEM":
        (t == AST_TRACE_STATEMENT)? "AST_TRACE_STATEMENT":
        (t == AST_RETURN_STATEMENT)? "AST_RETURN_STATEMENT":
        (t == AST_PRINT_STATEMENT)? "AST_PRINT_STATEMENT":
        (t == AST_EXIT_STATEMENT)? "AST_EXIT_STATEMENT":
        (t == AST_TYPE_STATEMENT)? "AST_TYPE_STATEMENT":
        (t == AST_FUNC_BODY_STATEMENT)? "AST_FUNC_BODY_STATEMENT":
        (t == AST_SINGLE_STATEMENT)? "AST_SINGLE_STATEMENT":
        (t == AST_ASSIGNMENT)? "AST_ASSIGNMENT":
        (t == AST_SYMBOL)? "AST_SYMBOL":
        (t == AST_OPERATOR)? "AST_OPERATOR": "UNKNOWN";
}

const char* operatorTypeToStr(OperatorType t) {

    return (t == OP_ADD)? "OP_ADD":
        (t == OP_SUB)? "OP_SUB":
        (t == OP_MUL)? "OP_MUL":
        (t == OP_DIV)? "OP_DIV":
        (t == OP_MOD)? "OP_MOD":
        (t == OP_UNNEG)? "OP_UNNEG":
        (t == OP_UNNOT)? "OP_UNNOT":
        (t == OP_EQU)? "OP_EQU":
        (t == OP_NEQU)? "OP_NEQU":
        (t == OP_LORE)? "OP_LORE":
        (t == OP_GORE)? "OP_GORE":
        (t == OP_LT)? "OP_LT":
        (t == OP_GT)? "OP_GT":
        (t == OP_AND)? "OP_AND":
        (t == OP_OR)? "OP_OR":
        (t == OP_ADD_ASSIGN)? "OP_ADD_ASSIGN":
        (t == OP_SUB_ASSIGN)? "OP_SUB_ASSIGN":
        (t == OP_MUL_ASSIGN)? "OP_MUL_ASSIGN":
        (t == OP_DIV_ASSIGN)? "OP_DIV_ASSIGN":
        (t == OP_MOD_ASSIGN)? "OP_MOD_ASSIGN": "UNKNOWN";
}

const char* exprTypeToStr(ExpressionFactorType t) {

    return (t == EXPR_FACT_CONST)? "EXPR_FACT_CONST":
        (t == EXPR_FACT_SYM)? "EXPR_FACT_SYM":
        (t == EXPR_FACT_FUNC_REF)? "EXPR_FACT_FUNC_REF": "UNKNOWN";
}

const char* dataTypeToStr(int t) {

    return (t == UINT)? "UINT":
        (t == INT)? "INT":
        (t == FLOAT)? "FLOAT":
        (t == STRING)? "STRING":
        (t == BOOLEAN)? "BOOLEAN":
        (t == NOTHING)? "NOTHING": "UNKNOWN";
}

void dumpAst(Ast* ptr) {

    print("%s", astTypeToStr(ptr->type));
    /*
    inc_indent();
    {
        print("%s: %d: %d", ptr->fname, ptr->line, ptr->col);
    }
    dec_indent();
    */
}

void dumpModule(Module* ptr) {

    if(outfh == NULL)
        outfh = stdout;
    print("\n");

    dumpAst(&ptr->ast);
    inc_indent();
    {
        dumpModuleList((ModuleList*)ptr->items);
    }
    dec_indent();
}

void dumpModuleList(ModuleList* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        int len = getLenPtrLst(ptr->list);
        Ast** lst = (Ast**)getRawPtrLst(ptr->list);
        for(int i = 0; i < len; i++) {
            switch(lst[i]->type) {
                case AST_SYMBOL_INTRO:
                    dumpSymbolIntro((SymbolIntro*)lst[i]);
                    break;
                case AST_FUNC_DEFINITION:
                    dumpFunctionDefinition((FunctionDefinition*)lst[i]);
                    break;
                default:
                    print("%s() type error", __func__);
                    printf("(REALLY) NOT SUPPORTED %s (%d)\n",
                           astTypeToStr(lst[i]->type), lst[i]->type);
                    break;

            }
        }
    }
    dec_indent();
}

void dumpTypeDefinition(TypeDefinition* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("type: %s", dataTypeToStr(ptr->type));
        print("is_const: %s", ptr->is_const? "true": "false");
    }
    dec_indent();
}

void dumpConstantExpression(ConstantExpression* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("type: %s", dataTypeToStr(ptr->type));
        switch(ptr->type) {
            case UINT:      print("value: %lu", ptr->data.unum); break;
            case INT:       print("value: %ld", ptr->data.inum); break;
            case FLOAT:     print("value: %f", ptr->data.fnum); break;
            case STRING:    print("value: \"%s\"", ptr->data.str); break;
            case BOOLEAN:   print("value: %s", ptr->data.bval? "true":"false"); break;
            case NOTHING:   print("value: none"); break;
            default:
                print("%s() type error", __func__);
                print("UNKNOWN constntant expression type");
                break;
        }
    }
    dec_indent();
}

void dumpSymbolIntro(SymbolIntro* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    { // type, name, assign
        print("name: \"%s\"", ptr->str);
        dumpTypeDefinition(ptr->type);
        if(ptr->assign != NULL)
            dumpExpression(ptr->assign);
    }
    dec_indent();
}

void dumpSymbolIntroList(SymbolIntroList* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
    }
    dec_indent();
}

void dumpExpression(Expression* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        int len = getLenPtrLst(ptr->list);
        Ast** lst = (Ast**)getRawPtrLst(ptr->list);
        for(int i = 0; i < len; i++) {
            // expression factor and operator
            switch(lst[i]->type) {
                case AST_EXPRESSION_FACTOR:
                    dumpExpressionFactor((ExpressionFactor*)lst[i]);
                    break;
                case AST_OPERATOR:
                    dumpOperator((Operator*)lst[i]);
                    break;
                case AST_FUNCTION_REFERENCE:
                    dumpFunctionReference((FunctionReference*)lst[i]);
                    break;
                default:
                    print("%s() type error", __func__);
                    break;
            }
        }
    }
    dec_indent();
}

void dumpOperator(Operator* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("operator: %s", operatorTypeToStr(ptr->type));
    }
    dec_indent();
}

void dumpExpressionFactor(ExpressionFactor* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("type: %s", astTypeToStr(ptr->type));
        switch(ptr->type) {
            case AST_CONSTANT_EXPRESSION:
                dumpConstantExpression((ConstantExpression*)ptr->item);
                break;
            case AST_SYMBOL:
                print("symbol: \"%s\"", (const char*)ptr->item);
            case AST_FUNCTION_REFERENCE:
                dumpFunctionReference((FunctionReference*)ptr->item);
                break;
            default:
                print("%s() type error", __func__);
                print("really not supported");
                break;
        }
    }
    dec_indent();
}

void dumpExpressionList(ExpressionList* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        int len = getLenPtrLst(ptr->list);
        Ast** lst = (Ast**)getRawPtrLst(ptr->list);
        for(int i = 0; i < len; i++) {
            dumpExpression((Expression*)lst[i]);
        }
    }
    dec_indent();
}

void dumpFunctionReference(FunctionReference* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("name: \"%s\"", ptr->name);
        if(ptr->list != NULL) {
            if(getLenPtrLst((PtrLst*)ptr->list) > 0)
                dumpExpressionList(ptr->list);
        }
        else
            print("empty expression list");
    }
    dec_indent();

}

void dumpSingleStatement(SingleStatement* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("type: %s", tokenToStr(ptr->type));
        if(ptr->expr != NULL)
            dumpExpression(ptr->expr);
    }
    dec_indent();
}

void dumpTraceStatement(TraceStatement* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("message: %s", ptr->str);
    }
    dec_indent();
}

void dumpFunctionDefinition(FunctionDefinition* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        dumpTypeDefinition(ptr->type);
        print("name: \"%s\"", ptr->symbol);
        if(ptr->sil != NULL)
            dumpSymbolIntroList(ptr->sil);
        else
            print("no parameter list defined");

        if(ptr->fbsl != NULL)
            dumpFuncBodyStatementList(ptr->fbsl);
        else
            print("no function body defined");
    }
    dec_indent();
}

void dumpFuncBodyStatementList(FuncBodyStatementList* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        int len = getLenPtrLst(ptr->list);
        Ast** lst = (Ast**)getRawPtrLst(ptr->list);
        for(int i = 0; i < len; i++) {
            switch(lst[i]->type) {
                case AST_FUNC_DEFINITION:
                    dumpFunctionDefinition((FunctionDefinition*)lst[i]);
                    break;
                case AST_FUNCTION_REFERENCE:
                    dumpFunctionReference((FunctionReference*)lst[i]);
                    break;
                case AST_SINGLE_STATEMENT:
                    dumpSingleStatement((SingleStatement*)lst[i]);
                    break;
                case AST_SYMBOL_INTRO:
                    dumpSymbolIntro((SymbolIntro*)lst[i]);
                    break;
                case AST_WHILE_STATEMENT:
                    dumpWhileStatement((WhileStatement*)lst[i]);
                    break;
                case AST_DO_STATEMENT:
                    dumpDoStatement((DoStatement*)lst[i]);
                    break;
                case AST_IF_STATEMENT:
                    dumpIfStatement((IfStatement*)lst[i]);
                    break;
                case AST_ASSIGNMENT:
                    dumpAssignment((Assignment*)lst[i]);
                    break;
                default:
                    print("%s() type error (%d)", __func__, lst[i]->type);
                    print("really not supported");
                    break;
            }
        }
        // TODO: ASSIGNMENTS ARE MISSING!
    }
    dec_indent();
}

void dumpWhileStatement(WhileStatement* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        if(ptr->expr != NULL)
            dumpExpression(ptr->expr);
        else
            print("expression is blank");

        if(ptr->fbsl != NULL)
            dumpFuncBodyStatementList(ptr->fbsl);
        else
            print("function body is blank");
    }
    dec_indent();
}

void dumpDoStatement(DoStatement* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        if(ptr->expr != NULL)
            dumpExpression(ptr->expr);
        else
            print("expression is blank");

        if(ptr->fbsl != NULL)
            dumpFuncBodyStatementList(ptr->fbsl);
        else
            print("function body is blank");
    }
    dec_indent();
}

void dumpIfStatement(IfStatement* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        if(ptr->expr != NULL)
            dumpExpression(ptr->expr);
        else
            print("expression is blank");

        if(ptr->fbsl != NULL)
            dumpFuncBodyStatementList(ptr->fbsl);
        else
            print("function body is blank");

        if(ptr->list != NULL)
            dumpElseClauseList(ptr->list);
        else
            print("no else clauses are present");
    }
    dec_indent();
}

void dumpElseClause(ElseClause* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        if(ptr->expr != NULL)
            dumpExpression(ptr->expr);
        else
            print("expression is blank");

        if(ptr->fbsl != NULL)
            dumpFuncBodyStatementList(ptr->fbsl);
        else
            print("function body is blank");
    }
    dec_indent();
}

void dumpElseClauseList(ElseClauseList* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        int len = getLenPtrLst(ptr->list);
        Ast** lst = (Ast**)getRawPtrLst(ptr->list);
        for(int i = 0; i < len; i++) {
            dumpElseClause((ElseClause*)lst[i]);
        }
    }
    dec_indent();
}

void dumpAssignment(Assignment* ptr) {

    dumpAst(&ptr->ast);
    inc_indent();
    {
        print("name: \"%s\"", ptr->name);
        dumpExpression(ptr->expr);
    }
    dec_indent();
}

#endif /* ENABLE_AST_DUMP */
