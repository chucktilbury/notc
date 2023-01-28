#ifndef _DUMP_H_
#define _DUMP_H_

#ifdef ENABLE_AST_DUMP

const char* astTypeToStr(AstType t);
const char* operatorTypeToStr(OperatorType t);
const char* exprTypeToStr(ExpressionFactorType t);
const char* dataTypeToStr(int t);

void dumpAst(Ast* ptr);
void dumpModule(Module* ptr);
void dumpModuleList(ModuleList* ptr);
void dumpTypeDefinition(TypeDefinition* ptr);
void dumpConstantExpression(ConstantExpression* ptr);
void dumpSymbolIntro(SymbolIntro* ptr);
void dumpSymbolIntroList(SymbolIntroList* ptr);
void dumpExpression(Expression* ptr);
void dumpOperator(Operator* ptr);
void dumpExpressionFactor(ExpressionFactor* ptr);
void dumpExpressionList(ExpressionList* ptr);
void dumpFunctionReference(FunctionReference* ptr);
void dumpSingleStatement(SingleStatement* ptr);
void dumpTraceStatement(TraceStatement* ptr);

void dumpFunctionDefinition(FunctionDefinition* ptr);
void dumpFuncBodyStatementList(FuncBodyStatementList* ptr);

void dumpWhileStatement(WhileStatement* ptr);
void dumpDoStatement(DoStatement* ptr);
void dumpIfStatement(IfStatement* ptr);

void dumpElseClause(ElseClause* ptr);
void dumpElseClauseList(ElseClauseList* ptr);

void dumpAssignment(Assignment* ptr);

#else

#define dumpModule(p)
#endif /* ENABLE_AST_DUMP */

#endif /* _DUMP_H_ */
