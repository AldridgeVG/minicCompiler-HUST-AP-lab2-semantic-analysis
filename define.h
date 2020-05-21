#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.tab.h"

#define MAXLENGTH   1000    //������ű��Ĵ�С
//�������൱���з��ս�� �� �ս��(.l�ļ��ʷ�����Return�Ķ���)
enum node_kind{EXT_DEF_LIST,EXT_DEF_VAR,EXT_DEF_FUNC,EXT_VARDEC_LIST,FUNC_DEC,FUNC_PARAM_LIST,FUNC_PARAM_DEC,COMP_STM,COMPSTM_LIST,COMPSTM_EXP,IF_THEN,IF_THEN_ELSE,DEF_LIST,VAR_DEF,VARDEC_LIST,FUNC_CALL,ARRAY_CALL,ARGS,FOR_CONDITION,EXP_FOR3_LIST};
 

struct node{
    enum node_kind nodeKind;    //�������
    union{
        int type_int;
	    float type_float;
        char type_char;
	    char type_id[32];
    };
    struct node *ptr[3];   
    struct node *parent;//���ڵ�   
    int pos;        //��¼�﷨��λ���ڵ�λ���к�
    int size;         //��¼����Ĵ�С
    int index;      //��¼������±�
    char scope[50]; //����������
    int level;      //���
    int place;      //����Ӧ�ı���/���������ű���λ�����
    int type;       //����Ӧ��ֵ������
    int num;        //��¼�ӽ������������Լ���
    int offset;     //ƫ����

    int break_num;
};

struct opn{
    int kind;                  //��ʶ����������
    int type;                  //��ʶ������������
    union {
        int     const_int;      //������ֵ��������
        float   const_float;    //���㳣��ֵ��������
        char    const_char;    //�ַ�����ֵ��������
        char    id[33];        //��������ʱ�����ı��������ַ���
        };
    int level;                 //�����Ĳ�ţ�0��ʾ��ȫ�ֱ��������ݱ����ھ�̬������
    int offset;                 //������Ԫƫ�����������ڷ��ű��Ķ���λ����ţ�Ŀ���������ʱ��
};

struct symbol{  
    char name[33];     //����������
    int level;   //��ţ��ⲿ���������������Ϊ0���β���Ϊ1��ÿ��1����������ż�1���˳���1
    int type;           //�������� �� ��������ֵ����
    int  paramnum;    //�βθ���
    char alias[10];      //������Ϊ���Ƕ�ײ��ʹ�ã�ʹ��ÿһ����������Ψһ
    char flag;          //���ű�ǣ�������'F'  ������'V'   ������'P'  ��ʱ������'T'
    char scope[50];
};

//���ű�����һ��˳��ջ��index��ֵΪ0
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
void ext_var_list(struct node *T); //δ��
int  match_param(int i,struct node *T);
void udisplay(struct node *T, int indent);
void semantic_Analysis(struct node *T);//δ��
void Exp(struct node *T);
void boolExp(struct node *T);
void semantic_Analysis0(struct node *T);