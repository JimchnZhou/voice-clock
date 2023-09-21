
/********************************** (C) COPYRIGHT *******************************
* File Name          :CompatibilityHID.C
* Author             : WCH
* Version            : V1.2
* Date               : 2018/02/28
* Description        : CH554模拟HID兼容设备，支持中断上下传，支持控制端点上下传，支持设置全速，低速
*******************************************************************************/

#include "CH554.H"
#include "hid.h"
#include "delay.h"
#include "tasklist.h"
#include <string.h>
#include "GPIO.H"

#define THIS_ENDP0_SIZE         64		// 端点数据包大小

UINT8X  Ep0Buffer[8 > (THIS_ENDP0_SIZE + 2) ? 8 : (THIS_ENDP0_SIZE + 2)] _at_ 0x0000; //端点0 OUT&IN缓冲区，必须是偶地址
UINT8X  Ep2Buffer[64 > (MAX_PACKET_SIZE + 2) ? 64 : (MAX_PACKET_SIZE + 2)] _at_ 0x0050; //端点2 IN缓冲区,必须是偶地址

UINT8   SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;
PUINT8  pDescr;                                                                    //USB配置标志
USB_SETUP_REQ   SetupReqBuf;                                                       //暂存Setup包
#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)

/*设备描述符*/
UINT8C DevDesc[18] =
{
    0x12,							//bLength							描述符长度（18字节0x12）
    0x01,							//bDescriptorType			描述符类型（设备描述符0x01）
    0x10, 0x01, 			//bcdUSB							USB HID协议版本（1.10）
    0x00,							//bDeviceClass				类代码，HID类是在接口描述符中定义的
    0x00,							//bDeviceSubClass			子类代码，这些代码由 bDeviceClass 字段的值限定
    0x00,							//bDeviceProtocol			协议代码，这些代码由 bDeviceSubClass 字段的值限定
    THIS_ENDP0_SIZE,	//bMaxPacketSize0			端点0的最大数据包大小（8、16、32 或 64）
    0x3d, 0x41, 			//idVendor						厂商ID（0xFFFF）
    0x07, 0x21, 			//idProduct						产品ID（0x0001）
    0x00, 0x00, 			//bcdDevice						设备版本号（0x0100）
    0x00,							//iManufacturer				描述厂商的字符串的索引
    0x00,							//iProduct						描述产品的字符串的索引
    0x00,							//iSerialNumber				产品序列号字符串的索引
    0x01							//bNumConfigurations	可能的配置数

};

