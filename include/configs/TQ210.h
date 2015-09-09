/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <gj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the EmbedSky TQ210(mDirac-III) board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H


#define CONFIG_TQ_ERROR						1

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_S5PV210						1		/* in a SAMSUNG S5PV210 SoC */
#define CONFIG_S5PC11X						1		/* in a SAMSUNG S5PV210 Family  */
#define CONFIG_TQ210						1		/* EmbedSky TQ210 Board */
#define CONFIG_CMD_MENU						1
#define CONFIG_EMBEDSKY_FAT					1
#define CONFIG_OEM_SDREAD					1
//#define CONFIG_EMBEDSKY_INIT_CUP			1
#if 1
#define CONFIG_EmbedSky_LCD					1
#else
#undef CONFIG_EmbedSky_LCD
#endif

#if 1												//是否使用1GB内存
#define CONFIG_TQ210_1G
#else
#undef CONFIG_TQ210_1G
#endif

#if 1								//是否支持WinCE的烧写
#define CONFIG_SUPPORT_WINCE
#else
#undef CONFIG_SUPPORT_WINCE
#endif

#ifdef CONFIG_SUPPORT_WINCE
#define CONFIG_RELOCATE_NK			//解析NK.bin镜像
#define CONFIG_WINCE_NK				//烧写解析后的NK.bin镜像
#else
#undef CONFIG_RELOCATE_NK
#undef CONFIG_WINCE_NK
#endif /* CONFIG_SUPPORT_WINCE */

#ifdef CONFIG_EmbedSky_LCD
#define CONFIG_EMBEDSKY_INPUT				1
#define CONFIG_LCD_TYPE						X800Y480
#else
#undef CONFIG_EMBEDSKY_INPUT
#undef CONFIG_LCD_TYPE
#endif /* CONFIG_EmbedSky_LCD */

#if 1
#define CONFIG_DIS_BOARD_INFO				1
#else
#undef CONFIG_DIS_BOARD_INFO
#endif

#if 0
#define CONFIG_TQ210_IIC_PM_CHIP			1    	//configs using iic power mangement chip
#else
#undef CONFIG_TQ210_IIC_PM_CHIP
#endif

#define CONFIG_MCP_SINGLE					1
#define CONFIG_EVT1							1		/* EVT1 */

#define CONFIG_FASTBOOT						1
//#define CONFIG_FUSED						1		/* Fused chip */
//#define CONFIG_SECURE_BOOT				1		/* secure booting */
#define BOOT_ONENAND						0x1
#define BOOT_NAND							0x2
#define BOOT_MMCSD							0x3
#define BOOT_NOR							0x4
#define BOOT_SEC_DEV						0x5

/* skip to load BL2 */
//#define FAST_BOOT							1

#define MEMORY_BASE_ADDRESS					0x20000000

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ					24000000	/* the TQ210 has 24MHz input clock */

//#define CONFIG_ENABLE_MMU
#undef CONFIG_ENABLE_MMU
#define CONFIG_MINI_BOOT

#ifdef CONFIG_ENABLE_MMU
#define virt_to_phys(x)						virt_to_phy_tq210(x)
#else
#define virt_to_phys(x)						(x)
#endif

#define CONFIG_MEMORY_UPPER_CODE

#undef CONFIG_USE_IRQ				/* we don't need IRQ/FIQ stuff */

#define CONFIG_INCLUDE_TEST

#define CONFIG_ZIMAGE_BOOT
#define CONFIG_IMAGE_BOOT

#define BOARD_LATE_INIT

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

/*
 * Architecture magic and machine type
 */

#define MACH_TYPE							2456

#define UBOOT_MAGIC							(0x43090000 | MACH_TYPE)

/* Power Management is enabled */
#define CONFIG_PM

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#undef CONFIG_SKIP_RELOCATE_UBOOT
#undef CONFIG_USE_NOR_BOOT

/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN						(CFG_ENV_SIZE + 1024*1024)
#define CONFIG_SYS_MALLOC_LEN				(CFG_MALLOC_LEN)
#define CFG_GBL_DATA_SIZE					128	/* size in bytes reserved for initial data */

#define CFG_STACK_SIZE						512*1024/*512*1024*/

/*
 * Hardware drivers
 */
#define DM9000_16BIT_DATA

#define CONFIG_DRIVER_DM9000				1

