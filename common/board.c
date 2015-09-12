#include <config.h>
#include <common.h>
#include <fat.h>
DECLARE_GLOBAL_DATA_PTR;


#if defined(CONFIG_ENABLE_MMU)
void copy_mmu_table()
{
	FATFS fs;         /* 逻辑驱动器的工作区(文件系统对象) */
	FIL file;      /* 文件对象 */
	FRESULT res;         /* FatFs 函数公共结果代码 */
	UINT br;         /* 文件读/写字节计数 */

	serial_puts("\n####### copy mmu table from mmu.bin ###########\n");

	/* 为逻辑驱动器注册工作区 */
	f_mount(&fs, "/", 0);

	/* 打开驱动器 1 上的源文件 */
	res = f_open(&file, "/mmu.bin", FA_OPEN_EXISTING | FA_READ);
	if (res)
	{
		serial_puts("open /mmu.bin error\n");
		return;
	}

	res = f_read(&file, (void*)CONFIG_MMU_TABLE_BASE, 0x00004000, &br);
	//if (res || br == 0) break;
	res = f_read(&file, (void*)CONFIG_MMU_TABLE_BASE, 0x00004000, &br);
	//if (res || br == 0) break;

	/* 关闭打开的文件 */
	f_close(&file);

	if (res || br == 0)
	{
		serial_puts("read file /mmu.bin error\n");
	}
	else
	{
		serial_puts("read file /mmu.bin succ\n");
	}
}
#endif

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
//
inline void delay_raw(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}
//
inline void delay(unsigned long loops)
{
	unsigned long i;
	for (i = 0; i < loops; i++)
	{
		delay_raw(133333);
	}
}

//显示扇区 sec sd卡的物理扇区号
int show_sd_sector(unsigned long sec)
{
	int i,j;	//16 * 32
	unsigned int loc = sec << 9;
	unsigned char sec_buf[512] = {0};
	unsigned int totol_block = *(volatile unsigned int *)(0xD0037480);

	if (!sd_read(sec, 1, sec_buf))
	{
		debug("Read sector error:sec(%d)\n", sec);
		return 0;
	}

	printf("Show SD Secot:%d/%d\n", sec + 1, totol_block);

	for (i = 0 ; i < 32; i++)
	{
		printf("0x%08X:", loc);
		for (j = 0; j < 16;j++)
		{
			unsigned int off = i * 16 + j;
			printf("%02X ", sec_buf[off]);
		}
		printf("\n");
		loc += 16;
	}
	if (totol_block == ++sec)
		return 0;
	else return sec;
}

//显示扇区 sec sd卡的物理扇区号
void show_mem_sector(unsigned long base)
{
	int i,j;	//16 * 32
	unsigned int loc = base;
	unsigned char *sec_buf = (unsigned char *)base;


	printf("Show Mem Secot:0X%08X\n", base);

	for (i = 0 ; i < 32; i++)
	{
		printf("0x%08X:", loc);
		for (j = 0; j < 16;j++)
		{
			unsigned int off = i * 16 + j;
			printf("%02X ", sec_buf[off]);
		}
		printf("\n");
		loc += 16;
	}
}
void show_mem(unsigned long base, unsigned int size)
{
	unsigned int count = (size + 511) >> 9;
	unsigned int i;
	for (i = 0; i < count; i++)
	{
		udelay(500000);
		show_mem_sector(base + i * 512 );
	}
}
void test_mem_copy()
{
	volatile int *base = (volatile int *)(0x20000000);
	printf("## test_mem_copy at 0x%p ...\n", base);
	printf("## read mem at 0x%p:0x%08X\n", base, *base);
	printf("## write mem at 0x%p:0x%08X\n", base, 0xa5a5a5a5);
	*base = 0xa5a5a5a5;
	printf("## read mem at 0x%p:0x%08X\n", base, *base);
}
void test_read_mem(unsigned int address, unsigned int count)
{
	volatile int *base = (volatile int *)(address);
	while(count--) address = *base;
}
void test_mem_read_speed(unsigned int address, unsigned int count)
{
	unsigned long long t1 = get_ticks();
	test_read_mem(address, count);
	unsigned long long t2 = get_ticks();
	printf("address:0x%08x, count:%d, cost:%d\n", address, count ,t2 - t1);
}

void test_clock_speed()
{
	unsigned long long t1 = get_ticks();
	udelay(1000000);
	unsigned long long t2 = get_ticks();
	printf("tick cost %d per second\n", t2 - t1);

}

void start_armboot(void)
{
	cpu_init();
	serial_init();
	interrupt_init();
	serial_puts("\n########## wxf sd loader for TQ210 ###########\n");
#ifdef SHOW_SD_IMAGE
	int i;
	for (i = 0; i < 32;i++)
		show_sd_sector(i+1);
	test_mem_copy();
#endif

//	test_clock_speed();
//	test_mem_read_speed(0x20000000, 100000000);
//	test_mem_read_speed(0xD0030000, 100000000);
	bootini("/wboot.ini");

	#ifdef CONFIG_COUNT_DELAY
	show_mem(0x20000000, fiel_len);
	while(1)
	{
		static int count = 0;
		udelay(1000000);
		debug("count : %d\n", count++);
	}
	#endif /* CONFIG_COUNT_DELAY  */
}
