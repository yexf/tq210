#include <config.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

//typedef unsigned int u32;
//typedef unsigned short u16;
typedef u32(*copy_sd_mmc_to_mem)
(u32 channel, u32 start_block, u16 block_size, u32 *trg, u32 init);

u32 sd_totle_block()
{
	u32 ret = *(volatile u32 *)(0xD0037480);
	return ret;
}

u32 sd_read_iram(u32 start_block, u16 block_size, u32 *trg)
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
void start_armboot(void)
{
	serial_init();
	serial_puts("\n############ sd loader for TQ210 #############\n");
	int count = 0;
	while(1)
	{
		delay(1000);
		debug("##### count = %d #####\n", count++);
	}
}
