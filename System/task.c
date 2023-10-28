#include "task.h"

#define SYS_LoopFun_ListCount 5    // 循环函数列表行数，决定同时运行几个循环函数
// #define SYS_DelayFun_ListCount 1   // 延时函数列表行数，决定同时运行几个延时函数
// #define SYS_MessageFun_ListCount 1 // 消息函数列表行数，决定同时运行几个消息函数

struct
{
    unsigned char Flag[SYS_LoopFun_ListCount];
    void (*Fun_Point_List[SYS_LoopFun_ListCount])(void);
    unsigned long Counter[SYS_LoopFun_ListCount];
    unsigned long Timer[SYS_LoopFun_ListCount];
} SYS_LoopFun_Struct;

// struct
// {
//     unsigned char Flag[SYS_DelayFun_ListCount];
//     void (*Fun_Point_List[SYS_DelayFun_ListCount])(void);
//     unsigned long Counter[SYS_DelayFun_ListCount];
//     unsigned long Timer[SYS_DelayFun_ListCount];
// } SYS_DelayFun_Struct;

// struct
// {
//     unsigned char MessageQueue[SYS_MessageFun_ListCount];
//     unsigned char MessageList[SYS_MessageFun_ListCount];
//     void (*MessageFun_Point_List[SYS_MessageFun_ListCount])(void);
// } SYS_MessageFun_Struct;

static unsigned char SYS_LoopFun_Interrupt_Flag = 0;
// static unsigned char SYS_MessageFun_Interrupt_Flag = 0;
// static unsigned char SYS_DelayFun_Interrupt_Flag = 0;

/********************************************************************************************/

// 添加事件到循环执行函数列表，成功返回 0，失败返回 -1
unsigned char addEventToLoopList(unsigned long Time, void (*Fun)(void))
{
    unsigned char i = 0;
    for (i = 0; i < SYS_LoopFun_ListCount; i++)
    {
        if (SYS_LoopFun_Struct.Flag[i] == 0)
        {
            SYS_LoopFun_Struct.Flag[i] = 1;
            SYS_LoopFun_Struct.Counter[i] = 0;
            SYS_LoopFun_Struct.Fun_Point_List[i] = Fun;
            SYS_LoopFun_Struct.Timer[i] = Time - 1;
            return 0;
        }
    }
    return -1;
}

// 从循环执行函数列表删除事件，成功返回 0，失败返回 -1
unsigned char delEventFromLoopList(void (*Fun)(void))
{
    unsigned char i = 0;
    for (i = 0; i < SYS_LoopFun_ListCount; i++)
    {
        if (SYS_LoopFun_Struct.Fun_Point_List[i] == Fun)
        {
            SYS_LoopFun_Struct.Flag[i] = 0;
            SYS_LoopFun_Struct.Counter[i] = 0;
            SYS_LoopFun_Struct.Fun_Point_List[i] = 0;
            SYS_LoopFun_Struct.Timer[i] = 0;
            return 1;
        }
    }
    return 0;
}

// // 添加事件到延时执行函数列表，成功返回 0，失败返回 -1
// unsigned char addEventToDelayList(unsigned long Time, void (*Fun)(void))
// {
//     unsigned char i = 0;
//     for (i = 0; i < SYS_DelayFun_ListCount; i++)
//     {
//         if (SYS_DelayFun_Struct.Flag[i] == 0)
//         {
//             SYS_DelayFun_Struct.Flag[i] = 1;
//             SYS_DelayFun_Struct.Counter[i] = 0;
//             SYS_DelayFun_Struct.Fun_Point_List[i] = Fun;
//             SYS_DelayFun_Struct.Timer[i] = Time - 1;
//             return 0;
//         }
//     }
//     return -1;
// }

// // 添加事件到消息执行函数列表，成功返回 0，失败返回 -1
// unsigned char addEventToMessageList(unsigned char Message, void (*Fun)(void))
// {
//     unsigned char i;
//     for (i = 0; i < SYS_MessageFun_ListCount; i++)
//     {
//         if (!SYS_MessageFun_Struct.MessageList[i])
//         {
//             SYS_MessageFun_Struct.MessageList[i] = Message;
//             SYS_MessageFun_Struct.MessageFun_Point_List[i] = Fun;
//             return 1;
//         }
//     }
//     return 0;
// }

// // 从消息执行函数列表删除事件，成功返回 0，失败返回 -1
// unsigned char delEventFromMessageList(void (*Fun)(void))
// {
//     unsigned char i;
//     for (i = 0; i < SYS_MessageFun_ListCount; i++)
//     {
//         if (SYS_MessageFun_Struct.MessageFun_Point_List[i] == Fun)
//         {
//             SYS_MessageFun_Struct.MessageList[i] = 0;
//             SYS_MessageFun_Struct.MessageFun_Point_List[i] = 0;
//             return 1;
//         }
//     }
//     return 0;
// }

