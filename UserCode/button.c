#include "STC89.h"
#include "Button.h"
#include "gpio.h"

#define	Debug_SendData	Send_Data1

sbit	Button_IO_0 =	P3 ^ 2;		//按键1接P3.2口
sbit	Button_IO_1	= P3 ^ 3;		//按键2接P3.3口
sbit	Button_IO_2	= P3 ^ 4;	//按键3接P3.4口

#define Button_Count 3			//按键的数量

#define	Button_Click_Time 5				//单击时间
#define	Button_Long_Press_Time 50	//长按时间
#define Button_Timeout_Time 150			//超时时间

static unsigned int Button_Hold_Timer[Button_Count];		//按键按下的时长
static void (*Click_Fun_List[Button_Count])(void);			//单击函数列表
static void (*Long_Press_Fun_List[Button_Count])(void);	//长按函数列表
static void (*Timeout_Fun_List[Button_Count])(void);		//超时函数列表

//初始化相关的变量
void Button_Init(void)
{
    unsigned char i = 0;
    for(i = 0; i < Button_Count; i++)
    {
        Button_Hold_Timer[i] = 0;
        Click_Fun_List[i] = 0;
        Long_Press_Fun_List[i] = 0;
        Timeout_Fun_List[i] = 0;
    }
}


//给按钮添加处理函数，“C”为单击，“L”为长按、“O”为超时
void Button_Set_Fun(unsigned char CH, unsigned char Type, void (*Fun)(void))
{
    if(Type == 'C')
    {
        Click_Fun_List[CH] = Fun;
    }
    if(Type == 'L')
    {
        Long_Press_Fun_List[CH] = Fun;
    }
    if(Type == 'O')
    {
        Timeout_Fun_List[CH] = Fun;
    }
}


//获取按键状态，读取按键电平
static unsigned char Get_Button_Press_Status(unsigned char CH)
{
    if(CH == 0)
        return Button_IO_0;
    if(CH == 1)
        return Button_IO_1;
    if(CH == 2)
        return Button_IO_2;
    return 0;
}


// 根据检测到的点动或者长按关联函数，“C”为单击，“L”为长按、“O”为超时
static void Button_RunFun(unsigned char CH, unsigned char Type)
{
    if(Type == 'C')
    {
        if(Click_Fun_List[CH] == 0)
            return;
        Click_Fun_List[CH]();
    }
    if(Type == 'L')
    {
        if(Long_Press_Fun_List[CH] == 0)
            return;
        Long_Press_Fun_List[CH]();
    }
    if(Type == 'O')
    {
        if(Timeout_Fun_List[CH] == 0)
            return;
        Timeout_Fun_List[CH]();
    }
}


//检测按键业务逻辑，放到10ms循环中不断重复
void Button_Loop(void)
{
    unsigned char i;

    for(i = 0; i < Button_Count; i++)		//循环检测各个按键
    {
        if(Get_Button_Press_Status(i) == 0)		//如果按键被按下
        {

            Button_Hold_Timer[i] ++ ;		//按键按下的时长++

        }
        else		//如果按键没有被按下
        {
            if(Button_Hold_Timer[i] != 0)		//如果按下时长不为零，说明按键曾经被按下过
            {
                if(Button_Hold_Timer[i] < Button_Click_Time || Button_Hold_Timer[i] > Button_Timeout_Time)
                    //小于单击时间为抖动，大于超时时间为超时
                {
                    Button_Hold_Timer[i] = 0;		//按下的时间作废，清零
                    continue;
                }
                if(Button_Hold_Timer[i] >= Button_Click_Time && Button_Hold_Timer[i] < Button_Long_Press_Time)
                    //单击时间与长按时间之间为单击
                {
                    Button_Hold_Timer[i] = 0;
                    Button_RunFun(i, 'C');
                    continue;
                }
                if(Button_Hold_Timer[i] >= Button_Long_Press_Time && Button_Hold_Timer[i] < Button_Timeout_Time)
                    //长按时间与超时时间之间为长按
                {
                    Button_Hold_Timer[i] = 0;
                    Button_RunFun(i, 'L');
                    continue;
                }
            }
            Button_Hold_Timer[i] = 0;
        }
    }
}



