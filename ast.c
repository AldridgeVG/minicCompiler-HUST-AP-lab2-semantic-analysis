#include "define.h"

struct node *mknode(int nodeKind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->nodeKind = nodeKind;
    if(first)first->parent=T;
    if(second)second->parent=T;
    if(third)third->parent=T;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}

void display(struct node *T, int indent)
{
    if (T)
    {
        switch (T->nodeKind)
        {
        case EXT_DEF_LIST:
            printf("%*cExtDefList: \n", indent, ' ');
            strcpy(T->scope,"Global");
            display(T->ptr[0], indent + 3); //显示外部定义列表的第一个
            display(T->ptr[1], indent + 3); //显示外部定义列表的其他个
            break;
        case EXT_DEF_VAR:
            printf("%*cExtDef_Var: \n", indent, ' ');
            strcpy(T->scope,T->parent->scope);

            display(T->ptr[0], indent + 3); //显示外部(全局)变量类型
            display(T->ptr[1], indent + 3); //显示全局变量列表
            break;
        case EXT_DEF_FUNC:
            printf("%*cExtDef_Func：\n", indent, ' ');
            strcpy(T->scope,T->ptr[1]->type_id);
            display(T->ptr[0], indent + 3); //显示函数返回类型
            display(T->ptr[1], indent + 3); //显示函数名和参数
            display(T->ptr[2], indent + 3); //显示函数体
            break;
        //case 外部结构体声明
        case EXT_VARDEC_LIST:
            strcpy(T->scope,"Global");
            display(T->ptr[0], indent); //依次显示全局变量名
            display(T->ptr[1], indent);
            break;
        case TYPE:
            strcpy(T->scope,T->parent->scope);
            //printf("**** %s\n", T->scope);
            printf("%*c%s\n", indent, ' ', T->type_id); //显示基本类型
            break;
        //结构体定义还未实现
        case ID:
            strcpy(T->scope,T->parent->scope);
           //printf("**** %s\n", T->scope);
            printf("%*c%s\n", indent, ' ', T->type_id); //显示变量名 
            break;
        case ARRAY:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%s[] %d\n", indent, ' ', T->type_id,T->size);
            break;
        case ARRAY_CALL:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%s[]\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent+3);
            break;
        case FUNC_DEC:
            strcpy(T->scope,"Global");
            printf("%*cFunc_name：%s\n", indent, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*cFunc_params：\n", indent, ' ');
                display(T->ptr[0], indent + 3); //显示函数参数列表
            }
            break;
        case FUNC_PARAM_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //依次显示参数类型和名称
            display(T->ptr[1], indent);
            break;
        case FUNC_PARAM_DEC:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%s %s\n", indent, ' ', T->ptr[0]->type_id, T->ptr[1]->type_id);
            break;
        case COMP_STM:
            strcpy(T->scope,T->parent->scope);
            printf("%*cComSt: \n",indent,' ');
            display(T->ptr[0], indent + 3); //显示变量定义部分
            display(T->ptr[1], indent + 3); //显示语句部分
            break;
        case COMPSTM_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //显示第一条语句
            display(T->ptr[1], indent); //显示剩下语句
            break;
        case COMPSTM_EXP:
            strcpy(T->scope,T->parent->scope);
            // printf("%*cstmt_exp: ", indent, ' ');
            display(T->ptr[0], indent);
            break;
        case RETURN:
            strcpy(T->scope,T->parent->scope);
            printf("%*cstmt_return：\n", indent, ' ');
            display(T->ptr[0], indent + 3);
            break;
        case IF_THEN:
            strcpy(T->scope,"IF of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_IF：\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示condition
            display(T->ptr[1], indent + 3); //显示if_body
            break;
        case IF_THEN_ELSE:
            strcpy(T->scope,"IF_ELSE of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_IF：\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示condition
            // printf("%*cstmt_ifbody:", indent+3, ' ');
            display(T->ptr[1], indent+3); //显示if_body
            // printf("%*cstmt_elsebody:", indent+3, ' ');
            display(T->ptr[2], indent+3); //显示else_body
            break;
        case WHILE:
            strcpy(T->scope,"WHILE of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_while:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示condition
            display(T->ptr[1], indent + 3); //显示while_body
            break;
        case FOR:
            strcpy(T->scope,"FOR of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_for:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示condition
            display(T->ptr[1], indent + 3); //显示for_body
            break;
        case FOR_CONDITION:
            strcpy(T->scope,T->parent->scope);
            printf("%*cstmt_for_condition:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示for循环第一个表达式——初始化
            display(T->ptr[1], indent + 3); //显示for循环第二个表达式
            display(T->ptr[2], indent + 3); //显示for循环第三个表达式
            break;
        case EXP_FOR3_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //显示for循环第一个表达式——初始化
            display(T->ptr[1], indent); //显示for循环第二个表达式
            break;
        case DEF_LIST:
            strcpy(T->scope,T->parent->scope);
            printf("%*cdef_list: \n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示局部变量定义列表中的第一个
            display(T->ptr[1], indent + 3); //显示其它局部变量定义
            break;
        case VAR_DEF:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //显示局部变量类型
            display(T->ptr[1], indent); //显示其他局部变量
            break;
        case VARDEC_LIST:
            strcpy(T->scope,T->parent->scope);
            while (T)
            {
                strcpy(T->scope,T->parent->scope);
                display(T->ptr[0], indent);
                T = T->ptr[1];
            }
            break;
        case ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c=\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case ADD_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c+=\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case MINUS_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c-=\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case STAR_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c*=\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case DIV_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c/=\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case AND:
            strcpy(T->scope,T->parent->scope);
            printf("%*c&&\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case OR:
            strcpy(T->scope,T->parent->scope);
            printf("%*c||\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case RELOP:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%s\n", indent, ' ',T->type_id);
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case PLUS:
            strcpy(T->scope,T->parent->scope);
            printf("%*c+\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case MINUS:
            strcpy(T->scope,T->parent->scope);
            printf("%*c-\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case STAR:
            strcpy(T->scope,T->parent->scope);
            printf("%*c*\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case DIV:
            strcpy(T->scope,T->parent->scope);
            printf("%*c/\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            display(T->ptr[1], indent+3);
            break;
        case UMINUS:
            strcpy(T->scope,T->parent->scope);
            printf("%*c-\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            break;
        case NOT:
            strcpy(T->scope,T->parent->scope);
            printf("%*c!\n", indent, ' ');
            display(T->ptr[0], indent+3); 
            break;
        case SELFADD:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); 
            printf("%*c++\n", indent+3, ' ');
            display(T->ptr[1], indent+6); 
            break;
        case SELFDEC:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); 
            printf("%*c--\n", indent+3, ' ');
            display(T->ptr[1], indent+6); 
            break;
        case FUNC_CALL:
            strcpy(T->scope,T->parent->scope);
            printf("%*cfunc_call:\n", indent, ' ');
            printf("%*cfunc_name：%s\n", indent + 3, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*cfunc_args:\n", indent + 3 );
                display(T->ptr[0], indent + 6);
            }
            break;
        case INT:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%d\n", indent, ' ', T->type_int);
            break;
        case FLOAT:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%f\n", indent, ' ', T->type_float);
            break;
        case CHAR:
            strcpy(T->scope,T->parent->scope);
            printf("%*c'%c'\n", indent, ' ', T->type_char);
            break;
        case ARGS:
            while (T)
            {                 
                display(T->ptr[0],indent);
                T = T->ptr[1];
            }
            break;
       
        }
    }
}
void udisplay(struct node *T, int indent)
{
    if (T)
    {
        switch (T->nodeKind)
        {
        case EXT_DEF_LIST:
            strcpy(T->scope,"Global");
            udisplay(T->ptr[0], indent + 3); //显示外部定义列表的第一个
            udisplay(T->ptr[1], indent + 3); //显示外部定义列表的其他个
            break;
        case EXT_DEF_VAR:
            strcpy(T->scope,T->parent->scope);

            udisplay(T->ptr[0], indent + 3); //显示外部(全局)变量类型
            udisplay(T->ptr[1], indent + 3); //显示全局变量列表
            break;
        case EXT_DEF_FUNC:
            
            strcpy(T->scope,T->ptr[1]->type_id);
            udisplay(T->ptr[0], indent + 3); //显示函数返回类型
            udisplay(T->ptr[1], indent + 3); //显示函数名和参数
            udisplay(T->ptr[2], indent + 3); //显示函数体
            break;
        //case 外部结构体声明
        case EXT_VARDEC_LIST:
            strcpy(T->scope,"Global");
            udisplay(T->ptr[0], indent); //依次显示全局变量名
            udisplay(T->ptr[1], indent);
            break;
        case TYPE:
            strcpy(T->scope,T->parent->scope);
            //printf("**** %s\n", T->scope);
           
            break;
        //结构体定义还未实现
        case ID:
            strcpy(T->scope,T->parent->scope);
           //printf("**** %s\n", T->scope);
           
            break;
        case ARRAY:
            strcpy(T->scope,T->parent->scope);
           
            break;
        case ARRAY_CALL:
            strcpy(T->scope,T->parent->scope);
            
            udisplay(T->ptr[0], indent+3);
            break;
        case FUNC_DEC:
            strcpy(T->scope,T->type_id);
            //printf("%s\n", T->type_id);
            if (T->ptr[0])
            {
               
                udisplay(T->ptr[0], indent + 3); //显示函数参数列表
            }
            break;
        case FUNC_PARAM_LIST:
            strcpy(T->scope,"Para of ");
            strcat(T->scope,T->parent->scope);
            //printf("******* %s %s\n", T->scope,T->type_id);
            udisplay(T->ptr[0], indent); //依次显示参数类型和名称
            udisplay(T->ptr[1], indent);
            break;
        case FUNC_PARAM_DEC:
            strcpy(T->scope,T->parent->scope);
           
            break;
        case COMP_STM:
            strcpy(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent + 3); //显示变量定义部分
            udisplay(T->ptr[1], indent + 3); //显示语句部分
            break;
        case COMPSTM_LIST:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //显示第一条语句
            udisplay(T->ptr[1], indent); //显示剩下语句
            break;
        case COMPSTM_EXP:
            strcpy(T->scope,T->parent->scope);
            // printf("%*cstmt_exp: ", indent, ' ');
            udisplay(T->ptr[0], indent);
            break;
        case RETURN:
            strcpy(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent + 3);
            break;
        case IF_THEN:
            strcpy(T->scope,"IF of ");
            strcat(T->scope,T->parent->scope);
          
            udisplay(T->ptr[0], indent + 3); //显示condition
            udisplay(T->ptr[1], indent + 3); //显示if_body
            break;
        case IF_THEN_ELSE:
            strcpy(T->scope,"IF_ELSE of ");
            strcat(T->scope,T->parent->scope);
         
            udisplay(T->ptr[0], indent + 3); //显示condition
            // printf("%*cstmt_ifbody:", indent+3, ' ');
            udisplay(T->ptr[1], indent+3); //显示if_body
            // printf("%*cstmt_elsebody:", indent+3, ' ');
            udisplay(T->ptr[2], indent+3); //显示else_body
            break;
        case WHILE:
            strcpy(T->scope,"WHILE of ");
            strcat(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent + 3); //显示condition
            udisplay(T->ptr[1], indent + 3); //显示while_body
            break;
        case FOR:
            strcpy(T->scope,"FOR of ");
            strcat(T->scope,T->parent->scope);
          
            udisplay(T->ptr[0], indent + 3); //显示condition
            udisplay(T->ptr[1], indent + 3); //显示for_body
            break;
        case FOR_CONDITION:
            strcpy(T->scope,T->parent->scope);
         
            udisplay(T->ptr[0], indent + 3); //显示for循环第一个表达式——初始化
            udisplay(T->ptr[1], indent + 3); //显示for循环第二个表达式
            udisplay(T->ptr[2], indent + 3); //显示for循环第三个表达式
            break;
        case EXP_FOR3_LIST:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //显示for循环第一个表达式——初始化
            udisplay(T->ptr[1], indent); //显示for循环第二个表达式
            break;
        case DEF_LIST:
            strcpy(T->scope,T->parent->scope);
        
            udisplay(T->ptr[0], indent + 3); //显示局部变量定义列表中的第一个
            udisplay(T->ptr[1], indent + 3); //显示其它局部变量定义
            break;
        case VAR_DEF:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //显示局部变量类型
            udisplay(T->ptr[1], indent); //显示其他局部变量
            break;
        case VARDEC_LIST:
            strcpy(T->scope,T->parent->scope);
            while (T)
            {
                strcpy(T->scope,T->parent->scope);
                udisplay(T->ptr[0], indent);
                T = T->ptr[1];
            }
            break;
        case ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case ADD_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
       
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case MINUS_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
       
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case STAR_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
        
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case DIV_ASSIGNOP:
            strcpy(T->scope,T->parent->scope);
          
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case AND:
            strcpy(T->scope,T->parent->scope);
          
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case OR:
            strcpy(T->scope,T->parent->scope);
         
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case RELOP:
            strcpy(T->scope,T->parent->scope);
    
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case PLUS:
            strcpy(T->scope,T->parent->scope);
    
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case MINUS:
            strcpy(T->scope,T->parent->scope);
      
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case STAR:
            strcpy(T->scope,T->parent->scope);
       
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case DIV:
            strcpy(T->scope,T->parent->scope);
       
            udisplay(T->ptr[0], indent+3); 
            udisplay(T->ptr[1], indent+3);
            break;
        case UMINUS:
            strcpy(T->scope,T->parent->scope);
    
            udisplay(T->ptr[0], indent+3); 
            break;
        case NOT:
            strcpy(T->scope,T->parent->scope);
     
            udisplay(T->ptr[0], indent+3); 
            break;
        case SELFADD:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); 
    
            udisplay(T->ptr[1], indent+6); 
            break;
        case SELFDEC:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); 
      
            udisplay(T->ptr[1], indent+6); 
            break;
        case FUNC_CALL:
            strcpy(T->scope,T->parent->scope);
         
            if (T->ptr[0])
            {
   
                udisplay(T->ptr[0], indent + 6);
            }
            break;
        case INT:
            strcpy(T->scope,T->parent->scope);
        
            break;
        case FLOAT:
            strcpy(T->scope,T->parent->scope);
       
            break;
        case CHAR:
            strcpy(T->scope,T->parent->scope);
    
            break;
        case ARGS:
            while (T)
            {                 
                udisplay(T->ptr[0],indent);
                T = T->ptr[1];
            }
            break;
       
        }
    }
}