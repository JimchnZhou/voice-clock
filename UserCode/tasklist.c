#include "tasklist.h"

#define	Task_List_Count	1							//任务总数量
unsigned char Task_Query_Flag = 0;		//任务列表查询标志

struct
{
    unsigned char Task_Handler_Flag[Task_List_Count];	//任务处理标志，标1代表任务需处理
    void (*Fun_Point_List[Task_List_Count])(void);		//任务列表关联的函数
    unsigned int Counter[Task_List_Count];
    unsigned int Timer[Task_List_Count];
} Task_Struct;

//初始化任务列表
void Task_List_Init(void)
{
    unsigned char i = 0;
    for(i = 0; i < Task_List_Count; i++)
    {
        Task_Struct.Task_Handler_Flag[i] = 0;
        Task_Struct.Counter[i] = 0;
    }
}

//添加一个函数到任务列表中
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

//轮询任务列表
void Polling_Task_List(void)
{
    unsigned char i = 0, j = 0;
    if(Task_Query_Flag == 0)								//无需查询任务，直接return
    {
        return;
    }
    else
    {
        Task_Query_Flag = 0;								//清除任务查询标志
        for(i = 0; i < Task_List_Count; i++) 	//查询任务列表
        {
            if(Task_Struct.Task_Handler_Flag[i])						//任务需处理
            {
                Task_Struct.Counter[i] ++ ;
                if(Task_Struct.Counter[i] > Task_Struct.Timer[i])		//任务需执行
                {
                    Task_Struct.Counter[i] = 0;
                    Task_Struct.Fun_Point_List[i]();
                }
            }
        }
    }

}

//使能任务列表查询，使能后系统主循环将查询一遍任务列表
void Task_Query_Flag_Enable(void)
{
    Task_Query_Flag = 1;
}

