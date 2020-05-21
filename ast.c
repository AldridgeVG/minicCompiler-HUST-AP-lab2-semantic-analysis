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
            display(T->ptr[0], indent + 3); //��ʾ�ⲿ�����б�ĵ�һ��
            display(T->ptr[1], indent + 3); //��ʾ�ⲿ�����б��������
            break;
        case EXT_DEF_VAR:
            printf("%*cExtDef_Var: \n", indent, ' ');
            strcpy(T->scope,T->parent->scope);

            display(T->ptr[0], indent + 3); //��ʾ�ⲿ(ȫ��)��������
            display(T->ptr[1], indent + 3); //��ʾȫ�ֱ����б�
            break;
        case EXT_DEF_FUNC:
            printf("%*cExtDef_Func��\n", indent, ' ');
            strcpy(T->scope,T->ptr[1]->type_id);
            display(T->ptr[0], indent + 3); //��ʾ������������
            display(T->ptr[1], indent + 3); //��ʾ�������Ͳ���
            display(T->ptr[2], indent + 3); //��ʾ������
            break;
        //case �ⲿ�ṹ������
        case EXT_VARDEC_LIST:
            strcpy(T->scope,"Global");
            display(T->ptr[0], indent); //������ʾȫ�ֱ�����
            display(T->ptr[1], indent);
            break;
        case TYPE:
            strcpy(T->scope,T->parent->scope);
            //printf("**** %s\n", T->scope);
            printf("%*c%s\n", indent, ' ', T->type_id); //��ʾ��������
            break;
        //�ṹ�嶨�廹δʵ��
        case ID:
            strcpy(T->scope,T->parent->scope);
           //printf("**** %s\n", T->scope);
            printf("%*c%s\n", indent, ' ', T->type_id); //��ʾ������ 
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
            printf("%*cFunc_name��%s\n", indent, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*cFunc_params��\n", indent, ' ');
                display(T->ptr[0], indent + 3); //��ʾ���������б�
            }
            break;
        case FUNC_PARAM_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //������ʾ�������ͺ�����
            display(T->ptr[1], indent);
            break;
        case FUNC_PARAM_DEC:
            strcpy(T->scope,T->parent->scope);
            printf("%*c%s %s\n", indent, ' ', T->ptr[0]->type_id, T->ptr[1]->type_id);
            break;
        case COMP_STM:
            strcpy(T->scope,T->parent->scope);
            printf("%*cComSt: \n",indent,' ');
            display(T->ptr[0], indent + 3); //��ʾ�������岿��
            display(T->ptr[1], indent + 3); //��ʾ��䲿��
            break;
        case COMPSTM_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //��ʾ��һ�����
            display(T->ptr[1], indent); //��ʾʣ�����
            break;
        case COMPSTM_EXP:
            strcpy(T->scope,T->parent->scope);
            // printf("%*cstmt_exp: ", indent, ' ');
            display(T->ptr[0], indent);
            break;
        case RETURN:
            strcpy(T->scope,T->parent->scope);
            printf("%*cstmt_return��\n", indent, ' ');
            display(T->ptr[0], indent + 3);
            break;
        case IF_THEN:
            strcpy(T->scope,"IF of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_IF��\n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾcondition
            display(T->ptr[1], indent + 3); //��ʾif_body
            break;
        case IF_THEN_ELSE:
            strcpy(T->scope,"IF_ELSE of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_IF��\n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾcondition
            // printf("%*cstmt_ifbody:", indent+3, ' ');
            display(T->ptr[1], indent+3); //��ʾif_body
            // printf("%*cstmt_elsebody:", indent+3, ' ');
            display(T->ptr[2], indent+3); //��ʾelse_body
            break;
        case WHILE:
            strcpy(T->scope,"WHILE of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_while:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾcondition
            display(T->ptr[1], indent + 3); //��ʾwhile_body
            break;
        case FOR:
            strcpy(T->scope,"FOR of ");
            strcat(T->scope,T->parent->scope);
            printf("%*cstmt_for:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾcondition
            display(T->ptr[1], indent + 3); //��ʾfor_body
            break;
        case FOR_CONDITION:
            strcpy(T->scope,T->parent->scope);
            printf("%*cstmt_for_condition:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾforѭ����һ�����ʽ������ʼ��
            display(T->ptr[1], indent + 3); //��ʾforѭ���ڶ������ʽ
            display(T->ptr[2], indent + 3); //��ʾforѭ�����������ʽ
            break;
        case EXP_FOR3_LIST:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //��ʾforѭ����һ�����ʽ������ʼ��
            display(T->ptr[1], indent); //��ʾforѭ���ڶ������ʽ
            break;
        case DEF_LIST:
            strcpy(T->scope,T->parent->scope);
            printf("%*cdef_list: \n", indent, ' ');
            display(T->ptr[0], indent + 3); //��ʾ�ֲ����������б��еĵ�һ��
            display(T->ptr[1], indent + 3); //��ʾ�����ֲ���������
            break;
        case VAR_DEF:
            strcpy(T->scope,T->parent->scope);
            display(T->ptr[0], indent); //��ʾ�ֲ���������
            display(T->ptr[1], indent); //��ʾ�����ֲ�����
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
            printf("%*cfunc_name��%s\n", indent + 3, ' ', T->type_id);
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
            udisplay(T->ptr[0], indent + 3); //��ʾ�ⲿ�����б�ĵ�һ��
            udisplay(T->ptr[1], indent + 3); //��ʾ�ⲿ�����б��������
            break;
        case EXT_DEF_VAR:
            strcpy(T->scope,T->parent->scope);

            udisplay(T->ptr[0], indent + 3); //��ʾ�ⲿ(ȫ��)��������
            udisplay(T->ptr[1], indent + 3); //��ʾȫ�ֱ����б�
            break;
        case EXT_DEF_FUNC:
            
            strcpy(T->scope,T->ptr[1]->type_id);
            udisplay(T->ptr[0], indent + 3); //��ʾ������������
            udisplay(T->ptr[1], indent + 3); //��ʾ�������Ͳ���
            udisplay(T->ptr[2], indent + 3); //��ʾ������
            break;
        //case �ⲿ�ṹ������
        case EXT_VARDEC_LIST:
            strcpy(T->scope,"Global");
            udisplay(T->ptr[0], indent); //������ʾȫ�ֱ�����
            udisplay(T->ptr[1], indent);
            break;
        case TYPE:
            strcpy(T->scope,T->parent->scope);
            //printf("**** %s\n", T->scope);
           
            break;
        //�ṹ�嶨�廹δʵ��
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
               
                udisplay(T->ptr[0], indent + 3); //��ʾ���������б�
            }
            break;
        case FUNC_PARAM_LIST:
            strcpy(T->scope,"Para of ");
            strcat(T->scope,T->parent->scope);
            //printf("******* %s %s\n", T->scope,T->type_id);
            udisplay(T->ptr[0], indent); //������ʾ�������ͺ�����
            udisplay(T->ptr[1], indent);
            break;
        case FUNC_PARAM_DEC:
            strcpy(T->scope,T->parent->scope);
           
            break;
        case COMP_STM:
            strcpy(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent + 3); //��ʾ�������岿��
            udisplay(T->ptr[1], indent + 3); //��ʾ��䲿��
            break;
        case COMPSTM_LIST:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //��ʾ��һ�����
            udisplay(T->ptr[1], indent); //��ʾʣ�����
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
          
            udisplay(T->ptr[0], indent + 3); //��ʾcondition
            udisplay(T->ptr[1], indent + 3); //��ʾif_body
            break;
        case IF_THEN_ELSE:
            strcpy(T->scope,"IF_ELSE of ");
            strcat(T->scope,T->parent->scope);
         
            udisplay(T->ptr[0], indent + 3); //��ʾcondition
            // printf("%*cstmt_ifbody:", indent+3, ' ');
            udisplay(T->ptr[1], indent+3); //��ʾif_body
            // printf("%*cstmt_elsebody:", indent+3, ' ');
            udisplay(T->ptr[2], indent+3); //��ʾelse_body
            break;
        case WHILE:
            strcpy(T->scope,"WHILE of ");
            strcat(T->scope,T->parent->scope);
           
            udisplay(T->ptr[0], indent + 3); //��ʾcondition
            udisplay(T->ptr[1], indent + 3); //��ʾwhile_body
            break;
        case FOR:
            strcpy(T->scope,"FOR of ");
            strcat(T->scope,T->parent->scope);
          
            udisplay(T->ptr[0], indent + 3); //��ʾcondition
            udisplay(T->ptr[1], indent + 3); //��ʾfor_body
            break;
        case FOR_CONDITION:
            strcpy(T->scope,T->parent->scope);
         
            udisplay(T->ptr[0], indent + 3); //��ʾforѭ����һ�����ʽ������ʼ��
            udisplay(T->ptr[1], indent + 3); //��ʾforѭ���ڶ������ʽ
            udisplay(T->ptr[2], indent + 3); //��ʾforѭ�����������ʽ
            break;
        case EXP_FOR3_LIST:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //��ʾforѭ����һ�����ʽ������ʼ��
            udisplay(T->ptr[1], indent); //��ʾforѭ���ڶ������ʽ
            break;
        case DEF_LIST:
            strcpy(T->scope,T->parent->scope);
        
            udisplay(T->ptr[0], indent + 3); //��ʾ�ֲ����������б��еĵ�һ��
            udisplay(T->ptr[1], indent + 3); //��ʾ�����ֲ���������
            break;
        case VAR_DEF:
            strcpy(T->scope,T->parent->scope);
            udisplay(T->ptr[0], indent); //��ʾ�ֲ���������
            udisplay(T->ptr[1], indent); //��ʾ�����ֲ�����
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