#ifdef CONFIG_DRIVER_DM9000
#define CONFIG_DM9000_BASE					(0x88000000)
//#define CONFIG_DM9000_DEBUG		
#define DM9000_IO							(CONFIG_DM9000_BASE)
#if defined(DM9000_16BIT_DATA)
#define DM9000_DATA							(CONFIG_DM9000_BASE+4)
#else
#define DM9000_DATA							(CONFIG_DM9000_BASE+1)
#endif
#endif
/*
 * select serial console configuration
 */
//串口号在这里是从1开始计算的到4结束。
#define CONFIG_SERIAL1      			    1	/* we use UART1 on TQ210 */
//#define CONFIG_SERIAL2      			    2	/* we use UART2 on TQ210 */
//#define CONFIG_SERIAL3      			    3	/* we use UART3 on TQ210 */
//#define CONFIG_SERIAL4      			    4	/* we use UART4 on TQ210 */

#if defined(CONFIG_SERIAL1)
#define SERIAL_PORT_NUM						0
#elif defined(CONFIG_SERIAL2)
#define SERIAL_PORT_NUM						1
#elif defined(CONFIG_SERIAL3)
#define SERIAL_PORT_NUM						2
#elif defined(CONFIG_SERIAL4)
#define SERIAL_PORT_NUM						3
#endif

#define CFG_HUSH_PARSER			/* use "hush" command parser	*/
#ifdef CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2					"> "
#endif

#define CONFIG_CMDLINE_EDITING

#undef CONFIG_S3C64XX_I2C		/* this board has H/W I2C */
#ifdef CONFIG_S3C64XX_I2C
#define CONFIG_HARD_I2C						1
#define CFG_I2C_SPEED						50000
#define CFG_I2C_SLAVE						0xFE
#endif

#define CONFIG_DOS_PARTITION
#define CONFIG_SUPPORT_VFAT

#define CONFIG_USB_OHCI
#undef CONFIG_USB_STORAGE
#define CONFIG_S3C_USBD

#define USBD_DOWN_ADDR						0xc0000000

/************************************************************
 * RTC
 ************************************************************/
/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE						115200

/***********************************************************
 * Command definition
 ***********************************************************/
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_USB
#define CONFIG_CMD_REGINFO

#define	CONFIG_CMD_NAND
#define	CONFIG_CMD_FLASH

#ifndef FPGA_TQ210
//#define CONFIG_CMD_ONENAND
#define CONFIG_CMD_MOVINAND
#endif
#define CONFIG_CMD_PING
#define CONFIG_CMD_DATE

#include <config_cmd_default.h>

#define CONFIG_CMD_ELF
#define CONFIG_CMD_DHCP
//#define CONFIG_CMD_I2C

#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT

//#define CONFIG_CMD_JFFS2
//#define CONFIG_JFFS2_CMDLINE

#define CONFIG_CMD_UBI
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_LZO

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH

#define CONFIG_ETHADDR						00:40:5c:26:0a:5b
#define CONFIG_NETMASK      			    255.255.255.0
#define CONFIG_IPADDR						192.168.1.6
#define CONFIG_SERVERIP						192.168.1.8
#define CONFIG_GATEWAYIP					192.168.1.2

#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_NET_MULTI
#undef	CONFIG_NET_MULTI

#ifdef CONFIG_NET_MULTI
#define CONFIG_DRIVER_SMC911X_BASE			0x98800300
#define CONFIG_DRIVER_SMC911X_16_BIT
#define CONFIG_DRIVER_CS8900
#define CS8900_BASE	  						0x18800300
#define CS8900_BUS16
#else
#define CONFIG_DRIVER_SMC911X_16_BIT
#define CONFIG_DRIVER_SMC911X_BASE			0x98800300
#undef	CONFIG_DRIVER_CS8900
#endif

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE				115200		/* speed to run kgdb serial port */
/* what's this ? it's not used anywhere */
#define CONFIG_KGDB_SER_INDEX				1			/* which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				/* undef to save memory		*/

#define CFG_PROMPT							"TQ210 # "   /* Monitor Command Prompt       */

#define CFG_CBSIZE							256		/* Console I/O Buffer Size	*/
#define CFG_PBSIZE							384		/* Print Buffer Size */
#define CFG_MAXARGS							16		/* max number of command args	*/
#define CFG_BARGSIZE						CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START					MEMORY_BASE_ADDRESS	/* memtest works on	*/

