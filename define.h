#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.tab.h"

#define MAXLENGTH   1000    //定义符号表的大小
//结点类别相当于有非终结符 和 终结符(.l文件词法分析Return的都是)
enum node_kind{_BREAK,_CONTINUE,EXT_DEF_LIST,EXT_DEF_VAR,EXT_DEF_FUNC,EXT_VARDEC_LIST,FUNC_DEC,FUNC_PARAM_LIST,FUNC_PARAM_DEC,COMP_STM,COMPSTM_LIST,COMPSTM_EXP,IF_THEN,IF_THEN_ELSE,DEF_LIST,VAR_DEF,VARDEC_LIST,FUNC_CALL,ARRAY_CALL,ARGS,FOR_CONDITION,EXP_FOR1_LIST,EXP_FOR3_LIST};
 

struct node{
    enum node_kind nodeKind;    //结点类型
    union{
        int type_int;
	    float type_float;
        char type_char;
	    char type_id[32];
    };
    struct node *ptr[3];   
    struct node *parent;//父节点   
    int pos;        //记录语法单位所在的位置行号
    int size;         //记录数组的大小
    int index;      //记录数组的下标
    char scope[50]; //所属作用域
    int level;      //层号
    int place;      //结点对应的变量/运算结果符号表的位置序号
    int type;       //结点对应的值的类型
    int num;        //记录子结点个数（包括自己）
    int offset;     //偏移量

    int return_num;
    int break_num;
};

struct opn{
    int kind;                  //标识操作的类型
    int type;                  //标识操作数的类型
    union {
        int     const_int;      //整常数值，立即数
        float   const_float;    //浮点常数值，立即数
        char    const_char;    //字符常数值，立即数
        char    id[33];        //变量或临时变量的别名或标号字符串
        };
    int level;                 //变量的层号，0表示是全局变量，数据保存在静态数据区
    int offset;                 //变量单元偏移量，或函数在符号表的定义位置序号，目标代码生成时用
};

//符号表的行
struct symbol{  
    char name[33];     //变量或函数名
    int level;              //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
    int type;           //变量类型 或 函数返回值类型
    int  paramnum;    //形参个数     
    char alias[10];      //别名，为解决嵌套层次使用，使得每一个数据名称唯一
    char flag;          //符号标记，函数：'F'  变量：'V'   参数：'P'  临时变量：'T'
    char scope[50];
};

//符号表（顺序栈），index初值为0
struct symboltable{
    struct symbol symbols[MAXLENGTH];
    int index;
} symbolTable;


struct symbol_scope_begin {  
    int TX[30];
    int top;
} symbol_scope_TX;

struct node *mknode(int kind,struct node *first,struct node *second, struct node *third,int pos );
char *strcats(char *s1,char *s2);
char *createAlias();
char *createTemp();
void semantic_error(int line,char *msg1,char *msg2);
void prn_symbol();
int searchSymbolTable(char *name);
int fillSymbolTable(char *name,char *alias,int level,int type,char flag,char *scope);
int fill_Temp(char *name,int level,int type,char flag,char *scope);
void ext_var_list(struct node *T); //未完
int  match_param(int i,struct node *T);
void udisplay(struct node *T, int indent);
void semantic_Analysis(struct node *T);//未完
void Exp(struct node *T);
void boolExp(struct node *T);
void semantic_Analysis0(struct node *T);