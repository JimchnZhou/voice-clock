
/********************************** (C) COPYRIGHT *******************************
* File Name          :CompatibilityHID.C
* Author             : WCH
* Version            : V1.2
* Date               : 2018/02/28
* Description        : CH554ģ��HID�����豸��֧���ж����´���֧�ֿ��ƶ˵����´���֧������ȫ�٣�����
*******************************************************************************/

#include "CH554.H"
#include "hid.h"
#include "delay.h"
#include "tasklist.h"
#include <string.h>
#include "GPIO.H"

#define THIS_ENDP0_SIZE         64		// �˵����ݰ���С

UINT8X  Ep0Buffer[8 > (THIS_ENDP0_SIZE + 2) ? 8 : (THIS_ENDP0_SIZE + 2)] _at_ 0x0000; //�˵�0 OUT&IN��������������ż��ַ
UINT8X  Ep2Buffer[64 > (MAX_PACKET_SIZE + 2) ? 64 : (MAX_PACKET_SIZE + 2)] _at_ 0x0050; //�˵�2 IN������,������ż��ַ

UINT8   SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;
PUINT8  pDescr;                                                                    //USB���ñ�־
USB_SETUP_REQ   SetupReqBuf;                                                       //�ݴ�Setup��
#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)

/*�豸������*/
UINT8C DevDesc[18] =
{
    0x12,							//bLength							���������ȣ�18�ֽ�0x12��
    0x01,							//bDescriptorType			���������ͣ��豸������0x01��
    0x10, 0x01, 			//bcdUSB							USB HIDЭ��汾��1.10��
    0x00,							//bDeviceClass				����룬HID�����ڽӿ��������ж����
    0x00,							//bDeviceSubClass			������룬��Щ������ bDeviceClass �ֶε�ֵ�޶�
    0x00,							//bDeviceProtocol			Э����룬��Щ������ bDeviceSubClass �ֶε�ֵ�޶�
    THIS_ENDP0_SIZE,	//bMaxPacketSize0			�˵�0��������ݰ���С��8��16��32 �� 64��
    0x3d, 0x41, 			//idVendor						����ID��0xFFFF��
    0x07, 0x21, 			//idProduct						��ƷID��0x0001��
    0x00, 0x00, 			//bcdDevice						�豸�汾�ţ�0x0100��
    0x00,							//iManufacturer				�������̵��ַ���������
    0x00,							//iProduct						������Ʒ���ַ���������
    0x00,							//iSerialNumber				��Ʒ���к��ַ���������
    0x01							//bNumConfigurations	���ܵ�������

};

/*����������*/
UINT8C CfgDesc[41] =
{
    // ����������
    0x09,				//bLength							���������ȣ�9�ֽ�0x09��
    0x02, 			//bDescriptorType			���������ͣ�����������0x02��
    0x29, 0x00,	//wTotalLength				���������������ܳ���(34�ֽ�0x0022 41�ֽ�0x0029)
    0x01,				//bNumInterfaces			����֧�ֵĽӿ���
    0x01,				//bConfigurationValue	���õ�ֵ
    0x00,				//iConfiguration			���������õ��ַ���������ֵ����0x00
    0xA0,				//bmAttributes				���豸�����ԣ�7���߹��� 6�Թ��� 5Զ�̻��� 4..0������10100000 B
    0x32,				//MaxPower						�豸����ĵ�������λ2mA��100mA = 0x32��

    // �ӿ�������
    0x09,				//bLength							�������ĳ��ȣ�9�ֽ�0x09��
    0x04,				//bDescriptorType			���������ͣ��ӿ�������Ϊ0x04��
    0x00,				//bInterfaceNumber		�ýӿڵı�ţ���0��ʼ��
    0x00,				//bAlternateSetting		�ýӿڵı��ñ��
    0x01,				//bNumEndpoints				�˽ӿ�ʹ�õĶ˵������������˵��㣬�����ֵΪ�㣬��˽ӿڽ�ʹ�ö˵��㡣
    0x03,				//bInterfaceClass			�ýӿ�ʹ�õ���
    0x01,				//bInterfaceSubClass	�ýӿ�ʹ�õ����࣬0û�����࣬1�����ӿ�����
    0x00,				//bInterfaceProtocol	�ýӿ�ʹ�õ�Э�飬0�ޣ�1���̣�2���
    0x00,				//iInterface					�����ýӿڵ��ַ���������ֵ

    // HID��������
    0x09, 			//bLength							�������ĳ��ȣ��������¼����������پ�����
    0x21, 			//bDescriptorType			���������ͣ�HID������Ϊ0x21��
    0x11, 0x01, //bcdHID							HIDЭ��汾
    0x00, 			//bCountryCode				���Ҵ���
    0x01, 			//bNumDescriptors			Ҫ��ѭ��HID���������������� 0x01
    0x22, 			//bDescriptorType			��������������
    0x36, 0x00,	//wDescriptorLength		�������������ܳ���52=0x34,56=0x38,68=0x44

    // �˵�������
    0x07,				//bLength							�������ĳ��ȣ�7�ֽڣ�
    0x05,				//bDescriptorType			���������ͣ��˵�������Ϊ0x05��
    0x82,				//bEndpointAddress		�ö˵�ĵ�ַ 10000010B
    0x03,				//bmAttributes				�ö˵�����ԣ�λ 0..1 �������ͣ���������λ��������00000011B
    0x08, 0x00,	//wMaxPacketSize			�ö˵�֧�ֵ�������
    0x0a,				//bInterval						�˵�Ĳ�ѯʱ��

    // �˵�������
    0x07,				//bLength							�������ĳ��ȣ�7�ֽڣ�
    0x05,				//bDescriptorType			���������ͣ��˵�������Ϊ0x05��
    0x82,				//bEndpointAddress		�ö˵�ĵ�ַ 10000010B
    0x03,				//bmAttributes				�ö˵�����ԣ�λ 0..1 �������ͣ���������λ��������00000011B
    0x08, 0x00,	//wMaxPacketSize			�ö˵�֧�ֵ�������
    0x0a,				//bInterval						�˵�Ĳ�ѯʱ��
};