#define CFG_MEMTEST_END						MEMORY_BASE_ADDRESS + 0x3E00000		/* 256 MB in DRAM	*/

#undef CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define CFG_LOAD_ADDR						MEMORY_BASE_ADDRESS	/* default load address	*/

/* the PWM TImer 4 uses a counter of 41687 for 10 ms, so we need */
/* it to wrap 100 times (total 4168750) to get 1 sec. */
#define CFG_HZ								4168750		// at PCLK 66MHz

/* valid baudrates */
#define CFG_BAUDRATE_TABLE					{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE					0x40000		/* regular stack 256KB */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ				(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ				(4*1024)	/* FIQ stack */
#endif

//#define CONFIG_CLK_667_166_166_133
//#define CONFIG_CLK_533_133_100_100
//#define CONFIG_CLK_800_100_166_133
//#define CONFIG_CLK_800_200_166_133
#define CONFIG_CLK_1000_200_166_133
//#define CONFIG_CLK_400_200_166_133
//#define CONFIG_CLK_400_100_166_133

//#define DEBUG_MTD

#if defined(CONFIG_CLK_667_166_166_133)
#define APLL_MDIV							0xfa
#define APLL_PDIV							0x6
#define APLL_SDIV							0x1
#elif defined(CONFIG_CLK_533_133_100_100)
#define APLL_MDIV							0x215
#define APLL_PDIV							0x18
#define APLL_SDIV							0x1
#elif defined(CONFIG_CLK_800_200_166_133) || \
	defined(CONFIG_CLK_800_100_166_133) || \
	defined(CONFIG_CLK_400_200_166_133) || \
	defined(CONFIG_CLK_400_100_166_133)
#define APLL_MDIV							0x64
#define APLL_PDIV						    0x3
#define APLL_SDIV							0x1
#elif defined(CONFIG_CLK_1000_200_166_133)
#define APLL_MDIV							0x7d
#define APLL_PDIV							0x3
#define APLL_SDIV							0x1
#endif

#define APLL_LOCKTIME_VAL					0x2cf

#if defined(CONFIG_EVT1)
/* Set AFC value */
#define AFC_ON								0x00000000
#define AFC_OFF								0x10000010
#endif

#if defined(CONFIG_CLK_533_133_100_100)
#define MPLL_MDIV							0x190
#define MPLL_PDIV							0x6
#define MPLL_SDIV							0x2
#else
#define MPLL_MDIV							0x29b
#define MPLL_PDIV							0xc
#define MPLL_SDIV							0x1
#endif

#define EPLL_MDIV							0x60
#define EPLL_PDIV							0x6
#define EPLL_SDIV							0x2

#define VPLL_MDIV							0x6c
#define VPLL_PDIV							0x6
#define VPLL_SDIV							0x3

/* CLK_DIV0 */
#define APLL_RATIO							0
#define A2M_RATIO							4
#define HCLK_MSYS_RATIO						8
#define PCLK_MSYS_RATIO						12
#define HCLK_DSYS_RATIO						16
#define PCLK_DSYS_RATIO						20
#define HCLK_PSYS_RATIO						24
#define PCLK_PSYS_RATIO						28

#define CLK_DIV0_MASK						0x7fffffff

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#define APLL_VAL							set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL							set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
#define EPLL_VAL							set_pll(EPLL_MDIV,EPLL_PDIV,EPLL_SDIV)
#define VPLL_VAL							set_pll(VPLL_MDIV,VPLL_PDIV,VPLL_SDIV)

#if defined(CONFIG_CLK_667_166_166_133)
#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(CONFIG_CLK_533_133_100_100)
#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(3<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(CONFIG_CLK_800_200_166_133)
#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(CONFIG_CLK_800_100_166_133)
#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(7<<A2M_RATIO)|(7<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(CONFIG_CLK_400_200_166_133)
#define CLK_DIV0_VAL						((1<<APLL_RATIO)|(3<<A2M_RATIO)|(1<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(CONFIG_CLK_400_100_166_133)
#define CLK_DIV0_VAL						((1<<APLL_RATIO)|(7<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))			
#elif defined(CONFIG_CLK_1000_200_166_133)
#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(4<<A2M_RATIO)|(4<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#endif