/*配置描述符*/
UINT8C CfgDesc[41] =
{
    // 配置描述符
    0x09,				//bLength							描述符长度（9字节0x09）
    0x02, 			//bDescriptorType			描述符类型（配置描述符0x02）
    0x29, 0x00,	//wTotalLength				配置描述符集合总长度(34字节0x0022 41字节0x0029)
    0x01,				//bNumInterfaces			配置支持的接口数
    0x01,				//bConfigurationValue	配置的值
    0x00,				//iConfiguration			描述该配置的字符串的索引值，无0x00
    0xA0,				//bmAttributes				该设备的属性，7总线供电 6自供电 5远程唤醒 4..0保留，10100000 B
    0x32,				//MaxPower						设备所需的电流，单位2mA（100mA = 0x32）

    // 接口描述符
    0x09,				//bLength							描述符的长度（9字节0x09）
    0x04,				//bDescriptorType			描述符类型（接口描述符为0x04）
    0x00,				//bInterfaceNumber		该接口的编号（从0开始）
    0x00,				//bAlternateSetting		该接口的备用编号
    0x01,				//bNumEndpoints				此接口使用的端点数，不包括端点零，如果此值为零，则此接口仅使用端点零。
    0x03,				//bInterfaceClass			该接口使用的类
    0x01,				//bInterfaceSubClass	该接口使用的子类，0没有子类，1引导接口子类
    0x00,				//bInterfaceProtocol	该接口使用的协议，0无，1键盘，2鼠标
    0x00,				//iInterface					描述该接口的字符串的索引值

    // HID类描述符
    0x09, 			//bLength							描述符的长度（长度由下级描述符多少决定）
    0x21, 			//bDescriptorType			描述符类型（HID描述符为0x21）
    0x11, 0x01, //bcdHID							HID协议版本
    0x00, 			//bCountryCode				国家代码
    0x01, 			//bNumDescriptors			要遵循的HID类描述符的数量。 0x01
    0x22, 			//bDescriptorType			报告描述符类型
    0x36, 0x00,	//wDescriptorLength		报告描述符的总长度52=0x34,56=0x38,68=0x44

    // 端点描述符
    0x07,				//bLength							描述符的长度（7字节）
    0x05,				//bDescriptorType			描述符类型（端点描述符为0x05）
    0x82,				//bEndpointAddress		该端点的地址 10000010B
    0x03,				//bmAttributes				该端点的属性，位 0..1 传输类型，所有其他位都保留。00000011B
    0x08, 0x00,	//wMaxPacketSize			该端点支持的最大包长
    0x0a,				//bInterval						端点的查询时间

    // 端点描述符
    0x07,				//bLength							描述符的长度（7字节）
    0x05,				//bDescriptorType			描述符类型（端点描述符为0x05）
    0x82,				//bEndpointAddress		该端点的地址 10000010B
    0x03,				//bmAttributes				该端点的属性，位 0..1 传输类型，所有其他位都保留。00000011B
    0x08, 0x00,	//wMaxPacketSize			该端点支持的最大包长
    0x0a,				//bInterval						端点的查询时间
};

/*字符串描述符 略*/

/*HID类报表描述符*/
UINT8C HIDRepDesc[54] =
{
    0x05, 0x01, 			// 使用页面（通用桌面）
    0x09, 0x0e, 			// 用法（系统多轴控制器）
    0xa1, 0x01, 			// 集合（应用程序）
    0x05, 0x0d, 			//  使用页面（数字化仪）
    0x09, 0x21, 			//  用法（Puck）
    0xa1, 0x00, 			//  集合（物理）
    0x05, 0x01, 			//   使用页面（通用桌面）
    0x09, 0x37, 			//   用法（拨号）
    0x95, 0x01, 			//   数据域数量 (1)
    0x75, 0x0f, 			//   数据域长度（15）
    0x55, 0x0f, 			//   单位指数 (-1)
    0x65, 0x14, 			//   单位（度数，英制旋转）
    0x36, 0xf0, 0xf1, //   物理最小值 (-3600)
    0x46, 0x10, 0x0e, //   物理最大值 (3600)
    0x16, 0xf0, 0xf1, //   逻辑最小值 (-3600)
    0x26, 0x10, 0x0e, //   逻辑最大值 (3600)
    0x81, 0x06, 			//   输入（数据，变量，相对）
    0x05, 0x09, 			//   使用页面（按钮）
    0x09, 0x01, 			//   用法（按钮 1）
    0x95, 0x01, 			//   数据域数量 (1)
    0x75, 0x01, 			//   数据域长度（1）
    0x15, 0x00, 			//   逻辑最小值（0）
    0x25, 0x01, 			//   逻辑最大值 (1)
    0x81, 0x02, 			//   输入（数据，变量，绝对值）
    0xc0, 						//  结束集合
    0xc0, 						// 结束集合
};

// unsigned char  code LangDes[]={0x04,0x03,0x09,0x04};           //语言描述符
// unsigned char  code SerDes[]={
//                           0x28,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//                           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//                           0x00,0x00,0x00,0x00,0x00,0x49,0x00,0x43,0x00,0x42,
//                           0x00,0x43,0x00,0x31,0x00,0x00,0x00,0x00,0x00,0x00
//                           };                                   //字符串描述符

