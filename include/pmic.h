#ifndef __PMIC_H__
#define __PMIC_H__

#define GPD1CON				*(volatile unsigned long *)(0xE02000C0)
#define GPD1DAT				*(volatile unsigned long *)(0xE02000C4)
#define GPD1PUD				*(volatile unsigned long *)(0xE02000C8)

#define GPJ2CON				*(volatile unsigned long *)(0xE0200280)
#define GPJ2DAT				*(volatile unsigned long *)(0xE0200284)
#define GPJ2PUD				*(volatile unsigned long *)(0xE0200288)


#define IIC_ESCL_Hi			GPD1DAT |= (0x1<<1)
#define IIC_ESCL_Lo			GPD1DAT &= ~(0x1<<1)
#define IIC_ESDA_Hi			GPD1DAT |= (0x1<<0)
#define IIC_ESDA_Lo			GPD1DAT &= ~(0x1<<0)

#define IIC_ESCL_INP		GPD1CON &= ~(0xf<<4)
#define IIC_ESCL_OUTP		GPD1CON = (GPD1CON & ~(0xf<<4))|(0x1<<4)

#define IIC_ESDA_INP		GPD1CON &= ~(0xf<<0)
#define IIC_ESDA_OUTP		GPD1CON = (GPD1CON & ~(0xf<<0))|(0x1<<0)

#define DELAY				100

#define TPS659101_ADDR		0x2D
#define MAX8698_ADDR		0x66	// when SRAD pin = 0, CC/CDh is selected

#define IIC_SDASR_EN2_UP	{GPJ2CON = ((GPJ2CON & (~0xf<<12)) | (0x1<<12)); GPJ2DAT |= (0x1<<3);}	//GPJ2_3
#define IIC_SDASR_EN2_DOWN	{GPJ2CON = ((GPJ2CON & (~0xf<<12)) | (0x1<<12)); GPJ2DAT &= ~(0x1<<3);}	//GPJ2_3
#define IIC_SCLSR_EN1_UP	{GPJ2CON = ((GPJ2CON & (~0xf<<16)) | (0x1<<16)); GPJ2DAT |= (0x1<<4);}	//GPJ2_4
#define IIC_SCLSR_EN1_DOWN	{GPJ2CON = ((GPJ2CON & (~0xf<<16)) | (0x1<<16)); GPJ2DAT &= ~(0x1<<4);}	//GPJ2_4

extern void PMIC_InitIp(void);

#endif /*__PMIC_H__*/