/*�ַ��������� ��*/

/*HID�౨��������*/
UINT8C HIDRepDesc[54] =
{
    0x05, 0x01, 			// ʹ��ҳ�棨ͨ�����棩
    0x09, 0x0e, 			// �÷���ϵͳ�����������
    0xa1, 0x01, 			// ���ϣ�Ӧ�ó���
    0x05, 0x0d, 			//  ʹ��ҳ�棨���ֻ��ǣ�
    0x09, 0x21, 			//  �÷���Puck��
    0xa1, 0x00, 			//  ���ϣ�����
    0x05, 0x01, 			//   ʹ��ҳ�棨ͨ�����棩
    0x09, 0x37, 			//   �÷������ţ�
    0x95, 0x01, 			//   ���������� (1)
    0x75, 0x0f, 			//   �����򳤶ȣ�15��
    0x55, 0x0f, 			//   ��λָ�� (-1)
    0x65, 0x14, 			//   ��λ��������Ӣ����ת��
    0x36, 0xf0, 0xf1, //   ������Сֵ (-3600)
    0x46, 0x10, 0x0e, //   �������ֵ (3600)
    0x16, 0xf0, 0xf1, //   �߼���Сֵ (-3600)
    0x26, 0x10, 0x0e, //   �߼����ֵ (3600)
    0x81, 0x06, 			//   ���루���ݣ���������ԣ�
    0x05, 0x09, 			//   ʹ��ҳ�棨��ť��
    0x09, 0x01, 			//   �÷�����ť 1��
    0x95, 0x01, 			//   ���������� (1)
    0x75, 0x01, 			//   �����򳤶ȣ�1��
    0x15, 0x00, 			//   �߼���Сֵ��0��
    0x25, 0x01, 			//   �߼����ֵ (1)
    0x81, 0x02, 			//   ���루���ݣ�����������ֵ��
    0xc0, 						//  ��������
    0xc0, 						// ��������
};

// unsigned char  code LangDes[]={0x04,0x03,0x09,0x04};           //����������
// unsigned char  code SerDes[]={
//                           0x28,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//                           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//                           0x00,0x00,0x00,0x00,0x00,0x49,0x00,0x43,0x00,0x42,
//                           0x00,0x43,0x00,0x31,0x00,0x00,0x00,0x00,0x00,0x00
//                           };                                   //�ַ���������

UINT8X UserEp2Buf[2] = {0x00, 0x00};                                            //�û����ݶ���

UINT8 Endp2Busy = 0;

