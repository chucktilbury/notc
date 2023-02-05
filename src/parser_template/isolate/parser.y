
%{
%}

%token NAMESPACE
%token SYMBOL INTEGER UNSIGNED FLOAT STRING LIST DICT CONST
%token LEQU GEQU NEQU EQU LT GT AND OR NOT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%left AND OR
%left LT GT EQU NEQU LEQU GEQU
%left '+' '-'
%left '*' '/' '%'
%left UNARY

%%

module
    : module_list {}
    ;

module_item
    : NAMESPACE SYMBOL '{' module_list '}'
    | assignment {}
    ;

module_list
    : module_item {}
    | module_list module_item {}
    ;

assignment
    : compound_symbol '=' compound_reference {}
    | compound_symbol ADD_ASSIGN compound_reference {}
    | compound_symbol SUB_ASSIGN compound_reference {}
    | compound_symbol MUL_ASSIGN compound_reference {}
    | compound_symbol DIV_ASSIGN compound_reference {}
    | compound_symbol MOD_ASSIGN compound_reference {}
    ;

native_type
    : INTEGER {}
    | UNSIGNED {}
    | FLOAT {}
    | STRING {}
    ;

native_object_type
    : LIST {}
    | DICT {}
    ;

high_level_type
    : native_type {}
    | native_object_type {}
    | compound_symbol {}
    ;

type_spec
    : high_level_type {}
    | CONST high_level_type {}
    ;

array_reference
    : SYMBOL '[' expression ']' {}
    ;

function_reference
    : SYMBOL '(' expression_list ')' {}
    | SYMBOL '(' ')' {}
    ;

compound_symbol
    : SYMBOL {}
    | compound_symbol '.' SYMBOL {}
    ;

compound_refrence_element
    : SYMBOL {}
    | array_reference {}
    | function_reference {}
    ;

compound_reference
    : compound_refrence_element {TRACE("compound_name:compound_element");}
    | compound_reference '.' compound_refrence_element {TRACE("compound_name:add");}
    ;

expression
    : compound_reference {}
    | expression '+' expression {}
    | expression '-' expression {}
    | expression '*' expression {}
    | expression '/' expression {}
    | expression '%' expression {}
    | expression LT expression {}
    | expression GT expression {}
    | expression GEQU expression {}
    | expression LEQU expression {}
    | expression NEQU expression {}
    | expression EQU expression {}
    | expression AND expression {}
    | expression OR expression {}
    | '-' expression %prec UNARY {}
    | NOT expression %prec UNARY {}
    | ':' type_spec ':' expression %prec UNARY {}
    | '(' expression ')'
    ;

expression_list
    : expression {}
    | expression_list ',' expression {}
    ;

%%

