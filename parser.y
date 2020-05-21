%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "define.h"

extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
void display(struct node *,int);
%}

%union {
	int    type_int;
	float  type_float;
        char   type_char;
	char   type_id[32];
	struct node *ptr;
};


%type  <ptr> Program ExtDefList ExtDef  Specifier ExtDecList FuncDec CompSt VarList VarDec  ParamDec Stmt StmtList DefList Def DecList Dec Exp Args ForCondition ExpFor1 ExpFor2 ExpFor3  ExpFor3List

%token <type_int> INT  
%token <type_float> FLOAT 
%token <type_char> CHAR
%token <type_id> ID RELOP TYPE ARRAY
%token LP RP LB RB LC RC SEMI COMMA   
%token bool ASSIGNOP PLUS MINUS STAR DIV AND OR DOT NOT STRUCT RETURN BREAK IF ELSE WHILE FOR SELFADD SELFDEC ADD_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP

%right ASSIGNOP ADD_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT SELFADD SELFDEC
%nonassoc LOWER_THAN_RB
%left DOT LP RP LB RB 
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE
%nonassoc ARRAY_FIRST

%%
//High-level Definitions
Program: ExtDefList    {/*display($1, 0);*/udisplay($1, 0);semantic_Analysis0($1);}   
         ; 
ExtDefList:     {$$=NULL;}
          | ExtDef ExtDefList {$$=mknode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}   //每一个EXTDEFLIST的结点，其第1棵子树对应一个全局变量、结构体或函数
          ;  
ExtDef:  Specifier ExtDecList SEMI   {$$=mknode(EXT_DEF_VAR,$1,$2,NULL,yylineno);}   //该结点对应一个全局变量
         |Specifier FuncDec CompSt    {$$=mknode(EXT_DEF_FUNC,$1,$2,$3,yylineno);}         //该结点对应一个函数定义
         | error SEMI   {$$=NULL; }
         // |Specifier SEMI    {}  //该结点对应一个结构体定义
         ;
ExtDecList:  VarDec      {$$=$1;}       
           |VarDec COMMA ExtDecList {$$=mknode(EXT_VARDEC_LIST,$1,$3,NULL,yylineno);}  /*每一个EXT_DECLIST的结点，其第一棵子树对应一个变量名(ID类型的结点),第二棵子树对应剩下的外部变量名*/
           ;  

//Specifiers
Specifier:  TYPE    {$$=mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);$$->type=!strcmp($1,"int")?INT:(!strcmp($1,"float")?FLOAT:CHAR);}              
           ;   
  


//Declarators
VarDec:  ID LB INT RB    {$$=mknode(ARRAY,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);$$->size=$3;}
        |ID       {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}   
        ;
