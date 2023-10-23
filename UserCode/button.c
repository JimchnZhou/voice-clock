#include "stc89.h"
#include "button.h"
#include "mode.h"
#include "ds1302.h"
#include "gpio.h"
#include "wt588.h"

sbit Button_IO_0 = P3 ^ 2; // 按键1接P3.2口
sbit Button_IO_1 = P3 ^ 3; // 按键2接P3.3口
sbit Button_IO_2 = P3 ^ 4; // 按键3接P3.4口

#define BUTTON_COUNT 3 // 按键的数量

#define Button_Click_Time 5       // 单击时间
#define Button_Long_Press_Time 20 // 长按时间
#define Button_Timeout_Time 50    // 超时时间

static unsigned int buttonHoldTimer[BUTTON_COUNT];    // 按键按下的时长
static void (*onClickFunList[BUTTON_COUNT])(void);      // 单击函数列表
static void (*longPressFunList[BUTTON_COUNT])(void); // 长按函数列表
static void (*timeoutFunList[BUTTON_COUNT])(void);    // 超时函数列表



// 初始化相关的变量
void buttonInit(void)
{
    unsigned char i = 0;
    for (i = 0; i < BUTTON_COUNT; i++)
    {
        buttonHoldTimer[i] = 0;
        onClickFunList[i] = 0;
        longPressFunList[i] = 0;
        timeoutFunList[i] = 0;
    }
}

/**
 * 函数：给按钮添加处理函数
 *
 * 参数：
 * CH：为哪个按钮添加函数
 * Type：“C”为单击，“L”为长按、“O”为超时
 * Fun：要绑定添加的函数名
 */
void addEventToButton(unsigned char ch, unsigned char type, void (*fun)(void))
{
    if (type == 'C')
    {
        onClickFunList[ch] = fun;
    }
    if (type == 'L')
    {
        longPressFunList[ch] = fun;
    }
    if (type == 'O')
    {
        timeoutFunList[ch] = fun;
    }
}

// 获取按键状态，读取按键电平
static unsigned char getButtonStatus(unsigned char ch)
{
    if (ch == 0)
        return Button_IO_0;
    if (ch == 1)
        return Button_IO_1;
    if (ch == 2)
        return Button_IO_2;
    return 0;
}

// 根据检测到的点动或者长按关联函数，“C”为单击，“L”为长按、“O”为超时
static void Button_RunFun(unsigned char ch, unsigned char type)
{
    if (type == 'C')
    {
        if (onClickFunList[ch] == 0)
            return;
        onClickFunList[ch]();
    }
    if (type == 'L')
    {
        if (longPressFunList[ch] == 0)
            return;
        longPressFunList[ch]();
    }
    if (type == 'O')
    {
        if (timeoutFunList[ch] == 0)
            return;
        timeoutFunList[ch]();
    }
}

// 检测按键业务逻辑，放到系统调度循环中不断重复
void Button_Loop(void)
{
    unsigned char i;

    for (i = 0; i < BUTTON_COUNT; i++) // 循环检测各个按键
    {
        if (getButtonStatus(i) == 0) // 如果按键被按下
        {
            buttonHoldTimer[i]++; // 按键按下的时长++
        }
        else // 如果按键没有被按下 or 按键被按下后放开
        {
            if (buttonHoldTimer[i] != 0) // 如果按下时长不为零，说明按键曾经被按下过
            {
                if (buttonHoldTimer[i] < Button_Click_Time || buttonHoldTimer[i] > Button_Timeout_Time)
                // 小于单击时间为抖动，大于超时时间为超时
                {
                    buttonHoldTimer[i] = 0; // 按下的时间作废，清零
                    continue;
                }
                if (buttonHoldTimer[i] >= Button_Click_Time && buttonHoldTimer[i] < Button_Long_Press_Time)
                // 单击时间与长按时间之间为单击
                {
                    buttonHoldTimer[i] = 0;
                    Button_RunFun(i, 'C');
                    continue;
                }
                if (buttonHoldTimer[i] >= Button_Long_Press_Time && buttonHoldTimer[i] < Button_Timeout_Time)
                // 长按时间与超时时间之间为长按
                {
                    buttonHoldTimer[i] = 0;
                    Button_RunFun(i, 'L');
                    continue;
                }
            }
            buttonHoldTimer[i] = 0;
        }
    }
}

// 按钮 0 单击事件
void button0OnClickEvent()
{
    switch (getRunningMode())
    {
    case 0: // 语音播报时间
        speakTime();
        break;
    case 1: // 保存小时，进入模式 2
        writeTimeToDs1302();
        setRunningMode(2);
        break;
    case 2: // 保存分，进入模式 3
        writeTimeToDs1302();
        setRunningMode(3);
        break;
    case 3: // 保存秒，进入模式 1
        writeTimeToDs1302();
        setRunningMode(1);
        break;
    default:
        break;
    }
}

// 按钮 0 长按事件
void button0LongPressEvent()
{
    switch (getRunningMode())
    {
    case 0: // 进入模式 1
        setRunningMode(1);
        break;
    case 1: // 保存小时，进入模式 0
        writeTimeToDs1302();
        setRunningMode(0);
        break;
    case 2: // 保存分钟，进入模式 0
        writeTimeToDs1302();
        setRunningMode(0);
        break;
    case 3: // 保存秒钟，进入模式 0
        writeTimeToDs1302();
        setRunningMode(0);
        break;
    default:
        break;
    }
}

// 按钮 1 单击
void button1OnClickEvent()
{
    unsigned char timeBuff;
    switch (getRunningMode())
    {
    case 0: // 语音播报温度
        speakTemperature();
        break;
    case 1: // 调时，加
        timeBuff = getNewTimeData(3);
        timeBuff++;
        if (timeBuff > 23)
        {
            timeBuff = 0;
        }
        setNewTimeData(3, timeBuff);
        break;
    case 2: // 调分，加
        timeBuff = getNewTimeData(4);
        timeBuff++;
        if (timeBuff > 59)
        {
            timeBuff = 0;
        }
        setNewTimeData(4, timeBuff);
        break;
    case 3: // 调秒，归零
        setNewTimeData(5, 0);
        break;
    default:
        break;
    }
}

// 按钮 1 长按
void button1LongPressEvent()
{
    switch (getRunningMode())
    {
    case 0:
        break;
    case 1: // 不保存，进入模式2
        setRunningMode(2);
        break;
    case 2: // 不保存，进入模式3
        setRunningMode(3);
        break;
    case 3: // 不保存，进入模式1
        setRunningMode(1);
        break;
    default:
        break;
    }
}

// 按钮 2 单击
void button2OnClickEvent()
{
    unsigned char timeBuff;
    switch (getRunningMode())
    {
    case 0: // 语音播报湿度
        speakHumidity();
        break;
    case 1: // 调时，减
        timeBuff = getNewTimeData(3);
        timeBuff--;
        if (timeBuff > 23)
        {
            timeBuff = 23;
        }
        setNewTimeData(3, timeBuff);
        break;
    case 2: // 调分，减
        timeBuff = getNewTimeData(4);
        timeBuff--;
        if (timeBuff > 59)
        {
            timeBuff = 59;
        }
        setNewTimeData(4, timeBuff);
        break;
    case 3: // 调秒，归零
        setNewTimeData(5, 0);
        break;
    default:
        break;
    }
}

// 按钮 2 长按：退出到默认模式
void button2LongPressEvent()
{
    setRunningMode(0);
}