// // 发送一个系统消息
// unsigned char sendASystemMessage(unsigned char Message)
// {
//     unsigned char i;
//     for (i = 0; i < SYS_MessageFun_ListCount; i++)
//     {
//         if (!SYS_MessageFun_Struct.MessageQueue[i])
//         {
//             SYS_MessageFun_Struct.MessageQueue[i] = Message;
//             return 1;
//         }
//     }
//     return 0;
// }

/********************************************************************************************/

// 系统定时器中断里面执行的函数
static void SYS_LoopFun_LOOP(void)
{
    unsigned char i = 0, j = 0;
    if (SYS_LoopFun_Interrupt_Flag == 0)
        return;
    SYS_LoopFun_Interrupt_Flag = 0;
    for (i = 0; i < SYS_LoopFun_ListCount; i++) // 循环定时的函数
    {
        if (SYS_LoopFun_Struct.Flag[i])
        {
            SYS_LoopFun_Struct.Counter[i]++;
            if (SYS_LoopFun_Struct.Counter[i] > SYS_LoopFun_Struct.Timer[i])
            {
                SYS_LoopFun_Struct.Counter[i] = 0;
                SYS_LoopFun_Struct.Fun_Point_List[i]();
            }
        }
    }
}

// // 系统循环执行-邮箱处理
// static void SYS_MessageFun_LOOP(void)
// {
//     unsigned char i = 0, j = 0;
//     if (SYS_MessageFun_Interrupt_Flag == 0)
//         return;
//     SYS_MessageFun_Interrupt_Flag = 0;

//     for (i = 0; i < SYS_MessageFun_ListCount; i++) // 调用消息队列中的函数
//     {
//         if (SYS_MessageFun_Struct.MessageQueue[i])
//         {
//             for (j = 0; j < SYS_MessageFun_ListCount; j++)
//             {
//                 if (SYS_MessageFun_Struct.MessageQueue[i] == SYS_MessageFun_Struct.MessageList[j])
//                 {
//                     SYS_MessageFun_Struct.MessageFun_Point_List[j]();
//                     j = SYS_MessageFun_ListCount + 10;
//                 }
//             }
//             SYS_MessageFun_Struct.MessageQueue[i] = 0;
//         }
//     }
// }

// // 系统循环，单次执行函数处理
// static void SYS_DelayFun_LOOP(void)
// {
//     unsigned char i = 0, j = 0;
//     if (SYS_DelayFun_Interrupt_Flag == 0)
//         return;
//     SYS_DelayFun_Interrupt_Flag = 0;

//     for (i = 0; i < SYS_DelayFun_ListCount; i++) // 一次性定时函数
//     {
//         if (SYS_DelayFun_Struct.Flag[i])
//         {
//             SYS_DelayFun_Struct.Counter[i]++;
//             if (SYS_DelayFun_Struct.Counter[i] > SYS_DelayFun_Struct.Timer[i])
//             {
//                 SYS_DelayFun_Struct.Counter[i] = 0;
//                 SYS_DelayFun_Struct.Flag[i] = 0;
//                 SYS_DelayFun_Struct.Fun_Point_List[i]();
//                 SYS_DelayFun_Struct.Flag[i] = 0;
//             }
//         }
//     }
// }

// 初始化内存数据
void loopListInit(void)
{
    unsigned char i = 0;
    for (i = 0; i < SYS_LoopFun_ListCount; i++)
    {
        SYS_LoopFun_Struct.Flag[i] = 0;
        SYS_LoopFun_Struct.Counter[i] = 0;
    }
    //    for(i = 0; i < SYS_DelayFun_ListCount; i++)
    //    {
    //        SYS_DelayFun_Struct.Flag[i] = 0;
    //        SYS_DelayFun_Struct.Counter[i] = 0;
    //    }
    //    for(i = 0; i < SYS_MessageFun_ListCount; i++)
    //    {
    //        SYS_MessageFun_Struct.MessageQueue[i] = 0;
    //        SYS_MessageFun_Struct.MessageList[i] = 0;
    //    }
}

// 在Main函数中执行  内部带while循环
void SYS_LOOP_While_APP(void)
{
    while (1)
    {
        SYS_LoopFun_LOOP();
        //        SYS_MessageFun_LOOP();
        //        SYS_DelayFun_LOOP();
    }
}

// 放到定时器中断中的函数
void SYS_TimerInterrupt_App(void)
{
    SYS_LoopFun_Interrupt_Flag = 1;
    //    SYS_MessageFun_Interrupt_Flag = 1;
    //    SYS_DelayFun_Interrupt_Flag = 1;
}
//
