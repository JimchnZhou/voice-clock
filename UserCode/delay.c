#include "CH554.H"
#include "delay.h"

/*******************************************************************************
* Function Name  : Delay_us(UNIT16 n)
* Description    : us��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	Delay_us(UINT16 n)    // ��uSΪ��λ��ʱ
{
    while(n)       // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
    {
        ++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
        -- n;
    }
}

/*******************************************************************************
* Function Name  : Delay_ms(UNIT16 n)
* Description    : ms��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	Delay_ms(UINT16 n)                                                    // ��mSΪ��λ��ʱ
{
    while(n)
    {
        Delay_us(1000);
        -- n;
    }
}