/*******************************************************************************
* Function Name  : USB_Device_Init()
* Description    : USB�豸ģʽ����,�豸ģʽ�������շ��˵����ã��жϿ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_Device_Init(void)
{
    IE_USB = 0;																																 // �ر�USB�ж�
    USB_CTRL = 0x00;                                                           // USB���ƼĴ�����ʼ�����豸ģʽ��ȫ��12Mbps
    UDEV_CTRL = bUD_PD_DIS;                                                    // ��ֹDP/DM��������

    UDEV_CTRL &= ~bUD_LOW_SPEED;                                               //ѡ��ȫ��12Mģʽ��Ĭ�Ϸ�ʽ
    USB_CTRL &= ~bUC_LOW_SPEED;

    UEP2_DMA = Ep2Buffer;                                                      //�˵�2���ݴ����ַ
    UEP2_3_MOD |= bUEP2_TX_EN | bUEP2_RX_EN;                                   //�˵�2���ͽ���ʹ��
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;                                              //�˵�2�շ���64�ֽڻ�����
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //�˵�2�Զ���תͬ����־λ��IN���񷵻�NAK��OUT����ACK
    UEP0_DMA = Ep0Buffer;                                                      //�˵�0���ݴ����ַ
    UEP4_1_MOD &= ~(bUEP4_RX_EN | bUEP4_TX_EN);                                //�˵�0��64�ֽ��շ�������
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT���񷵻�ACK��IN���񷵻�NAK

    USB_DEV_AD = 0x00;
    USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                     // ����USB�豸��DMA�����ж��ڼ��жϱ�־δ���ǰ�Զ�����NAK
    UDEV_CTRL |= bUD_PORT_EN;                                                  // ����USB�˿�
    USB_INT_FG = 0xFF;                                                         // ���жϱ�־
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    IE_USB = 1;
}

/*******************************************************************************
* Function Name  : Enp2BlukIn()
* Description    : USB�豸ģʽ�˵�2�������ϴ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2BlukIn()
{
    memcpy(Ep2Buffer + MAX_PACKET_SIZE, UserEp2Buf, sizeof(UserEp2Buf));       //�����ϴ�����
    UEP2_T_LEN = sizeof(UserEp2Buf);                                              //�ϴ���������
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                  //������ʱ�ϴ����ݲ�Ӧ��ACK
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB�жϴ�����
*******************************************************************************/
void DeviceInterrupt(void) interrupt INT_NO_USB using 1                      //USB�жϷ������,ʹ�üĴ�����1
{
    UINT8 len, i;

    // UIF_TRANSFER��USB������ɱ�־
    if(UIF_TRANSFER)                                                		//�����USB�������
    {
        // �ж��������ƺͶ˵��
        switch(USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
            // USB_INT_ST��			USB�ж�״̬�Ĵ���
            // MASK_UIS_TOKEN��	ǰ��������PID��ʶ��bUIS_TOKEN1 �� bUIS_TOKEN0 ��ɣ����ڱ�ʶ��ǰUSB�������������PID��00��ʾOUT����01��ʾSOF����10��ʾIN����11��ʾSETUP ����
            // MASK_UIS_ENDP��	��������Ķ˵�ţ�0000��ʾ�˵�0������1111��ʾ�˵�15
        {

        // �˵�2�յ�IN���ư�
        case UIS_TOKEN_IN | 2:                                         		// endpoint 2# �˵������ϴ�
            UEP2_T_LEN = 0;                                            		// Ԥʹ�÷��ͳ���һ��Ҫ���
//            UEP1_CTRL ^= bUEP_T_TOG;                                 		// ����������Զ���ת����Ҫ�ֶ���ת
            Endp2Busy = 0 ;
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;  		// Ĭ��Ӧ��NAK
            break;

        // �˵�2�յ�OUT���ư�
        case UIS_TOKEN_OUT | 2:                                        		// endpoint 2# �˵������´�
            if(U_TOG_OK)                                                	// ��ͬ�������ݰ�������
            {
                len = USB_RX_LEN;                                         //�������ݳ��ȣ����ݴ�Ep2Buffer�׵�ַ��ʼ���
                for(i = 0; i < len; i ++)
                {
                    Ep2Buffer[MAX_PACKET_SIZE + i] = Ep2Buffer[i] ^ 0xFF; // OUT����ȡ����IN�ɼ������֤
                }
                UEP2_T_LEN = len;
                UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; // �����ϴ�
            }
            break;

        //�˵�0�յ�SETUP���ư�
        case UIS_TOKEN_SETUP | 0:
            len = USB_RX_LEN;                      //USB_RX_LEN��USB���ճ��ȼĴ���

            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                len = 0;                                                         // Ĭ��Ϊ�ɹ������ϴ�0����
                SetupReq = UsbSetupBuf->bRequest;

                if((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD)     /*HID������*/
                {
                    switch(SetupReq)
                    {
                    case 0x01:                                                  //GetReport
                        pDescr = UserEp2Buf;                                    //���ƶ˵��ϴ����
                        if(SetupLen >= THIS_ENDP0_SIZE)                         //���ڶ˵�0��С����Ҫ���⴦��
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
                        len = 0xFF;  				                              /*���֧��*/
                        break;
                    }
                    if(SetupLen > len)
                    {
                        SetupLen = len;    //�����ܳ���
                    }
                    len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;   //���δ��䳤��
                    memcpy(Ep0Buffer, pDescr, len);                                   //�����ϴ�����
                    SetupLen -= len;
                    pDescr += len;
                }
                else                                                             //��׼����
                {
                    switch(SetupReq)                                             //������
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                                  //�豸������
                            pDescr = DevDesc;                                    //���豸�������͵�Ҫ���͵Ļ�����
                            len = sizeof(DevDesc);
                            break;

                        case 2:                                                  //����������
                            pDescr = CfgDesc;                                    //���豸�������͵�Ҫ���͵Ļ�����
                            len = sizeof(CfgDesc);
                            break;

                        case 0x22:                                               //����������
                            pDescr = HIDRepDesc;                                 //����׼���ϴ�
                            len = sizeof(HIDRepDesc);
                            break;

                        default:
                            len = 0xff;                                          //��֧�ֵ�������߳���
                            break;
                        }
                        if(SetupLen > len)
                        {
                            SetupLen = len;    //�����ܳ���
                        }
                        len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;//���δ��䳤��
                        memcpy(Ep0Buffer, pDescr, len);                          //�����ϴ�����
                        SetupLen -= len;
                        pDescr += len;
                        break;

                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                         //�ݴ�USB�豸��ַ
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
                            Ready = 1;                                            //set config����һ�����usbö����ɵı�־
                        }
                        break;

                    case 0x0A:
                        break;

                    case USB_CLEAR_FEATURE:                                      //Clear Feature
                        if((UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)     // �˵�
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
                                len = 0xFF;                                       // ��֧�ֵĶ˵�
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                           // ���Ƕ˵㲻֧��
                        }
                        break;

                    case USB_SET_FEATURE:                                         /* Set Feature */
                        if((UsbSetupBuf->bRequestType & 0x1F) == 0x00)            /* �����豸 */
                        {
                            if((((UINT16)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x01)
                            {
                                if(CfgDesc[ 7 ] & 0x20)
                                {
                                    /* ���û���ʹ�ܱ�־ */
                                }
                                else
                                {
                                    len = 0xFF;                                    /* ����ʧ�� */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                        /* ����ʧ�� */
                            }
                        }
                        else if((UsbSetupBuf->bRequestType & 0x1F) == 0x02)        /* ���ö˵� */
                        {
                            if((((UINT16)UsbSetupBuf->wValueH << 8) | UsbSetupBuf->wValueL) == 0x00)
                            {
                                switch(((UINT16)UsbSetupBuf->wIndexH << 8) | UsbSetupBuf->wIndexL)
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* ���ö˵�2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* ���ö˵�2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* ���ö˵�1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                                     /* ����ʧ�� */
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                         /* ����ʧ�� */
                            }
                        }
                        else
                        {
                            len = 0xFF;                                             /* ����ʧ�� */
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
                        len = 0xff;                                                  //����ʧ��
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                          //�����ȴ���
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= THIS_ENDP0_SIZE)                                         //�ϴ����ݻ���״̬�׶η���0���Ȱ�
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//Ĭ�����ݰ���DATA1������Ӧ��ACK
            }
            else
            {
                UEP0_T_LEN = 0;  //��Ȼ��δ��״̬�׶Σ�������ǰԤ���ϴ�0�������ݰ��Է�������ǰ����״̬�׶�
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//Ĭ�����ݰ���DATA1,����Ӧ��ACK
            }
            break;

        //�˵�0�յ�IN���ư�
        case UIS_TOKEN_IN | 0:                                                      //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
            case HID_GET_REPORT:
                len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;     //���δ��䳤��
                memcpy(Ep0Buffer, pDescr, len);                                     //�����ϴ�����
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                            //ͬ����־λ��ת
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                                      //״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ���
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
            break;

        //�˵�0�յ�OUT���ư�
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
            UEP0_CTRL ^= bUEP_R_TOG;                                     //ͬ����־λ��ת
            break;

        //�������
        default:
            break;
        }

        UIF_TRANSFER = 0;                                                           //д0����ж�
    }

    //�豸ģʽUSB���߸�λ�ж�
    if(UIF_BUS_RST)
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        Endp2Busy = 0;
        UIF_BUS_RST = 0;                                                             //���жϱ�־
    }

    //USB���߹���/�������
    if(UIF_SUSPEND)
    {
        //��������ж�
        UIF_SUSPEND = 0;
        if(USB_MIS_ST & bUMS_SUSPEND)
            // USB_MIS_ST������״̬�Ĵ���
            // bUMS_SUSPEND������״̬λ��0��ʾ��USB���Ϊ1��ʾ�������
        {
        }
    }

    //������ж�,�����ܷ��������
    else
    {
        USB_INT_FG = 0xFF;                                                             //���жϱ�־
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
    while(Endp2Busy);                                              //���æ����һ������û�д���ȥ������ȴ���
    Endp2Busy = 1;                                                 //����Ϊæ״̬
    Enp2BlukIn();
}
