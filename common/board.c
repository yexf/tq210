#include <config.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

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
//
void start_armboot(void)
{
	cpu_init();
	serial_init();
	interrupt_init();
	serial_puts("\n############ sd loader for TQ210 #############\n");
	bootini("wboot.ini");
	while(1)
	{
		static int count = 0;
		udelay(1000000);
		count = show_sd_sector(count);
	}
}
