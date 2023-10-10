/*
基本数字：
0-14:		零、一、二、三、四、五、六、七、八、九、十/时、百、千、万、亿
15-24:	甲、乙、丙、丁、戊、己、庚、辛、壬、癸
25-36:	子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥

辅助音：
37-42:	欢迎使用、智能、语音、电子、时钟、门锁
43-48：	系统、网络、正在、初始化、连接、请稍后
49-52：	成功、失败、错误、请再试一次
53-58：	时间、温度、湿度、设置、模式、音量

时间：
59-60：	现在是、北京时间
61-68：	年、月、日、天、点、分、秒、整
69-76：	凌晨、早上、上午、中午、下午、傍晚、晚上、深夜
77-80：	整点、半点、报时、闹钟
81-84：	星期、农历、闰、初

门锁：
85-93:	请输入、管理员、指纹、密码、请刷卡、验证、请开门、电量过低、警告

计算器：
94-99:	加、减、乘、除、等于、归零
*/

// WT588F一线单字节控制程序
void One_Wire_Send_Byte(unsigned char Byte_Value);

// WT588F一线双字节控制程序
void One_Wire_Send_Double(unsigned int Double_Value);

// WT588F一线单字节连码示例(F3 + 01 + F3 + 02)
void List_1A_Play_WT588F(void);

// WT588F一线双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_1A_Play_WT588F_Couple(void);

// WT588F二线单字节控制程序
void Two_Wire_Send_Byte(unsigned char Byte_Value);

// WT588F二线双字节控制程序
void Two_Wire_Send_Double(unsigned int Double_Value);

// WT588F二线单字节连码示例(F3 + 01 + F3 + 02)
void List_2A_Play_WT588F(void);

// WT588F二线双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_2A_Play_WT588F_Couple(void);
