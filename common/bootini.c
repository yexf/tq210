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
	FATFS fs;         /* �߼��������Ĺ�����(�ļ�ϵͳ����) */
    FIL file;      /* �ļ����� */
    FRESULT res;         /* FatFs ��������������� */
    UINT br;         /* �ļ���/д�ֽڼ��� */

    /* Ϊ�߼�������ע�Ṥ���� */
    f_mount(&fs, "/", 0);

    /* �������� 1 �ϵ�Դ�ļ� */
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
    	/* ����Դ�ļ���Ŀ���ļ� */
    	res = f_read(&file, buf, 4096, &br);
    	if (res || br == 0) break;
    	__asm__ __volatile__("": : :"memory");
    	memcpy(buffer + offset, buf, br);
    	offset += br;
    }

    /* �رմ򿪵��ļ� */
    f_close(&file);

    return offset;

}

//��ʾ���� sec sd��������������
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
 �� �� ����GetIniKey()
 ���ܸ�Ҫ: ���ָ�������ļ���ָ���ؼ��ֵ�ֵ
 �� �� ֵ: void
 ��    ����const char * filename  �����ļ�����
           const * title     ��Ŀ����
		   const * key       �ؼ�������
		   const * chResult  ȡ�õĽ��
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
//	//�ļ�ָ��
//	FILE * fp = NULL;
//
//	//��ʱ����
//	char tmpLine[1024]="";         // ÿ������ַ���
//	char every_line[1024]="";
//	char tempKey[255]="";          // ������ʱ�ؼ���
//	char tmpTitle[255]="";         // ������ʷ��Ŀ����
//	char * pEveryLine = NULL;
//   	char * pResult = NULL;
//
//	//��ȡ�ļ�����ֵ
//	char * pReturn = NULL;
//
//	int i=0,j=0,k=0,nLen = 0;
//
//	//��־һ����Ŀ�µ�key�Ƿ��Ѿ�ɨ����
//	int flag = 0;
//
//	char * tmp=NULL;
//
//	//���ļ�
//	if ((fp = fopen( filename, "r")) == NULL )
//	{
//		fprintf(stderr,"Open File %s Failed,Reason:%s \n",filename,strerror(errno));
//		return;
//	}
//
//	//��ȡ����
//	while (!feof(fp))
//	{
//		memset(tmpLine,0,1024);
//		memset(every_line,0,1024);
//
//		pReturn = fgets(every_line,1023,fp );    //��ȡһ��
//
//		if ( pReturn == NULL )                //�ļ�������������
//		{
//			break;
//		}
//
//		nLen = strlen(every_line);
//        pEveryLine = every_line;
//        j = 0;
//
//		// ȥ�����׿ո�(����һ����Ϊ�ո��ַ�ǰ�����пո�ȥ��,)
//		for(i =0; i<nLen; i++)
//		{
//			if(every_line[i] == 0x20)
//			{
//				j++;         // �ո���Ŀ
//			}
//			else             // ���ֲ�Ϊ�ո�ʱ���˳���ѭ��
//				break;
//		}
//
//		memcpy(tmpLine,pEveryLine+j,nLen-j);  // ���Ƴ��ո�����ַ�
//        nLen = nLen - j;                      // �������ó���
//
//		// ����ע���л����
//		if( ( tmpLine[0] == ';') ||                           // ';'��ע��
//		    ((tmpLine[0] == '/') && (tmpLine[1] == '/')) ||   // '//'ע��
//		    ((tmpLine[0] == '/') && (tmpLine[1] == '*')) ||   // '/*'ע��
//			((tmpLine[0] == '-') && (tmpLine[1] == '-')) ||   // '--'ע��
//			((tmpLine[0] == '\r')&& (tmpLine[1] == '\n'))||   // windows �»��з�(�޿ո�Ŀ���)
//			 (tmpLine[0] =='\n') )                            // linux/unix�»��з� (�޿ո�Ŀ���)
//		{
//			continue;
//		}
//
//        // ȥ�����з�
//		if( (nLen >= 1) && (tmpLine[nLen-1] == '\n') )      // windows\linux�»��з����Ϊ'\n'
//			tmpLine[nLen-1] = 0;
//		if( (nLen >= 2) && (tmpLine[nLen-2] == '\r') )      //  windows�»��з�Ϊ��'\n'֮ǰ���и�'\r'
//			tmpLine[nLen-2] = 0;
//
//		tmp = strchr(tmpLine, '=');
//
//		//��һ�ζ��� '=' Ӧ���� ��Ŀ����
//		if (( tmp != NULL )&&(flag == 1))
//		{
//			i= j = 0;
//			memset(tempKey,0,255);
//
//			//ȡ���ؼ���
//			while(tmpLine[j] != '=')
//			{
//				if(tmpLine[j] != 0x20)                        //ȥ���ո�
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
//	    	//����ؼ�����ͬ��ȡ��ֵ (ȥ������еĿո�)
//			if(strcmp(tempKey,key) == 0)
//			{
//				nLen = strlen(tmp) - 1;
//				tmp = tmp + 1;          // ���� '='��
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
//				fclose ( fp );        // �ر��ļ�
//
//				return ;
//			}
//		}
//		else
//		{
//			//���flag=1,��˵��ǰһ��title�µ�key�Ѿ�����
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
//			// ������Ŀ����
//			strcpy(tmpTitle,"[");
//			strcat(tmpTitle,title);
//			strcat(tmpTitle,"]");
//
//			// ȷ�϶�������Ϣ�Ƿ�����Ŀ����
//			if( strstr(tmpLine,tmpTitle) != NULL )
//			{
//				flag = 1;   // ��ʶ�ҵ�title
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

