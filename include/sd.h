/************************************************************************************** 
** Copyright (C) 2013,Inc. All rights reserved
**
** Filename : tq210.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/12/19
**
** Description:
**************************************************************************************/ 
#ifndef _sd_h_
#define _sd_h_

#define DEFLAUT_SDIROM_DEV					(0xEB000000)

typedef enum
{
	si_ctrl_sync = 0,
	si_sector_count,
	si_sector_size,
	si_block_size,
	si_erase_sector,
}sd_info_em;

int sd_info(sd_info_em em_info);
int sd_read (unsigned int sector,unsigned short count,unsigned char *buff);
int sd_write (unsigned int sector,unsigned short count,const unsigned char *buff);
int sd_init (void);


#endif /*_sd_h_*/
