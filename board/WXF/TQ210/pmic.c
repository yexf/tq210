#include <config.h>
#include <pmic.h>

void Delay(void)
{
	unsigned long i,j;
	for(i=0;i<DELAY;i++);
}

void SCLH_SDAH()
{
	IIC_ESCL_Hi;
	IIC_ESDA_Hi;
	Delay();
}

void SCLH_SDAL()
{
	IIC_ESCL_Hi;
	IIC_ESDA_Lo;
	Delay();
}

void SCLL_SDAH()
{
	IIC_ESCL_Lo;
	IIC_ESDA_Hi;
	Delay();
}

void SCLL_SDAL()
{
	IIC_ESCL_Lo;
	IIC_ESDA_Lo;
	Delay();
}

void IIC_ELow()
{
	SCLL_SDAL();
	SCLH_SDAL();
	SCLH_SDAL();
	SCLL_SDAL();
}

void IIC_EHigh()
{
	SCLL_SDAH();
	SCLH_SDAH();
	SCLH_SDAH();
	SCLL_SDAH();
}

void IIC_EStart()
{
	SCLH_SDAH();
	SCLH_SDAL();
	Delay();
	SCLL_SDAL();
}

void IIC_EEnd()
{
	SCLL_SDAL();
	SCLH_SDAL();
	Delay();
	SCLH_SDAH();
}

void IIC_EAck()
{
	unsigned long ack;

	IIC_ESDA_INP;			// Function <- Input

	IIC_ESCL_Lo;
	Delay();
	IIC_ESCL_Hi;
	Delay();
	ack = GPD1DAT;
	IIC_ESCL_Hi;
	Delay();
	IIC_ESCL_Hi;
	Delay();

	IIC_ESDA_OUTP;			// Function <- Output (SDA)

	ack = ack & 0x1;
//	while(ack!=0);

	SCLL_SDAL();
}

void IIC_ESetport(void)
{
	GPD1PUD &= ~(0xf<<0);	// Pull Up/Down Disable	SCL, SDA

	IIC_ESCL_Hi;
	IIC_ESDA_Hi;

	IIC_ESCL_OUTP;		// Function <- Output (SCL)
	IIC_ESDA_OUTP;		// Function <- Output (SDA)

	Delay();
}

void IIC_EWrite (unsigned char ChipId, unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_ELow();	// write 'W'

	IIC_EAck();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EAck();	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EAck();	// ACK

	IIC_EEnd();
}

unsigned char IIC_ERead (unsigned char ChipId, unsigned char IicAddr)
{
	unsigned long i;
	unsigned char val=0x0;

	IIC_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EHigh();	// write 'R'

	IIC_EAck();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EAck();	// ACK

////////////////// read reg. data. //////////////////
	IIC_ESDA_INP;			// Function <- Input
	for(i = 8; i>0; i--)
	{
		IIC_ESCL_Lo;
		Delay();
		IIC_ESCL_Hi;
		Delay();
		val |= (unsigned char)(GPD1DAT<<(i));
		IIC_ESCL_Hi;
		Delay();
		IIC_ESCL_Lo;
		Delay();
	}
	IIC_ESDA_OUTP;			// Function <- Output (SDA)

	IIC_ELow();
//	IIC_EAck();	// ACK

	IIC_EEnd();
}

void PMIC_InitIp(void)
{
	IIC_ESetport();

#ifdef CONFIG_TQ210_IIC_PM_CHIP
	IIC_SDASR_EN2_UP;
	IIC_SCLSR_EN1_UP;
	IIC_EWrite(TPS659101_ADDR, 0x30, 0x09);		//VDD_MIPI_A to 1.8V
	IIC_EWrite(TPS659101_ADDR, 0x33, 0x0D);		//VDD_ADC to 3.3V
	IIC_EWrite(TPS659101_ADDR, 0x3F, 0x44);		//set DEVCTRL_REG to DEV_ON（位2）=1（设置为1之后，重启它都会上电的，除非关电）,CK32K_CTRL（位5）=1
	IIC_EWrite(TPS659101_ADDR, 0x40, 0x30);		//set DEVCTRL2_REG to TSLOT_LENGTH（位4、5）=0x3, PWRON_LP_OFF（位2）=0（如果为1，长按电源管理芯片就会断电；为0长按不起作用）,IT_POL（位0）=0（INT1脚为高，有中断时为低）
	IIC_EWrite(TPS659101_ADDR, 0x50, 0x00);		//设置 INT_STS_REG寄存器，清空PWRON_LP_IT位（位3）。
	//理论上在uboot中应该将所有中断都屏蔽才行，毕竟u-boot中不需要中断。
	IIC_EWrite(TPS659101_ADDR, 0x51, 0xFF);		//设置 INT_MSK_REG寄存器，禁止RTC的两个中断、热点中断、PWRHOLD中断和电池中断。
	IIC_EWrite(TPS659101_ADDR, 0x52, 0x00);		//设置 INT_STS2_REG寄存器，
	IIC_EWrite(TPS659101_ADDR, 0x53, 0x03);		//设置 INT_MSK2_REG寄存器，禁止GPIO的中断。
//	while(IIC_ERead(TPS659101_ADDR, 0x51) != 0x51);
#else
#if 0
	IIC_EWrite(MAX8698_ADDR, 0x00, 0x3E);
	IIC_EWrite(MAX8698_ADDR, 0x01, 0xF0);

	// VDD_ARM
#if defined(CONFIG_CLK_1200_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x04, 0xBB);	// default:0x99=>1.2v, 0xBB=>1.30v
	IIC_EWrite(MAX8698_ADDR, 0x05, 0xBB);	// default:0x99=>1.2v, 0xBB=>1.30v
#elif defined (CONFIG_CLK_1000_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x04, 0xAA);	// default:0x99=>1.2v, 0xAA=>1.25v
	IIC_EWrite(MAX8698_ADDR, 0x05, 0xAA);	// default:0x99=>1.2v, 0xAA=>1.25v
#else
	IIC_EWrite(MAX8698_ADDR, 0x04, 0x99);	// default:0x99=>1.2v, 0x99=>1.2v
	IIC_EWrite(MAX8698_ADDR, 0x05, 0x99);	// default:0x99=>1.2v, 0x99=>1.2v
#endif

	// VDD_INT
#if defined(CONFIG_CLK_1200_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x06, 0x88);	// default:0x99=>1.2v, 0x88=>1.15v
#else
	IIC_EWrite(MAX8698_ADDR, 0x06, 0x77);	// default:0x99=>1.2v, 0x77=>1.1v
#endif
	IIC_EWrite(MAX8698_ADDR, 0x07, 0x02);

	IIC_EWrite(MAX8698_ADDR, 0x08, 0x66);
	IIC_EWrite(MAX8698_ADDR, 0x09, 0x02);
	IIC_EWrite(MAX8698_ADDR, 0x0A, 0x0C);
	IIC_EWrite(MAX8698_ADDR, 0x0B, 0x0A);
	IIC_EWrite(MAX8698_ADDR, 0x0C, 0x0E);
	IIC_EWrite(MAX8698_ADDR, 0x0D, 0x33);
	IIC_EWrite(MAX8698_ADDR, 0x0E, 0x0E);
#endif
#endif /* CONFIG_TQ210_IIC_PM_CHIP */
}

