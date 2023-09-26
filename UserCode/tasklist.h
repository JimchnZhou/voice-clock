#ifndef	__DEBUG_H__
#define __DEBUG_H__

//初始化任务列表
void Task_List_Init(void);

//添加一个函数到任务列表中
unsigned char Task_List_Add_Fun(unsigned int Time, void (*Fun)(void));

//轮询任务列表
void Polling_Task_List(void);

//使能任务列表查询
void Task_Query_Flag_Enable(void);

#endif