#define CLK_DIV1_VAL						((1<<16)|(1<<12)|(1<<8)|(1<<4))
#define CLK_DIV2_VAL						(1<<0)

#if defined(CONFIG_CLK_533_133_100_100)

#if defined(CONFIG_MCP_SINGLE)

#define DMC0_TIMINGA_REF					0x40e
#define DMC0_TIMING_ROW						0x10233206
#define DMC0_TIMING_DATA					0x12130005
#define	DMC0_TIMING_PWR						0x0E100222

#define DMC1_TIMINGA_REF					0x40e
#define DMC1_TIMING_ROW						0x10233206
#define DMC1_TIMING_DATA					0x12130005
#define	DMC1_TIMING_PWR						0x0E100222

#else

#error "You should define memory type (AC type or H type or B type)"

#endif

#elif defined(CONFIG_CLK_800_200_166_133) || \
	defined(CONFIG_CLK_1000_200_166_133) || \
	defined(CONFIG_CLK_800_100_166_133) || \
	defined(CONFIG_CLK_400_200_166_133) || \
	defined(CONFIG_CLK_400_100_166_133)

#if defined(CONFIG_MCP_SINGLE)
/*
#define DMC0_MEMCONFIG_0					0x20E01323	// MemConfig0	256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC0_MEMCONFIG_1					0x40F01323	// MemConfig1
#define DMC0_TIMINGA_REF					0x00000618	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC0_TIMING_ROW						0x28233287	// TimingRow	for @200MHz
#define DMC0_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC0_TIMING_PWR						0x09C80232	// TimingPower

#define	DMC1_MEMCONTROL						0x00202400	// MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
#define DMC1_MEMCONFIG_0					0x40C01323	// MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC1_MEMCONFIG_1					0x00E01323	// MemConfig1
#define DMC1_TIMINGA_REF					0x00000618	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4
#define DMC1_TIMING_ROW						0x28233289	// TimingRow	for @200MHz
#define DMC1_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC1_TIMING_PWR						0x08280232	// TimingPower*///lhh_debug

#define DMC0_MEMCONFIG_0					0x20E01323	// MemConfig0	256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC0_MEMCONFIG_1					0x40F01323	// MemConfig1
#define DMC0_TIMINGA_REF					0x00000618	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC0_TIMING_ROW						0x28233287	// TimingRow	for @200MHz
#define DMC0_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC0_TIMING_PWR						0x09C80232	// TimingPower

#define	DMC1_MEMCONTROL						0x00202400	//0x00202400	//0x00212400(16)// MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
#define DMC1_MEMCONFIG_0					0x40C01323	// MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC1_MEMCONFIG_1					0x00E01323	// MemConfig1
#define DMC1_TIMINGA_REF					0x00000618	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4
#define DMC1_TIMING_ROW						0x28233287	// TimingRow	for @200MHz
#define DMC1_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC1_TIMING_PWR						0x09c80232	// TimingPower

#if defined(CONFIG_CLK_800_100_166_133) || defined(CONFIG_CLK_400_100_166_133)
#define DMC0_MEMCONFIG_0					0x20E01323	// MemConfig0	256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC0_MEMCONFIG_1					0x40F01323	// MemConfig1
#define DMC0_TIMINGA_REF					0x0000030C	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC0_TIMING_ROW						0x28233287	// TimingRow	for @200MHz
#define DMC0_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC0_TIMING_PWR						0x09C80232	// TimingPower

#define	DMC1_MEMCONTROL						0x00202400	// MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
#define DMC1_MEMCONFIG_0					0x40C01323	// MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC1_MEMCONFIG_1					0x00E01323	// MemConfig1
#define DMC1_TIMINGA_REF					0x0000030C	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4
#define DMC1_TIMING_ROW						0x28233289	// TimingRow	for @200MHz
#define DMC1_TIMING_DATA					0x23240304	// TimingData	CL=3
#define	DMC1_TIMING_PWR						0x08280232	// TimingPower
#endif

#else

#error "You should define memory type (AC type or H type)"

#endif //

#else

#define DMC0_TIMINGA_REF					0x50e
#define DMC0_TIMING_ROW						0x14233287
#define DMC0_TIMING_DATA					0x12130005
#define	DMC0_TIMING_PWR						0x0E140222

