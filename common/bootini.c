#include <config.h>
#include <common.h>
#include <fat.h>
typedef struct
{
	char boot[60];			//""
	uint base;				//0xFFFFFFFF
}boot_ini_t;

char *get_line(char *src, char *buf)
{
	while(*src != '\0' && *src != '\r' && *src != '\n')
	{
		*buf++ = *src++;
	}
	while (*src == '\r' || *src == '\n')   src++;
	return src;
}

int isspace(int ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch =='\b') return 1;
	return 0;
}

int analysis_ini_line(boot_ini_t *pini_info, const char *line_data)
{
	while (isspace(*line_data)) line_data++;

	if (memcmp(line_data, "boot", 4) == 0)
	{
		while (isspace(*line_data)) line_data++;
		if (*line_data++ != '=')
		{
			return -1;
		}
		while (isspace(*line_data)) line_data++;
		memset(pini_info->boot, 0, 60);
		char *pboot = pini_info->boot;
		while (!isspace(*line_data) && *line_data != '\0')
			*pboot++ = *line_data++;
	}
	else if (memcmp(line_data, "base", 4) == 0)
	{
		while (isspace(*line_data)) line_data++;
		if (*line_data++ != '=')
		{
			return -1;
		}
		pini_info->base = simple_strtoul(line_data, NULL, 0);
		return 0;
	}
	else
	{
		return -1;
	}
}
int analysis_ini(boot_ini_t *pini_info, char *ini_data)
{
	while (*ini_data)
	{
		char ini_line[256] = {0};
		ini_data = get_line(ini_data, ini_line);
		analysis_ini_line(pini_info, ini_line);
	}
	if (pini_info->base == 0xFFFFFFFF || pini_info->boot[0] == '\0')
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
__attribute__((weak))
unsigned long go_exec (ulong (*entry)(int, char *[]), int argc, char *argv[])
{
	return entry (argc, argv);
}

void bootini(const char *strBootFile)
{
	block_dev_desc_t sd_dev;
	get_sdirom_dev(&sd_dev);
	if (fat_register_device(&sd_dev, 0) == 0)
	{
		file_fat_ls("/");
	}

	char ini_buf[4096] = {0};
	uint file_len = file_fat_read(strBootFile, ini_buf, 4096);

	if (file_len < 4096)
	{
		boot_ini_t ini_info = {{0},0xFFFFFFFF};
		if (analysis_ini(&ini_info, ini_buf) == 0)
		{
			file_fat_read(ini_info.boot, ini_info.base, 0);
			go_exec((void *)ini_info.base, 0, NULL);
		}
	}
}

