#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "time.h"
#include "string.h"
#include "sys/stat.h"   

#define TEA_ROUND            32
#define TEA_DELTA            0x9E3779B9L

uint32_t crc32tab[256] = 
{
 0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
 0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
 0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
 0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
 0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

typedef struct DateTime
{
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint16_t r1;
	uint16_t r2;
} DateTime;

typedef struct FileHead
{
	int8_t startFlag[4];                                                         //起始标志
	int8_t fileExt[4];                                                           //扩展名
	DateTime createTime;                                                        //时间
	uint32_t fileSize;                                                           //文件大小
	uint32_t dataSegmentStartAddress;                                            //数据区起始地址
	uint32_t dataSegmentLength;                                                  //数据区长度
	uint32_t  infoSegmentStartAddress;                                           //信息区起始地址
	uint32_t infoSegmentLength;                                                  //信息区长度

	int8_t modelName[16];                                                        //型号名称
	int8_t mainBoardSN[16];                                                      //主板号
	uint32_t unitNumber;                                                         //单元数量(4byte)
	uint32_t EncryptFlag;                                                        //加密标志，0为不加密

	uint32_t fileHeadLength;                                                     //4byte文件头长度
	uint32_t dataSegmentCrc32;                                                   //4byte 数据区Crc32检验
	uint32_t yxfCreateClientId;                                                  //4byte yxf文件生成客户端ID
	uint32_t updateModuleId;                                                     //4byte 升级模块ID
	int8_t outSideInfo[16];                                                      //16byte 分发到外部时的信息
} FileHead;

typedef struct UnitInfo
{
	int8_t modelName[16];                                                        //16 byte型号名称
	uint32_t unitId;                                                             //4byte 单元编号
	int8_t unitDescribe[16];                                                     //16byte 描述
	int8_t boardSN[16];                                                          //16byte 板号
	int8_t softVersion[8];                                                       //8byte程序版本
	uint32_t dataAddressOffset;                                                  //4byte在数据区的偏移地址
	uint32_t dataLength;                                                         //4byte 数据长度
	uint32_t dataSpaceSize;                                                      //4byte 有效数据长度
	uint32_t dataCrc32;                                                          //4byte 数据crc32

	int8_t reserve[52];                                                          //保留,用于扩展使用
}UnitInfo;

typedef struct FileTail
{
	int8_t r1[4];
	int8_t r2[4];
	int8_t crc[4];                                                               //crc(4byte)
	int8_t endFlag[4];                                                           //结束标志'end'(4byte
}FileTail;

typedef struct
{
	char file_identifier[4]; // 文件识别符,fwf
	char file_inf[4];        // 文件信息
	char file_time[20];      // 文件创建时间

	uint8_t data_startaddress[4]; // 数据区起始地址
	uint8_t data_size[4];         // 数据区大小,默认512开始
	uint8_t data_crc32[4];        // 数据区crc32值

	char model_name[16];           // 型号名称
	char model_Board[16];          // 主板号
	char model_node[16];           // 节点
	char software_version[16];     // 软件版本
	char software_startaddress[4]; // 软件起始地址
} FWF_FileHead;

enum {format_yxf,format_fwf};

uint8_t* byteData;
uint8_t* crptByteData;

uint32_t pkey[4] = { 0x20110524, 0xAA001652, 0x19901979, 0x04220606 };
uint32_t Table_CRC[256];

uint32_t hex_start_adrress;

char cvt_filename[256];
int16_t output_format;
struct stat stat_buf;
struct tm* lt;

char cvt_format[16];
char model_txt[16];
char pcb_txt[16];
char node_txt[16];
char ver_txt[16];
char desc_txt[128];

uint8_t convertHextoInt(char c1, char c2)
{
	uint8_t a, b;
	a = 0; b = 0;
	if (c1 >= 0x30 && c1 <= 0x39)
	{
		a = (uint8_t)(c1 - 0x30);
	}
	else if (c1 >= 0x41 && c1 <= 0x5a)
	{
		a = (uint8_t)(c1 - 0x37);
	}

	if (c2 >= 0x30 && c2 <= 0x39)
	{
		b = (uint8_t)(c2 - 0x30);
	}
	else if (c2 >= 0x41 && c2 <= 0x5a)
	{
		b = (uint8_t)(c2 - 0x37);
	}
	return (uint8_t)(a * 16 + b);
}

uint16_t CRC16(uint8_t* array, uint32_t start, uint32_t end)
{
	uint16_t crc = 0xffff;
	uint32_t i, j;
	uint16_t crc_gen = 0xa001;
	if (start != end)
	{
		for (i = start; i < end; i++)
		{
			crc ^= (uint16_t)(array[i]);
			for (j = 0; j < 8; j++)
			{
				if ((crc & 0x01) != 0)
				{
					crc >>= 1;
					crc ^= crc_gen;
				}
				else
					crc >>= 1;
			}
		}
	}
	return crc;
}

void TeaDeCrypt(char* Src, char* Key)
{
	int32_t n;
	uint32_t x, y;
	uint32_t sum;
	uint32_t* pKey = (uint32_t*)Key;
	uint32_t* pSrc = (uint32_t*)Src;
	uint32_t a = pKey[0], b = pKey[1], c = pKey[2], d = pKey[3];

	sum = (uint32_t)((0x9E3779B9L) << 5);
	x = pSrc[0], y = pSrc[1];
	n = TEA_ROUND;

	while (n-- > 0)
	{
		y -= ((x << 4) + c) ^ (x + sum) ^ ((x >> 5) + d);
		x -= ((y << 4) + a) ^ (y + sum) ^ ((y >> 5) + b);
		sum -= TEA_DELTA;
	}
	pSrc[0] = x; pSrc[1] = y;
}

void DeCrypt(char* pSrc, char* pKey, uint32_t length)
{
	uint32_t LIndex = 0;
	for (LIndex = 0; LIndex < (length >> 3); LIndex++)
	{
		TeaDeCrypt(pSrc + (LIndex << 3), pKey);
	}
}

void TeaCrypt(uint32_t* v, uint32_t* k)
{
	uint32_t l = v[0], r = v[1];
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
	uint32_t delta = 0x9e3779b9;
	uint32_t i;
	uint32_t sum = 0;
	for (i = 0; i < 32; i++)          //核心加密算法，建议32轮，最低16轮
	{
		sum += delta;
		l += ((r << 4) + k0) ^ (r + sum) ^ ((r >> 5) + k1);     //r<<4/r*16
		r += ((l << 4) + k2) ^ (l + sum) ^ ((l >> 5) + k3);
	}
	v[0] = l;
	v[1] = r;
}

void Crypt(char* pSrc, char* pKey, uint32_t length)
{
	uint32_t LIndex = 0;
	for (LIndex = 0; LIndex < (length >> 3); LIndex++)
	{
		TeaCrypt(pSrc + (LIndex << 3), pKey);
	}
}

uint32_t yxf_crc32(uint8_t* buf, uint32_t startaddr, uint32_t endaddr)
{
	uint32_t i;
	uint32_t crc;
	crc = 0;
	for (i = startaddr; i < endaddr; i++)
		crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
	return crc ^ 0;
}

uint32_t ssh_crc32(uint8_t* buf, uint32_t startaddr, uint32_t endaddr)
{
	uint32_t i;
	uint32_t crc;
	crc = 0xFFFFFFFF;
	for (i = startaddr; i < endaddr; i++)
		crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFF;
}

void ConvertHex(char* hex_filename)
{
	//string str = "";
	//string hexfilename = "";
	char ch = 1;
	int16_t ch_length = 0;

	char hexline[50];
	uint8_t hexdata[50];
	uint32_t address = 0; uint8_t recordtype = 0; uint8_t checksum = 0; uint8_t length = 0;
	uint8_t checksum1 = 0;
	uint32_t readBytes = 0;
	uint32_t startaddress = 0;
	uint32_t baseaddress = 0;
	uint32_t ReadState = 0;
	uint32_t ReadLines = 0;
	uint32_t i, j;
	uint32_t writeaddress = 0;
	uint32_t FlashStartAddress = 0;
	uint32_t firstReadAddressFlag = 0;
	FILE* fil = NULL;
	int32_t fd;
	bool finishflag = true;

	FileHead main_head;
	UnitInfo main_info;
	FileTail main_tail;
	uint8_t decrpt_array[64];
	uint32_t file_length;
	uint32_t numRead;
	uint32_t hex_data;

	FWF_FileHead fwf_main_head;


	fil = fopen(hex_filename, "rb");
	fd = fileno(fil);
	fstat(fd, &stat_buf);
	lt = localtime(&stat_buf.st_mtime);
	printf("Start Convert....\n");
	printf("Target:%s\n", hex_filename);
	printf("Hex created time:%04d-%02d-%02d %02d:%02d:%02d\n", lt->tm_year+1900, \
												lt->tm_mon+1, \
												lt->tm_mday, \
												lt->tm_hour, \
												lt->tm_min, \
												lt->tm_sec);
	printf("Extension:%s\n", cvt_format);
	printf("Model:%s\n", model_txt);
	printf("Pcb:%s\n", pcb_txt);
	printf("Node:%s\n", node_txt);
	printf("Version:%s\n", ver_txt);
	printf("Description:%s\n\n", desc_txt);

	if (fil != NULL)
	{

		finishflag = true;
		ReadState = 0;
		while (finishflag)
		{
			ch = 1;
			ch_length = 0;
			while (ch != '\n')
			{
				ch = fgetc(fil);
				hexline[ch_length++] = ch;
			}
			ReadLines++;
			if (hexline[0] == ':')
			{
				for (i = 1; i < ch_length; i = i + 2)
				{
					hexdata[(i - 1) / 2] = convertHextoInt(hexline[i], hexline[i + 1]);
				}
				length = hexdata[0];
				address = hexdata[1] * 256 + hexdata[2];
				recordtype = hexdata[3];
				checksum = hexdata[4 + length];
				checksum1 = 0;
				for (i = 0; i < (length + 4); i++)
				{
					checksum1 += hexdata[i];
				}
				checksum1 ^= 0xff;
				checksum1++;
				if (checksum1 != checksum)
				{
					printf("CheckSum error!\n");
					finishflag = false;
				}
				if (recordtype == 1)
				{
					//MessageBox.Show("hex读取完毕！");
					ReadState = 1;
					readBytes = writeaddress + 1;
					printf("TotalBytes:0x%x\n", readBytes);
					finishflag = false;
				}
				//else if (recordtype == 0)
				//{
				//    //写入数据
				//    //如果是第一行数据
				//    if (readBytes == 0)
				//    {
				//        startaddress = (baseaddress << 16) + address;
				//        label8.Text = "0x" + startaddress.ToString("X");
				//    }
				//    for (i = 0; i < length; i++)
				//    {
				//        byteData[0x200 + readBytes + i] = hexdata[4 + i];
				//    }
				//    readBytes += length;
				//}
				else if (recordtype == 0)
				{
					//写入数据
					//如果是第一行数据
					startaddress = (uint32_t)((baseaddress << 16) + address);
					if (startaddress <= 0x8200000)
					{
						if (firstReadAddressFlag == 0)
						{
							FlashStartAddress = startaddress;
							hex_start_adrress = startaddress - 0x8000000;
							printf("Address:0x%x\n", startaddress);
							firstReadAddressFlag = 1;
						}
						for (i = 0; i < length; i++)
						{
							writeaddress = (uint32_t)(startaddress + i - FlashStartAddress);
							byteData[0x200 + writeaddress] = hexdata[4 + i];
						}
					}
				}
				else if (recordtype == 2)
				{
					//MessageBox.Show("扩展段地址");
				}
				else if (recordtype == 3)
				{
					//MessageBox.Show("开始段地址");
				}
				else if (recordtype == 4)
				{
					//MessageBox.Show("扩展线性地址");
					baseaddress = (uint32_t)(hexdata[4] * 256 + hexdata[5]);
				}
				else if (recordtype == 5)
				{
					//MessageBox.Show("开始线性段地址");
				}
			}
			else
			{
				printf("hex is corrupted！\n");
				finishflag = false;
			}
		}
		fclose(fil);

		if (ReadState == 1)
		{
			uint32_t crc32 = 0;
			uint16_t crc16 = 0;
			uint32_t ntemp = 0;

			uint16_t page_counts = 0;
			uint16_t page_left = 0;
			uint16_t check_line = 0;
			page_counts = readBytes / 64;
			page_left = readBytes % 64;

			if (page_left != 0)page_counts += 1;
			for (j = 0; j < page_counts; j++)
			{
				for (i = 0; i < 64; i++)
					decrpt_array[i] = byteData[0x200 + j * 64 + i];
				Crypt(decrpt_array, pkey, 64);
				for (i = 0; i < 64; i++)
					crptByteData[0x200 + j * 64 + i] = decrpt_array[i];

			}
			//check_line = 5752;
			//for (i = check_line*16; i < (check_line * 16 + 16); i=i+4)
			//{
			//	hex_data = byteData[0x200 + i] + (byteData[0x200 + i + 1] << 8) + (byteData[0x200 + i + 2] << 16) + (byteData[0x200 + i + 3] << 24);
			//	printf("%08x,", hex_data);
			//}
			//printf("\n");
			//for (i = check_line * 16; i < (check_line * 16 +16); i++)
			//{
			//	printf("%02x ", crptByteData[0x200 + i]);
			//	if (i % 16 == 15)printf("\n");
			//}
			//printf("\n");
			if (output_format == format_yxf)
			{
				memset(&main_head, 0, sizeof(main_head));
				memset(&main_info, 0, sizeof(main_info));
				memset(&main_tail, 0, sizeof(main_tail));
				main_head.startFlag[0] = 0xea;
				main_head.startFlag[1] = 0x54;
				main_head.startFlag[2] = 0x00;
				main_head.startFlag[3] = 0x00;
				sprintf(main_head.fileExt, "yxf");
				main_head.createTime.year = lt->tm_year + 1900;
				main_head.createTime.month = lt->tm_mon + 1;
				main_head.createTime.day = lt->tm_mday;
				main_head.createTime.hour = lt->tm_hour;
				main_head.createTime.minute = lt->tm_min;
				main_head.createTime.second = lt->tm_sec;
				main_head.createTime.r1 = 0;
				main_head.createTime.r2 = 0;
				main_head.fileSize = (readBytes / 256 + 1) * 256 + 0x210;
				main_head.dataSegmentStartAddress = 0x200;
				main_head.dataSegmentLength = (readBytes / 256 + 1) * 256;
				main_head.infoSegmentStartAddress = 0x100;
				main_head.infoSegmentLength = 0x100;
				sprintf(main_head.modelName, model_txt);
				sprintf(main_head.mainBoardSN, pcb_txt);
				main_head.unitNumber = 1;
				main_head.EncryptFlag = 1;
				main_head.fileHeadLength = sizeof(FileHead);
				main_head.dataSegmentCrc32 = 0;
				main_head.yxfCreateClientId = 0x132db15;
				main_head.updateModuleId = 0;
				for (i = 0; i < 16; i++)main_head.outSideInfo[i] = 0;
				sprintf(main_info.modelName, model_txt);
				main_info.unitId = 1;
				sprintf(main_info.unitDescribe, desc_txt);
				sprintf(main_info.boardSN, pcb_txt);
				sprintf(main_info.softVersion, ver_txt);
				main_info.dataAddressOffset = 0;
				main_info.dataLength = readBytes;
				main_info.dataSpaceSize = (readBytes / 256 + 1) * 256;
				main_info.dataCrc32 = yxf_crc32(byteData , 0x200, 0x200 + readBytes);
				printf("CheckSum:0x%0x\n", main_info.dataCrc32);
				sprintf(main_tail.endFlag, "end");
				memcpy(crptByteData, &main_head, sizeof(FileHead));
				memcpy(crptByteData + 0x100, &main_info, sizeof(UnitInfo));
				memcpy(crptByteData + main_head.fileSize - 0x10, &main_tail, sizeof(main_tail));

				fil = fopen(cvt_filename, "wb");
				if (fil != NULL)
				{
					fwrite(crptByteData, main_head.fileSize, 1, fil);
					fclose(fil);
					printf("Convert to '.yxf' finished!\n");
				}
				else
				{
					printf("Create .yxf file failed!\n");
				}
			}
			else
			{
				byteData[0x0] = '.';   //起始标志
				byteData[0x1] = 'f';
				byteData[0x2] = 'w';
				byteData[0x3] = 'f';   //扩展名
				byteData[0x4] = '1';
				byteData[0x5] = '.';
				byteData[0x6] = '0';
				byteData[0x7] = '0';
				sprintf(&byteData[0x8],"%04d-%02d-%02d %02d:%02d%02d\n", lt->tm_year + 1900, \
					lt->tm_mon+1, \
					lt->tm_mday, \
					lt->tm_hour, \
					lt->tm_min, \
					lt->tm_sec);

				ntemp = 512;
				byteData[0x1c] = (ntemp & 0xff);
				byteData[0x1d] = ((ntemp & 0xffff) >> 8);
				byteData[0x1e] = ((ntemp & 0xff0000) >> 16);
				byteData[0x1f] = ((ntemp & 0xffff0000) >> 24);

				ntemp = readBytes;
				byteData[0x20] = (ntemp & 0xff);
				byteData[0x21] = ((ntemp & 0xffff) >> 8);
				byteData[0x22] = ((ntemp & 0xff0000) >> 16);
				byteData[0x23] = ((ntemp & 0xffff0000) >> 24);

				crc32 = ssh_crc32(byteData, 0x200, 0x200 + readBytes);
				crc16 = CRC16(byteData, 0x200, 0x200 + readBytes);
				ntemp = crc32;
				byteData[0x24] = (ntemp & 0xff);
				byteData[0x25] = ((ntemp & 0xffff) >> 8);
				byteData[0x26] = ((ntemp & 0xff0000) >> 16);
				byteData[0x27] = ((ntemp & 0xffff0000) >> 24);
				printf("CheckSum:0x%0x\n",crc32);

				sprintf(&byteData[0x28], model_txt);
				sprintf(&byteData[0x38], pcb_txt);
				sprintf(&byteData[0x48], node_txt);
				sprintf(&byteData[0x58], ver_txt);
				byteData[0x68] = (FlashStartAddress & 0xff);
				byteData[0x69] = ((FlashStartAddress & 0xffff) >> 8);
				byteData[0x6a] = ((FlashStartAddress & 0xff0000) >> 16);
				byteData[0x6b] = ((FlashStartAddress & 0xffff0000) >> 24);
				sprintf(&byteData[0x100], desc_txt);

				byteData[0x200 + readBytes] = ((crc16 & 0xffff) >> 8);
				byteData[0x200 + readBytes + 1] = (crc16 & 0xff);
				byteData[0x200 + readBytes + 2] = 0;
				byteData[0x200 + readBytes + 3] = 0;

				byteData[0x200 + readBytes + 4] = 0x65;
				byteData[0x200 + readBytes + 5] = 0x6e;
				byteData[0x200 + readBytes + 6] = 0x64;
				byteData[0x200 + readBytes + 7] = 0;

				fil = fopen(cvt_filename, "wb");
				if (fil != NULL)
				{
					fwrite(byteData, 0x200 + readBytes + 8, 1, fil);
					fclose(fil);
					printf("Convert to '.fwf' finished!\n");
				}
				else
				{
					printf("Create .fwf file failed!\n");
				}
			}
		}
	}
	else
	{
		printf("Hex file dosen't exist!\n");
	}
}

read_line(char* sss,FILE* fp)
{
	char ch = 0;
	int16_t ch_idx = 0;
	while (ch != '\n')
	{
		ch = fgetc(fp);
		if(ch != '\r' && ch != '\n')
		sss[ch_idx++] = ch;
	}
	sss[ch_idx] = 0;
	return sss;
}

void main(int argc, char* argv[])
{
	if (argc == 3)
	{
		FILE* fp = NULL;
		strcpy(cvt_filename, argv[1]);

		fp = fopen(argv[2], "rb");
		if (fp != NULL)
		{
			 read_line(cvt_format,fp);
			 read_line(model_txt, fp);
			 read_line(pcb_txt, fp);
			 read_line(node_txt, fp);
			 read_line(ver_txt, fp);
			 read_line(desc_txt, fp);
			 fclose(fp);
			if (strcmp(cvt_format, "yxf") == 0)
			{
				output_format = format_yxf;
				strcat(cvt_filename, ".yxf");
			}
			else if (strcmp(cvt_format, "fwf") == 0)
			{
				output_format = format_fwf;
				strcat(cvt_filename, ".fwf");
			}
			else
			{
				printf("Output format is not specified!");
				exit(-1);
			}

			byteData = __vcrt_malloc_normal(0x200000);
			crptByteData = __vcrt_malloc_normal(0x200000);
			memset(byteData, 0x0, 0x200000);
			memset(crptByteData, 0x0, 0x200000);
			ConvertHex(argv[1]);
			__vcrt_free_normal(byteData);
			__vcrt_free_normal(crptByteData);
			fclose(fp);
		}
		else
		{
			printf("Convert ini file doesn't exist!\n");
		}
	}
	else
	{
		printf("parameters is not found!\n");
	}
}