#define DMC1_TIMINGA_REF					0x618
#define DMC1_TIMING_ROW						0x11344309
#define DMC1_TIMING_DATA					0x12130005
#define	DMC1_TIMING_PWR						0x0E190222

#endif


#if defined(CONFIG_CLK_533_133_100_100)
#define UART_UBRDIV_VAL						26
#define UART_UDIVSLOT_VAL					0x0808
#else
#define UART_UBRDIV_VAL						34
#define UART_UDIVSLOT_VAL					0xDDDD
#endif

#define CONFIG_NR_DRAM_BANKS				2						/* we have 2 bank of DRAM */
#define SDRAM_BANK_SIZE						0x20000000				/* 512 MB */
#define PHYS_SDRAM_1						MEMORY_BASE_ADDRESS		/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE					SDRAM_BANK_SIZE
#define PHYS_SDRAM_2						(MEMORY_BASE_ADDRESS + SDRAM_BANK_SIZE) /* SDRAM Bank #2 */
#define PHYS_SDRAM_2_SIZE					SDRAM_BANK_SIZE

#define CFG_FLASH_BASE						0x80000000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_MX_LV640EB		/* MX29LV640EB */
//#define CONFIG_AMD_LV800		/* AM29LV800BB */

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */

#if	defined(CONFIG_MX_LV640EB)
#define CFG_MAX_FLASH_SECT					135
#define PHYS_FLASH_SIZE						0x800000	/* 8MB */
#elif	defined(CONFIG_AMD_LV800)
#define CFG_MAX_FLASH_SECT					19
#define PHYS_FLASH_SIZE						0x100000	/* 1MB */
#else
#define CFG_MAX_FLASH_SECT					512
#define PHYS_FLASH_SIZE						0x100000	/* 1MB */
#endif

