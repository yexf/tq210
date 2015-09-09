#include <config.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

//typedef unsigned int u32;
//typedef unsigned short u16;
typedef u32(*copy_sd_mmc_to_mem)
(u32 channel, u32 start_block, u16 block_size, u32 *trg, u32 init);

static int block_totol = 0;

u32 sd_read_irom(u32 start_block, u16 block_size, u32 *trg)
{
	u32 ch = *(volatile u32 *)(0xD0037488);
	copy_sd_mmc_to_mem copy_bl2 =
		    (copy_sd_mmc_to_mem) (*(u32 *) (0xD0037F98));

	u32 ret;
	if (ch == 0xEB000000)
	{
		ret = copy_bl2(0, start_block, block_size, trg, 0);
	}
	else if (ch == 0xEB200000)
	{
		ret = copy_bl2(2, start_block, block_size, trg, 0);
	}

	return ret;
}

inline unsigned int sd_totle_block(void)
{
	block_totol = *(volatile unsigned int *)(0xD0037480);
	return block_totol;
}

#if defined(CONFIG_ENABLE_MMU)
void copy_mmu_table()
{
	extern u32 mmu_table;
	u32 mmu_base = mmu_table;
	u16 block_size = 32;
	u32 mmu_off = mmu_table - CFG_UBOOT_BASE;
	u32 start_block = mmu_off >> 9 + 1;
	sd_read_iram(start_block, block_size, mmu_base);
}
#endif
void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
static inline void delay_raw(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}
// —” ±loops ∫¡√Î
inline void delay(unsigned long loops)
{
	unsigned long i;
	for (i = 0; i < loops; i++)
	{
		delay_raw(133333);
	}
}

void show_sd_sector(unsigned long sec)
{
	unsigned char sec_buf[512] = {0};
	sd_read_irom(sec, 1, sec_buf);

	unsigned int totol_block = *(volatile unsigned int *)(0xD0037480);
	printf("Show SD Secot:%d/%d\n", sec+1, totol_block);

	int i,j;	//16 * 32
	unsigned int loc = sec << 9;
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


void start_armboot(void)
{
	serial_init();
	serial_puts("\n############ sd loader for TQ210 #############\n");
	int count = 0;
	unsigned int totol_block = *(volatile unsigned int *)(0xD0037480);
	while(1)
	{
		show_sd_sector(count++);
		delay(1000);
		if (totol_block == count)
		{
			count = 0;
		}
	}
}