/*VarDec: ID VarDec_replace    {$$=mknode(ID,$2,NULL,NULL,yylineno);strcpy($$->type_id,$1);}   //ID结点，标识符符号串存放结点的type_id
        ;
VarDec_replace: {$$=NULL; }
                | LB INT RB VarDec_replace {$$=mknode(ARRAY,$4,NULL,NULL,yylineno);strcpy($$->type_id,"ARRAY");$$->size =$2;} //数组结点
                ;
*/
FuncDec: ID LP VarList RP   {$$=mknode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
	|ID LP  RP   {$$=mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
        ;  
VarList: ParamDec  {$$=mknode(FUNC_PARAM_LIST,$1,NULL,NULL,yylineno);}
        | ParamDec COMMA  VarList  {$$=mknode(FUNC_PARAM_LIST,$1,$3,NULL,yylineno);}
        ;
ParamDec: Specifier VarDec         {$$=mknode(FUNC_PARAM_DEC,$1,$2,NULL,yylineno);}
         ;

//Statements
CompSt: LC DefList StmtList RC    {$$=mknode(COMP_STM,$2,$3,NULL,yylineno);}
       ;
StmtList: {$$=NULL; }  
        | Stmt StmtList  {$$=mknode(COMPSTM_LIST,$1,$2,NULL,yylineno);}
        ;
Stmt:   Exp SEMI    {$$=mknode(COMPSTM_EXP,$1,NULL,NULL,yylineno);}
      | CompSt      {$$=$1;}      //复合语句结点直接最为语句结点，不再生成新的结点
      | RETURN Exp SEMI   {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
      | WHILE LP Exp RP Stmt {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
      | FOR LP ForCondition RP Stmt {$$=mknode(FOR,$3,$5,NULL,yylineno);}
      ;

ForCondition: ExpFor1 SEMI ExpFor2 SEMI ExpFor3 {$$=mknode(FOR_CONDITION,$1,$3,$5,yylineno);}
              ;

//Local Definitions
DefList: {$$=NULL; }
        | Def DefList {$$=mknode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;
Def:    Specifier DecList SEMI {$$=mknode(VAR_DEF,$1,$2,NULL,yylineno);}
        ;
DecList: Dec  {$$=mknode(VARDEC_LIST,$1,NULL,NULL,yylineno);}
       | Dec COMMA DecList  {$$=mknode(VARDEC_LIST,$1,$3,NULL,yylineno);}
	   ;
Dec:     VarDec  {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
       ;

//Expressions
Exp:    Exp ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
      | Exp AND Exp   {$$=mknode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}
      | Exp OR Exp    {$$=mknode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}
      | Exp RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
      | Exp PLUS Exp  {$$=mknode(PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUS");}
      | Exp MINUS Exp {$$=mknode(MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS");}
      | Exp STAR Exp  {$$=mknode(STAR,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=mknode(DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV");}
      | LP Exp RP     {$$=$2;}
      | MINUS Exp %prec UMINUS   {$$=mknode(UMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"UMINUS");}
      | NOT Exp       {$$=mknode(NOT,$2,NULL,NULL,yylineno);strcpy($$->type_id,"NOT");}
      | VarDec SELFADD     {$$=mknode(SELFADD,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SELFADD");}
      | SELFADD VarDec     {$$=mknode(SELFADD,NULL,$2,NULL,yylineno);strcpy($$->type_id,"SELFADD");}
      | VarDec SELFDEC     {$$=mknode(SELFDEC,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SELFDEC");}
      | SELFDEC VarDec     {$$=mknode(SELFDEC,NULL,$2,NULL,yylineno);strcpy($$->type_id,"SELFDEC");}
      | VarDec ADD_ASSIGNOP Exp {$$=mknode(ADD_ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ADD_ASSIGNOP");}
      | VarDec MINUS_ASSIGNOP Exp {$$=mknode(MINUS_ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS_ASSIGNOP");}
      | VarDec STAR_ASSIGNOP Exp {$$=mknode(STAR_ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR_ASSIGNOP");}
      | VarDec DIV_ASSIGNOP Exp {$$=mknode(DIV_ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV_ASSIGNOP");}
      | ID LP Args RP {$$=mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | ID LP RP      {$$=mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | ID LB Exp RB  {$$=mknode(ARRAY_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
    //| Exp DOT ID 
      | VarDec          {$$=$1;}
      | INT %prec LOWER_THAN_RB   {$$=mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}
      | FLOAT         {$$=mknode(FLOAT,NULL,NULL,NULL,yylineno);$$->type_float=$1;$$->type=FLOAT;}
      | CHAR          {$$=mknode(CHAR,NULL,NULL,NULL,yylineno);$$->type_char=$1;$$->type=CHAR;}
      | BREAK         {$$=mknode(BREAK,NULL,NULL,NULL,yylineno);}
      ;
Args:    Exp COMMA Args    {$$=mknode(ARGS,$1,$3,NULL,yylineno);}
       | Exp               {$$=mknode(ARGS,$1,NULL,NULL,yylineno);}
       ;
ExpFor1: {$$=NULL; }
        |VarDec ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
        ;
ExpFor2: {$$=NULL; }
        |VarDec RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}
        ;
ExpFor3:  {$$=NULL;}
      | ExpFor3List {$$=$1;}
      ;
ExpFor3List: Exp {$$=$1;}
      | Exp COMMA ExpFor3List {$$=mknode(EXP_FOR3_LIST,$1,$3,NULL,yylineno);}
      ; 

%%
#include<stdarg.h>
int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
	}

void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}	

