#include <config.h>
#include <common.h>
#include <fat.h>
DECLARE_GLOBAL_DATA_PTR;

typedef unsigned int  (*exe_entry)(int, char *[]);

#if defined(CONFIG_ENABLE_MMU)
void copy_mmu_table()
{
	extern u32 mmu_table;
	u32 mmu_base = mmu_table;
	u16 block_size = 32;
	u32 mmu_off = mmu_table - CFG_UBOOT_BASE;
	u32 start_block = mmu_off >> 9 + 1;
	sdirom_read(0, start_block, block_size, mmu_base);
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
		return;
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
	volatile unsigned char *sec_buf = (volatile unsigned char *)base;


	printf("Show Mem Secot:0x%p\n", sec_buf);

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
void copy_to_mem(void *dst, const void *src, const void *end)
{
	__asm__ __volatile__(
			"loop:ldr r3, [r1]" "\n\t"
			"str r3, [r0]" "\n\t"
			"add	r0, r0, #4" "\n\t"
			"add	r1, r1, #4" "\n\t"
			"cmp	r2, r1" "\n\t"
			"ble	loop" "\n\t"
			:
			:
			:"memory"
	);
}
unsigned int ff_read_file(const char *pfile_name, unsigned int base, unsigned int max_size)
{
	FATFS fs;         /* 逻辑驱动器的工作区(文件系统对象) */
    FIL file;      /* 文件对象 */
    FRESULT res;         /* FatFs 函数公共结果代码 */
    UINT br;         /* 文件读/写字节计数 */

    /* 为逻辑驱动器注册工作区 */
    f_mount(&fs, "/", 0);

    /* 打开驱动器 1 上的源文件 */
    res = f_open(&file, pfile_name, FA_OPEN_EXISTING | FA_READ);
    if (res) return 0;

    if (0 == max_size)
    {
    	max_size = 0xFFFFFFFF;
    }
    unsigned int count = 0;
    while (max_size >= count)
    {
    	char buf[4096] = {0};
    	char *pbuf = buf;
    	/* 拷贝源文件到目标文件 */
    	res = f_read(&file, buf, 4096, &br);
    	if (res || br == 0) break;
    	volatile char *pdst = (volatile char *)(base);
    	while(br--) *pdst++ = *pbuf++;
    	//memcpy(pdst, buf, 4096);
    	//copy_to_mem((volatile char*)(base + count), buf, buf+4096);
    	debug("show buffer:%p\n", buf);
    	show_mem((unsigned int)(buf), 512);
    	debug("show base:0x%08X\n", base + count);
    	show_mem((unsigned int)(base), 512);
    	count += br;
    }
    if (res || br == 0)
    {
    	debug("read file error, ret :%d, count:%d\n", res, count);
    }
    else
    {
    	debug("read file succ, ret :%d, count:%d\n", res, count);
    }
    /* 关闭打开的文件 */
    f_close(&file);

    return count;

}

void test_mem_copy()
{
	volatile char *base = (volatile char *)(0x20000000);
	printf("## test_mem_copy at 0x%p ...\n", base);

	sdirom_read(0, 1, 1, base);

	show_mem(0x20000000, 512);
}

void bootloader()
{
	exe_entry addr;
	volatile char *base = (volatile char *)(0x20000000);
	addr = (exe_entry)base;

	printf("## Booting ldr image at 0x%p ...\n", addr);

	icache_disable();
	dcache_disable();

	uint fiel_len = ff_read_file("/wboot.bin", 0x20000000, 0);
	show_mem(0x20000000, fiel_len);
	addr(0, NULL);
	return 0;
}
void start_armboot(void)
{
	cpu_init();
	serial_init();
	interrupt_init();
	serial_puts("\n############ show sd loader for TQ210 ###########\n");
#ifdef SHOW_SD_IMAGE
	int i;
	for (i = 0; i < 32;i++)
		show_sd_sector(i+1);
#endif

	test_mem_copy();

	//bootloader();
	while(1)
	{
		static int count = 0;
		udelay(1000000);
		debug("count : %d\n", count++);
	}
}
