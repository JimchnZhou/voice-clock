#ifndef	__DEBUG_H__
#define __DEBUG_H__

//��ʼ�������б�
void Task_List_Init(void);

//���һ�������������б���
unsigned char Task_List_Add_Fun(unsigned int Time, void (*Fun)(void));

//��ѯ�����б�
void Polling_Task_List(void);

//ʹ�������б��ѯ
void Task_Query_Flag_Enable(void);

#endif

