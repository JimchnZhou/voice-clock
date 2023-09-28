/*
基本数字：
	零、一、二、三、四、五、六、七、八、九、十/时、百、千、万、亿
	甲、乙、丙、丁、戊、己、庚、辛、壬、癸
	子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥

辅助音：
	欢迎使用、智能、语音、电子、时钟、门锁
	系统、网络、正在、初始化、连接、请稍后
	成功、失败、错误、请再试一次
	
功能转换：
	时间、温度、湿度、设置、模式

时间：
	现在是、北京时间
	年、月、日、天、点、分、秒
	凌晨、早上、上午、中午、下午、傍晚、晚上、深夜
	整点、半点、报时、闹钟
	农历、闰、初
	
门锁：
	请输入、管理员、指纹、密码、请刷卡、验证、请开门、电量过低
	
计算器：
	加、减、乘、除、等于、归零
*/

//WT588F一线单字节控制程序
void One_Wire_Send_Byte(unsigned char Byte_Value);

//WT588F一线双字节控制程序
void One_Wire_Send_Double(unsigned int Double_Value);

//WT588F一线单字节连码示例(F3 + 01 + F3 + 02)
Void List_1A_Play_WT588F(void);

//WT588F一线双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
Void List_1A_Play_WT588F_Couple(void);

//WT588F二线单字节控制程序
void Two_Wire_Send_Byte(unsigned char Byte_Value);
//WT588F二线双字节控制程序
void Two_Wire_Send_Double(unsigned int Double_Value);

//WT588F二线单字节连码示例(F3 + 01 + F3 + 02)
void List_2A_Play_WT588F(void);

//WT588F二线双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_2A_Play_WT588F_Couple(void);