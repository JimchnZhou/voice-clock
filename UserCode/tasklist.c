#include "tasklist.h"

#define	Task_List_Count	1							//����������
unsigned char Task_Query_Flag = 0;		//�����б��ѯ��־

struct
{
    unsigned char Task_Handler_Flag[Task_List_Count];	//�������־����1���������账��
    void (*Fun_Point_List[Task_List_Count])(void);		//�����б�����ĺ���
    unsigned int Counter[Task_List_Count];
    unsigned int Timer[Task_List_Count];
} Task_Struct;

//��ʼ�������б�
void Task_List_Init(void)
{
    unsigned char i = 0;
    for(i = 0; i < Task_List_Count; i++)
    {
        Task_Struct.Task_Handler_Flag[i] = 0;
        Task_Struct.Counter[i] = 0;
    }
}

//���һ�������������б���
unsigned char Task_List_Add_Fun(unsigned int Time, void (*Fun)(void))
{
    unsigned char i = 0;
    for(i = 0; i < Task_List_Count; i++)
    {
        if(Task_Struct.Task_Handler_Flag[i] == 0)
        {
            Task_Struct.Task_Handler_Flag[i] = 1;
            Task_Struct.Counter[i] = 0;
            Task_Struct.Fun_Point_List[i] = Fun;
            Task_Struct.Timer[i] = Time - 1;
            return 1;
        }
    }
    return 0;
}

//��ѯ�����б�
void Polling_Task_List(void)
{
    unsigned char i = 0, j = 0;
    if(Task_Query_Flag == 0)								//�����ѯ����ֱ��return
    {
        return;
    }
    else
    {
        Task_Query_Flag = 0;								//��������ѯ��־
        for(i = 0; i < Task_List_Count; i++) 	//��ѯ�����б�
        {
            if(Task_Struct.Task_Handler_Flag[i])						//�����账��
            {
                Task_Struct.Counter[i] ++ ;
                if(Task_Struct.Counter[i] > Task_Struct.Timer[i])		//������ִ��
                {
                    Task_Struct.Counter[i] = 0;
                    Task_Struct.Fun_Point_List[i]();
                }
            }
        }
    }

}

//ʹ�������б��ѯ��ʹ�ܺ�ϵͳ��ѭ������ѯһ�������б�
void Task_Query_Flag_Enable(void)
{
    Task_Query_Flag = 1;
}

