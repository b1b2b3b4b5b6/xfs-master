
#include "xfs5152.h"
#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
#define XFS_ENCODE 0x00 //XFS文本编码 0x00:GB2312 0x01:GBK 0x02:BIG5 0x03:UNICODE

uint8_t XFS_StopCom[]={0xFD,0X00,0X01,0X02};//停止合成
uint8_t XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//暂停合成
uint8_t XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//恢复合成
uint8_t XFS_ChackCom[]={0XFD,0X00,0X01,0X21};//状态查询
uint8_t XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//进入POWER DOWN 状态命令
uint8_t XFS_PowerUp[]={0XFD,0X00,0X01,0XFF};//进入POWER DOWN 状态命令

void xfs_set(uint8_t *data, int len)
{
	serial_send((char *)data, len);
}

void xfs_speak(char *str)
{
	int size;
    size = sconv_utf8_to_unicode(str, -1, NULL, 0);
    wchar *unicode = STD_CALLOC(size / 2 + 1, sizeof(wchar));
    size = sconv_utf8_to_unicode(str, -1, unicode, size);
    unicode[size / 2] = 0;

    size = sconv_unicode_to_gbk(unicode, -1, NULL, 0);
    char *ansi_str = STD_CALLOC(size + 1, sizeof(wchar));
    size = sconv_unicode_to_gbk(unicode, -1, ansi_str, size);
    ansi_str[size] = 0;

	char *data = STD_MALLOC(strlen(ansi_str) + 5);
	int len = strlen(ansi_str) + 2;
       
	data[0] = 0xFD ; 			//构造帧头FD
	data[1] = (len >> 8)&0xff; 			//构造数据区长度的高字节
	data[2] = len&0xff; 		//构造数据区长度的低字节
	data[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
	data[4] = XFS_ENCODE;       //文本编码格式：GBK 

	memcpy(&data[5], ansi_str, strlen(ansi_str));
	serial_send(data, strlen(ansi_str) + 5); //发送帧配置
	STD_FREE(data);
	STD_FREE(unicode);
	STD_FREE(ansi_str);
}



void xfs_init()
{
	serial_init();
}

