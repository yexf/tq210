#include <config.h>
#include <common.h>
#include <fat.h>
typedef struct
{
	char boot[60];			//""
	uint base;				//0xFFFFFFFF
}boot_ini_t;

int isnextline(int ch)
{
	if (ch == '\r' || ch == '\n')
		return 1;
	else return 0;
}

char *get_line(char *src, char *buf)
{
	while(*src)
	{
		if (!isnextline(*src))
			*buf++ = *src++;
		else
		{
			*buf = '\0';
			break;
		}
	}
	while(*src)
	{
		if (isnextline(*src))
			src++;
		else break;
	}

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
		line_data += 4;
		while (isspace(*line_data)) line_data++;
		if (*line_data++ != '=')
		{
			debug("line:%s analysis error\n", line_data);
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
		line_data += 4;
		while (isspace(*line_data)) line_data++;
		if (*line_data++ != '=')
		{
			debug("line:%s analysis error\n", line_data);
			return -1;
		}
		pini_info->base = simple_strtoul(line_data, NULL, 0);
		return 0;
	}
	else
	{
		debug("line:%s analysis error\n", line_data);
		return -1;
	}
}
int analysis_ini(boot_ini_t *pini_info, char *ini_data)
{
	char line_buf[256];
	char *pstr = ini_data;
	int line = 0;
	while(*pstr)
	{
		pstr = get_line(pstr, line_buf);
		debug("line:%d==>>%s\n", ++line, line_buf);
		analysis_ini_line(pini_info, line_buf);
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
unsigned int ff_read_file(const char *pfile_name, char *buffer, unsigned int max_size)
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
    unsigned int offset = 0;
    while (1)
    {
    	char buf[4096] = {0};
    	/* 拷贝源文件到目标文件 */
    	res = f_read(&file, buf, 4096, &br);
    	if (res || br == 0) break;
    	__asm__ __volatile__("": : :"memory");
    	memcpy(buffer + offset, buf, br);
    	offset += br;
    }

    /* 关闭打开的文件 */
    f_close(&file);

    return offset;

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
/*************************************************************
 函 数 名：GetIniKey()
 功能概要: 获得指定配置文件中指定关键字的值
 返 回 值: void
 参    数：const char * filename  配置文件名称
           const * title     项目名称
		   const * key       关键字名称
		   const * chResult  取得的结果
***************************************************************/
//void GetIniKey(const char *filename, const char *title,
//				  const char *key,char *chResult)
//{
//	if(chResult == NULL)
//	{
//		fprintf(stderr,"the last parameter is NULL in GetIniKey()");
//		return ;
//	}
//
//	//文件指针
//	FILE * fp = NULL;
//
//	//临时变量
//	char tmpLine[1024]="";         // 每行最多字符数
//	char every_line[1024]="";
//	char tempKey[255]="";          // 保存临时关键字
//	char tmpTitle[255]="";         // 保存历史项目名称
//	char * pEveryLine = NULL;
//   	char * pResult = NULL;
//
//	//读取文件返回值
//	char * pReturn = NULL;
//
//	int i=0,j=0,k=0,nLen = 0;
//
//	//标志一个项目下的key是否已经扫描完
//	int flag = 0;
//
//	char * tmp=NULL;
//
//	//打开文件
//	if ((fp = fopen( filename, "r")) == NULL )
//	{
//		fprintf(stderr,"Open File %s Failed,Reason:%s \n",filename,strerror(errno));
//		return;
//	}
//
//	//读取内容
//	while (!feof(fp))
//	{
//		memset(tmpLine,0,1024);
//		memset(every_line,0,1024);
//
//		pReturn = fgets(every_line,1023,fp );    //读取一行
//
//		if ( pReturn == NULL )                //文件结束或发生错误
//		{
//			break;
//		}
//
//		nLen = strlen(every_line);
//        pEveryLine = every_line;
//        j = 0;
//
//		// 去除行首空格(将第一个不为空格字符前的所有空格去除,)
//		for(i =0; i<nLen; i++)
//		{
//			if(every_line[i] == 0x20)
//			{
//				j++;         // 空格数目
//			}
//			else             // 出现不为空格时则退出该循环
//				break;
//		}
//
//		memcpy(tmpLine,pEveryLine+j,nLen-j);  // 复制除空格外的字符
//        nLen = nLen - j;                      // 重新设置长度
//
//		// 过滤注释行或空行
//		if( ( tmpLine[0] == ';') ||                           // ';'号注释
//		    ((tmpLine[0] == '/') && (tmpLine[1] == '/')) ||   // '//'注释
//		    ((tmpLine[0] == '/') && (tmpLine[1] == '*')) ||   // '/*'注释
//			((tmpLine[0] == '-') && (tmpLine[1] == '-')) ||   // '--'注释
//			((tmpLine[0] == '\r')&& (tmpLine[1] == '\n'))||   // windows 下换行符(无空格的空行)
//			 (tmpLine[0] =='\n') )                            // linux/unix下换行符 (无空格的空行)
//		{
//			continue;
//		}
//
//        // 去除换行符
//		if( (nLen >= 1) && (tmpLine[nLen-1] == '\n') )      // windows\linux下换行符最后都为'\n'
//			tmpLine[nLen-1] = 0;
//		if( (nLen >= 2) && (tmpLine[nLen-2] == '\r') )      //  windows下换行符为在'\n'之前还有个'\r'
//			tmpLine[nLen-2] = 0;
//
//		tmp = strchr(tmpLine, '=');
//
//		//第一次读到 '=' 应该是 项目名称
//		if (( tmp != NULL )&&(flag == 1))
//		{
//			i= j = 0;
//			memset(tempKey,0,255);
//
//			//取出关键字
//			while(tmpLine[j] != '=')
//			{
//				if(tmpLine[j] != 0x20)                        //去除空格
//				{
//					tempKey[i]=tmpLine[j];
//					i++;
//
//					if(i > 254)
//						break;
//				}
//				j++;
//			}
//
//	    	//如果关键字相同则取其值 (去除结果中的空格)
//			if(strcmp(tempKey,key) == 0)
//			{
//				nLen = strlen(tmp) - 1;
//				tmp = tmp + 1;          // 跳过 '='号
//                pResult = chResult;
//
//				while( nLen > 0)
//				{
//					if( (*tmp) != 0x20 )
//					{
//						*pResult = *tmp;
//						pResult ++;
//					}
//
//                    tmp ++;
//					nLen --;
//				}
//
//				fclose ( fp );        // 关闭文件
//
//				return ;
//			}
//		}
//		else
//		{
//			//如果flag=1,则说明前一个title下的key已经读完
//			if( flag == 1)
//			{
//				fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n", \
//					   title,key,filename);
//				wxf_bzero(chResult,strlen(chResult));
//				fclose(fp);
//				return ;
//			}
//			memset(tmpTitle,0,255);
//
//			// 生成项目名称
//			strcpy(tmpTitle,"[");
//			strcat(tmpTitle,title);
//			strcat(tmpTitle,"]");
//
//			// 确认读到的信息是否有项目名称
//			if( strstr(tmpLine,tmpTitle) != NULL )
//			{
//				flag = 1;   // 标识找到title
//			}
//		}
//	}
//
//	fclose ( fp );
//
//	fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n",\
//		    title,key,filename);
//	wxf_bzero(chResult,strlen(chResult));
//	return;
//}

unsigned int bootini(const char *strBootFile)
{
	char ini_buf[4096] = {0};
	uint file_len = ff_read_file(strBootFile, ini_buf, 4096);
	boot_ini_t ini_info = {{0},0xFFFFFFFF};

	debug("read file:%s len:%d, data:%s\n", strBootFile, file_len, ini_buf);

	if (file_len < 4096 && file_len != 0)
	{

		if (analysis_ini(&ini_info, ini_buf) != 0)
		{
			debug("analysis %s error:boot(%s) base(0x%08X)\n", strBootFile, ini_info.boot, ini_info.base);
			return;
		}
		else
		{
			debug("analysis %s success:boot(%s) base(0x%08X)\n", strBootFile, ini_info.boot, ini_info.base);
		}

		file_len = ff_read_file(ini_info.boot, (char *)ini_info.base, 0);
		show_mem(ini_info.base, file_len);
		go_exec((void *)ini_info.base, 0, NULL);
		debug("read file:%s len:%d\n", ini_info.boot, file_len);

	}
	return ini_info.base;
}

