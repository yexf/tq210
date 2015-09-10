/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
#include "sd.h"			/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SDCARD 0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	switch (pdrv) {
    case SDCARD:
        if (sd_init() == 0)
			stat = 0;
        break;
    default:
		stat = STA_NOINIT;
        break;	
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int res = 0;

    if (!count)
        return RES_PARERR;//count不能等于0，否则返回参数错误
    switch(pdrv)
    {
        case SDCARD://SD卡
            res = sd_read(sector, (WORD)count, (char *)buff);
            break;
        default:
            res = 0;
    }
   //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
        return RES_ERROR;
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	BYTE res = 0;
    if (!count)
        return RES_PARERR;//count不能等于0，否则返回参数错误
    switch(pdrv)
    {
        case SDCARD://SD卡
            res = sd_write(sector, count,buff);
            break;
        default:
            res = 0;
    }
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
        return RES_ERROR;
    return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
 DRESULT res;
    if(pdrv == SDCARD)//SD卡
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            //            if (!SDHC_WaitForCard2TransferState(&sCh))
            //                res = RES_ERROR;
            //                if(SDCardWaitReady() == 0)
            //                    res = RES_OK;
            //                else
            //                    res = RES_ERROR;
            //                SDCardDisSelect();
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = (WORD)sd_info(si_sector_size);
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = (WORD)sd_info(si_block_size);
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = (DWORD)sd_info(si_sector_count);
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    } else
        res = RES_ERROR;//其他的不支持
    return res;

	return RES_PARERR;
}
#endif