UINT8X UserEp2Buf[2] = {0x00, 0x00};                                            //用户数据定义

UINT8 Endp2Busy = 0;

/*******************************************************************************
* Function Name  : USB_Device_Init()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_Device_Init(void)
{
    IE_USB = 0;																																 // 关闭USB中断
    USB_CTRL = 0x00;                                                           // USB控制寄存器初始化：设备模式、全速12Mbps
    UDEV_CTRL = bUD_PD_DIS;                                                    // 禁止DP/DM下拉电阻

    UDEV_CTRL &= ~bUD_LOW_SPEED;                                               //选择全速12M模式，默认方式
    USB_CTRL &= ~bUC_LOW_SPEED;

    UEP2_DMA = Ep2Buffer;                                                      //端点2数据传输地址
    UEP2_3_MOD |= bUEP2_TX_EN | bUEP2_RX_EN;                                   //端点2发送接收使能
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;                                              //端点2收发各64字节缓冲区
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
    UEP0_DMA = Ep0Buffer;                                                      //端点0数据传输地址
    UEP4_1_MOD &= ~(bUEP4_RX_EN | bUEP4_TX_EN);                                //端点0单64字节收发缓冲区
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT事务返回ACK，IN事务返回NAK

    USB_DEV_AD = 0x00;
    USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                     // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
    UDEV_CTRL |= bUD_PORT_EN;                                                  // 允许USB端口
    USB_INT_FG = 0xFF;                                                         // 清中断标志
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    IE_USB = 1;
}

/*******************************************************************************
* Function Name  : Enp2BlukIn()
* Description    : USB设备模式端点2的批量上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2BlukIn()
{
    memcpy(Ep2Buffer + MAX_PACKET_SIZE, UserEp2Buf, sizeof(UserEp2Buf));       //加载上传数据
    UEP2_T_LEN = sizeof(UserEp2Buf);                                              //上传最大包长度
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                  //有数据时上传数据并应答ACK
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB中断处理函数
*******************************************************************************/
void DeviceInterrupt(void) interrupt INT_NO_USB using 1                      //USB中断服务程序,使用寄存器组1
{
    UINT8 len, i;

    // UIF_TRANSFER：USB传输完成标志
    if(UIF_TRANSFER)                                                		//如果是USB传输完成
    {
        // 判断事物令牌和端点号
        switch(USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
            // USB_INT_ST：			USB中断状态寄存器
            // MASK_UIS_TOKEN；	前事务令牌PID标识：bUIS_TOKEN1 和 bUIS_TOKEN0 组成，用于标识当前USB传输事务的令牌PID：00表示OUT包；01表示SOF包；10表示IN包；11表示SETUP 包。
            // MASK_UIS_ENDP：	传输事务的端点号：0000表示端点0；…；1111表示端点15
        {

        // 端点2收到IN令牌包
        case UIS_TOKEN_IN | 2:                                         		// endpoint 2# 端点批量上传
            UEP2_T_LEN = 0;                                            		// 预使用发送长度一定要清空
//            UEP1_CTRL ^= bUEP_T_TOG;                                 		// 如果不设置自动翻转则需要手动翻转
            Endp2Busy = 0 ;
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;  		// 默认应答NAK
            break;

        // 端点2收到OUT令牌包
        case UIS_TOKEN_OUT | 2:                                        		// endpoint 2# 端点批量下传
            if(U_TOG_OK)                                                	// 不同步的数据包将丢弃
            {
                len = USB_RX_LEN;                                         //接收数据长度，数据从Ep2Buffer首地址开始存放
                for(i = 0; i < len; i ++)
                {
                    Ep2Buffer[MAX_PACKET_SIZE + i] = Ep2Buffer[i] ^ 0xFF; // OUT数据取反到IN由计算机验证
                }
                UEP2_T_LEN = len;
                UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; // 允许上传
            }
            break;

        //端点0收到SETUP令牌包
        case UIS_TOKEN_SETUP | 0:
            len = USB_RX_LEN;                      //USB_RX_LEN：USB接收长度寄存器

            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                len = 0;                                                         // 默认为成功并且上传0长度
                SetupReq = UsbSetupBuf->bRequest;

                if((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD)     /*HID类命令*/
                {
                    switch(SetupReq)
                    {
                    case 0x01:                                                  //GetReport
                        pDescr = UserEp2Buf;                                    //控制端点上传输据
                        if(SetupLen >= THIS_ENDP0_SIZE)                         //大于端点0大小，需要特殊处理
                        {
                            len = THIS_ENDP0_SIZE;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                        break;
                    case 0x02:                                                   //GetIdle
                        break;
                    case 0x03:                                                   //GetProtocol
                        break;
                    case 0x09:                                                   //SetReport
                        break;
                    case 0x0A:                                                   //SetIdle
                        break;
                    case 0x0B:                                                   //SetProtocol
                        break;
                    default:
                        len = 0xFF;  				                              /*命令不支持*/
                        break;
                    }
                    if(SetupLen > len)
                    {
                        SetupLen = len;    //限制总长度
                    }
                    len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;   //本次传输长度
                    memcpy(Ep0Buffer, pDescr, len);                                   //加载上传数据
                    SetupLen -= len;
                    pDescr += len;
                }
                else                                                             //标准请求
                {
                    switch(SetupReq)                                             //请求码
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                                  //设备描述符
                            pDescr = DevDesc;                                    //把设备描述符送到要发送的缓冲区
                            len = sizeof(DevDesc);
                            break;

                        case 2:                                                  //配置描述符
                            pDescr = CfgDesc;                                    //把设备描述符送到要发送的缓冲区
                            len = sizeof(CfgDesc);
                            break;

                        case 0x22:                                               //报表描述符
                            pDescr = HIDRepDesc;                                 //数据准备上传
                            len = sizeof(HIDRepDesc);
                            break;

                        default:
                            len = 0xff;                                          //不支持的命令或者出错
                            break;
                        }
                        if(SetupLen > len)
                        {
                            SetupLen = len;    //限制总长度
                        }
                        len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;//本次传输长度
                        memcpy(Ep0Buffer, pDescr, len);                          //加载上传数据
                        SetupLen -= len;
                        pDescr += len;
                        break;

                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                         //暂存USB设备地址
                        break;

                    case USB_GET_CONFIGURATION:
                        Ep0Buffer[0] = UsbConfig;
                        if(SetupLen >= 1)
                        {
                            len = 1;
                        }
                        break;

                    case USB_SET_CONFIGURATION:
                        UsbConfig = UsbSetupBuf->wValueL;
                        if(UsbConfig)
                        {
                            Ready = 1;                                            //set config命令一般代表usb枚举完成的标志
                        }
                        break;

                    case 0x0A:
                        break;

                    case USB_CLEAR_FEATURE:                                      //Clear Feature
                        if((UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)     // 端点
                        {
                            switch(UsbSetupBuf->wIndexL)
                            {
                            case 0x82:
                                UEP2_CTRL = UEP2_CTRL & ~(bUEP_T_TOG | MASK_UEP_T_RES) | UEP_T_RES_NAK;
                                break;
                            case 0x81:
                                UEP1_CTRL = UEP1_CTRL & ~(bUEP_T_TOG | MASK_UEP_T_RES) | UEP_T_RES_NAK;
                                break;
                            case 0x02:
                                UEP2_CTRL = UEP2_CTRL & ~(bUEP_R_TOG | MASK_UEP_R_RES) | UEP_R_RES_ACK;
                                break;
                            default:
                                len = 0xFF;                                       // 不支持的端点
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                           // 不是端点不支持
                        }
                        break;

                    case USB_SET_FEATURE:                                         /* Set Feature */
                        if((UsbSetupBuf->bRequestType & 0x1F) == 0x00)            /* 设置设备 */
                        {
                            if((((UINT16)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x01)
                            {
                                if(CfgDesc[ 7 ] & 0x20)
                                {
                                    /* 设置唤醒使能标志 */
                                }
                                else
                                {
                                    len = 0xFF;                                    /* 操作失败 */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                        /* 操作失败 */
                            }
                        }
                        else if((UsbSetupBuf->bRequestType & 0x1F) == 0x02)        /* 设置端点 */
                        {
                            if((((UINT16)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x00)
                            {
                                switch(((UINT16)UsbSetupBuf->wIndexH << 8) | UsbSetupBuf->wIndexL)
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                                     /* 操作失败 */
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                         /* 操作失败 */
                            }
                        }
                        else
                        {
                            len = 0xFF;                                             /* 操作失败 */
                        }
                        break;

                    case USB_GET_STATUS:
                        Ep0Buffer[0] = 0x00;
                        Ep0Buffer[1] = 0x00;
                        if(SetupLen >= 2)
                        {
                            len = 2;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                        break;

                    default:
                        len = 0xff;                                                  //操作失败
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                          //包长度错误
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= THIS_ENDP0_SIZE)                                         //上传数据或者状态阶段返回0长度包
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
            }
            else
            {
                UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
            }
            break;

        //端点0收到IN令牌包
        case UIS_TOKEN_IN | 0:                                                      //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
            case HID_GET_REPORT:
                len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;     //本次传输长度
                memcpy(Ep0Buffer, pDescr, len);                                     //加载上传数据
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                            //同步标志位翻转
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                                      //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
            break;

        //端点0收到OUT令牌包
        case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
            len = USB_RX_LEN;
            if(SetupReq == 0x09)
            {
                if(Ep0Buffer[0])
                {
//                    printf("Light on Num Lock LED!\n");
                }
                else if(Ep0Buffer[0] == 0)
                {
//                    printf("Light off Num Lock LED!\n");
                }
            }
            UEP0_CTRL ^= bUEP_R_TOG;                                     //同步标志位翻转
            break;

        //其他情况
        default:
            break;
        }

        UIF_TRANSFER = 0;                                                           //写0清空中断
    }

    //设备模式USB总线复位中断
    if(UIF_BUS_RST)
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        Endp2Busy = 0;
        UIF_BUS_RST = 0;                                                             //清中断标志
    }

    //USB总线挂起/唤醒完成
    if(UIF_SUSPEND)
    {
        //清除挂起中断
        UIF_SUSPEND = 0;
        if(USB_MIS_ST & bUMS_SUSPEND)
            // USB_MIS_ST：杂项状态寄存器
            // bUMS_SUSPEND：挂起状态位，0表示有USB活动；为1表示请求挂起
        {
        }
    }

    //意外的中断,不可能发生的情况
    else
    {
        USB_INT_FG = 0xFF;                                                             //清中断标志
    }
}

void HID_Button_Up()
{
    UserEp2Buf[0] = 0x00;	//0111 1111
    UserEp2Buf[1] = 0x00;
    Task_Query_Flag_Enable();
}

void HID_Button_Down()
{
    UserEp2Buf[0] = 0x00;	//0111 1111
    UserEp2Buf[1] = 0x80;
    Task_Query_Flag_Enable();
}

void HID_Right()
{
    UserEp2Buf[0] = 0x64;	//0111 1111
    UserEp2Buf[1] &= 0x80;
    Task_Query_Flag_Enable();
}

void HID_Left()
{
    UserEp2Buf[0] = 0x9C;	//0111 1111
    UserEp2Buf[1] |= 0x7F;
    Task_Query_Flag_Enable();
}

void USB_Report_Data()
{
    while(Endp2Busy);                                              //如果忙（上一包数据没有传上去），则等待。
    Endp2Busy = 1;                                                 //设置为忙状态
    Enp2BlukIn();
}
