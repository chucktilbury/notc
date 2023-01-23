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
#else

#define dumpModule(p)
#endif /* ENABLE_AST_DUMP */

#endif /* _DUMP_H_ */
