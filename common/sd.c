#include <config.h>
#include <common.h>
#include <sd.h>

typedef u32(*copy_sd_mmc_to_mem)
	(u32 channel, u32 start_block, u16 block_size, u32 *trg, u32 init);

int sd_info(sd_info_em em_info)
{
	switch (em_info)
	{
	case si_sector_count:
		return *(volatile unsigned long *)(0xD0037480);
	case si_sector_size:
		return 512;
	case si_block_size:
		return 512;
	default:break;
	}
	return 0;
}
int sd_read (unsigned int start,unsigned short block_size,unsigned char *buffer)
{
	copy_sd_mmc_to_mem copy_bl2 =
			(copy_sd_mmc_to_mem) (*(u32 *) (0xD0037F98));

	unsigned int ch = *(volatile unsigned int *)(0xD0037488);

	if (0xEB000000 == ch)
	{
		return copy_bl2(0, start, block_size, buffer, 0);
	}
	else if (0xEB200000 == ch)
	{
		return copy_bl2(2, start, block_size, buffer, 0);
	}

	return 0;
}
int sd_write (unsigned int sector,unsigned short count,const unsigned char *buff)
{
	return 0;
}
int sd_init ()
{
	return 0;
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

//return ==>> success : 1 error : 0
unsigned long sdirom_read(int dev, unsigned long start, lbaint_t blkcnt, void *buffer)
{
	copy_sd_mmc_to_mem copy_bl2 =
		    (copy_sd_mmc_to_mem) (*(u32 *) (0xD0037F98));

	u16 block_size = (u16)(blkcnt & 0x0000FFFF);
	if (0xEB000000 == dev)
	{
		return copy_bl2(0, start, block_size, buffer, 0);
	}
	else if (0xEB200000 == dev)
	{
		return copy_bl2(2, start, block_size, buffer, 0);
	}
	else
	{
		debug("Unknow Dev:%d\n", dev);
		return 0;
	}
}
int get_sdirom_dev(block_dev_desc_t *pdev)
{
	int ret = *(volatile int *)(0xD0037488);
	if (pdev != NULL)
	{
		pdev->dev = ret;
		pdev->lba = *(volatile unsigned long *)(0xD0037480);
		pdev->blksz = 512;
		pdev->block_read = sdirom_read;
		pdev->block_write = NULL;

		pdev->priv = NULL;
		pdev->if_type = IF_TYPE_MMC;			/* type of the interface */
		pdev->part_type = PART_TYPE_DOS;		/* partition type */
		pdev->target = 0;						/* target SCSI ID */
		pdev->lun = 0;							/* target LUN */
		pdev->type = DEV_TYPE_OPDISK;			/* device type */
		pdev->removable = 0;					/* removable device */
		pdev->vendor [0] = 0;		/* IDE model, SCSI Vendor */
		pdev->product [0] = 0;		/* IDE Serial no, SCSI product */
		pdev->revision [0] = 0;		/* firmware revision */
	}
	return ret;
}