#define CFG_FLASH_LEGACY_512Kx16
#define CONFIG_FLASH_CFI_LEGACY
#define CFG_FLASH_CFI

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT				(5*CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT				(5*CFG_HZ) /* Timeout for Flash Write */

#define CFG_ENV_ADDR						0
#define CFG_ENV_OFFSET						0x00100000//0x00080000

#define CFG_ENV_SIZE						0x4000	/* 0x8000//Total Size of Environment Sector */

/*
 * TQ210 board specific data
 */

#define CONFIG_IDENT_STRING					" for TQ210"

/* total memory required by uboot */
#define CFG_UBOOT_SIZE						(3*1024*1024)

 /* base address for uboot */
#ifdef CONFIG_ENABLE_MMU
#define CFG_UBOOT_BASE						0xd0020000
#else
#define CFG_UBOOT_BASE						0x23e00000
#endif
#define CONFIG_UBOOT_BASE					CFG_UBOOT_BASE
#define CONFIG_CMD_MEMTEST					1
#define CFG_PHY_UBOOT_BASE					MEMORY_BASE_ADDRESS + 0x3e00000
#define CFG_PHY_KERNEL_BASE					MEMORY_BASE_ADDRESS + 0x8000


/* nand copy size from nand to DRAM.*/
#define	COPY_BL2_SIZE						0x80000

/* NAND configuration */
#define CFG_MAX_NAND_DEVICE  				1
#define CFG_NAND_BASE        				(0xE7200000)
#define NAND_MAX_CHIPS      				1

#define NAND_DISABLE_CE()					(NFCONT_REG |= (1 << 1))
#define NAND_ENABLE_CE()					(NFCONT_REG &= ~(1 << 1))
#define NF_TRANSRnB()						do { while(!(NFSTAT_REG & (1 << 0))); } while(0)

#define CFG_NAND_SKIP_BAD_DOT_I				1  /* ".i" read skips bad blocks   */
#define	CFG_NAND_WP							1
#define CFG_NAND_YAFFS_WRITE				1  /* support yaffs write */
#define CONFIG_TQ210_YAFFS					1  //lhh add
#define CFG_NAND_HWECC 							//lhh del test ubi
#define CONFIG_NAND_BL1_8BIT_ECC 				//lhh del test 8g08
//#define CFG_NAND_FLASH_BBT
#define CONFIG_TQ210_NAND					1	// HJ add

/* IROM specific data */
#define SDMMC_BLK_SIZE						(0xD003A500)
#define COPY_SDMMC_TO_MEM					(0xD003E008)

/* SD/MMC configuration */
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_S3C_HSMMC
#undef DEBUG_S3C_HSMMC

/* The macro for MMC channel 0 is defined by default and can't be undefined */
#define USE_MMC0
#define USE_MMC2
#define MMC_MAX_CHANNEL						4

/*
 * SD/MMC detection takes a little long time
 * So, you can ignore detection process for SD/MMC card
 */
#undef	CONFIG_NO_SDMMC_DETECTION

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_CONCAT

#define MTDIDS_DEFAULT						"nand0=s5pv210-nand"	/* temporary */
#define MTDPARTS_DEFAULT					"mtdparts=s5pv210-nand:1m@0(bios),1m(params),3m(logo),5m(kernel),-(root)"	/* temporary */ 

#if defined(CONFIG_SERIAL1)
#define CONFIG_BOOTARGS						"noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/init console=ttySAC0"
#elif defined(CONFIG_SERIAL2)
#define CONFIG_BOOTARGS						"noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/init console=ttySAC1"
#elif defined(CONFIG_SERIAL3)
#define CONFIG_BOOTARGS						"noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/init console=ttySAC2"
#elif defined(CONFIG_SERIAL4)
#define CONFIG_BOOTARGS						"noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/init console=ttySAC3"
#endif /* CONFIG_SERIALn */

/* OneNAND configuration */
#define CFG_ONENAND_BASE					(0xB0000000)
#define CFG_MAX_ONENAND_DEVICE				1

#define CONFIG_BOOT_ONENAND_IROM
#define CONFIG_NAND
//#define CONFIG_BOOT_NAND
//#define CONFIG_ONENAND
#define ONENAND_REG_DBS_DFS_WIDTH			(0x160)
#define ONENAND_REG_FLASH_AUX_CNTRL			(0x300)

#define GPNCON_OFFSET						0x830
#define GPNDAT_OFFSET						0x834
#define GPNPUD_OFFSET						0x838

//#define CFG_ENV_IS_IN_AUTO
#define CFG_ENV_IS_IN_NAND
//#define CFG_ENV_IS_IN_MOVINAND
/* Fastboot variables */
#define CFG_FASTBOOT_TRANSFER_BUFFER		(0x40000000)
#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE	(0x8000000)   /* 128MB */
#define CFG_FASTBOOT_ADDR_KERNEL			(0xC0008000)
#define CFG_FASTBOOT_ADDR_RAMDISK			(0x30A00000)
#define CFG_FASTBOOT_PAGESIZE				(2048)	// Page size of booting device
#define CFG_FASTBOOT_SDMMC_BLOCKSIZE		(512)	// Block size of sdmmc

/* Just one BSP type should be defined. */
//#define CFG_FASTBOOT_ONENANDBSP
#define CFG_FASTBOOT_NANDBSP
//#define CFG_FASTBOOT_SDMMCBSP //lhh delete

/* LCD setting */
//#define CFG_LCD_TL2796		// for C110 - narrow LCD
#define CFG_LCD_NONAME1			// for V210 - wide LCD
#define CFG_LCD_FBUFFER						(0x48000000)

#define CONFIG_BOOTDELAY					0
#if defined(CFG_FASTBOOT_NANDBSP)
#define CONFIG_BOOTCOMMAND					"nand read.jffs2 0xc0008000 kernel;bootm 0xc0008000"
#elif defined(CFG_FASTBOOT_SDMMCBSP)
#define CONFIG_BOOTCOMMAND					"movi read kernel C0008000; movi read rootfs 30A00000 180000; bootm C0008000 30A00000"
#endif

#define CONFIG_TQ_READ_LINE


#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootargs_defaults=setenv bootargs " \
		"${bootargs} " \
		"lcd=${lcdtype}\0" \
	"bootloaderimgname=u-boot.bin\0"\
	"logoimgname=logo.bin\0"\
	"nkimgname=NK.bin\0"\
	"kernelimgname=zImage.bin\0"\
	"rootimgname=root.bin\0"\
	"nfsipaddr=192.168.1.6\0"\
	"nfsserverip=192.168.1.8\0"\
	"nfsnetmask=255.255.255.0\0"\
	"nfs_dir=/opt/EmbedSky/TQ210/root_nfs\0"


#endif	/* __CONFIG_H */
