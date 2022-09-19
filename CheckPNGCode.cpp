#include<stdio.h>
#include<windows.h>
int main(int argc, char* argv[])
{
	const char* destFile;
	/*if (argc < 2)
	{
		printf("useage: CheckPng.exe FilePath");
		return 0;
	}
	else
	{
	}*/
	long FileSize;
	destFile = "C:\\Users\\antiy\\Downloads\\pngsucai_607406_c3c768.png";//argv[1];
	FILE* fp = fopen(destFile, "rb");					// 尝试读取(r)一个二进制(b)文件,成功则返回一个指向文件结构的指针，失败返回空指针
	if (fp == NULL)
	{
		printf("PE文件读取失败！\n");
	}
	fseek(fp, 0, SEEK_END);							// 设置文件流指针指向PE文件的结尾处
	FileSize = ftell(fp);							// 得到文件流指针当前位置相对于文件头部的偏移字节数，即获取到了PNG文件大小（字节）

	char* buf = new char[FileSize];				// 新建一个数组指针buf，指向一个以PNG文件字节数作为大小的数组
	memset(buf, 0, FileSize);						// buf指针指向内存中数组的开始位置
													// 在这里用0初始化一块FileSize大小的内存，即为数组分配内存
	fseek(fp, 0, SEEK_SET);							// 将文件流指针指向PNG文件头部
	fread(buf, 8, 1, fp);					// 读取前4个字节的内容，是PNG文件的魔数
		printf("========================================================\n");
		printf("文件大小:%d\n", FileSize);
		printf("========================================================\n");
		//开始解析文件结构
		//开始8个字节是PNG文件的文件署名域		
		printf("=======================文件数据块========================\n");
		int ChunkOffset = 8;
		unsigned int ChunkLen = 0;
		unsigned int ChunkCRC32 = 0;
		int chunknumber = 0;
		while (1)
		{
			chunknumber++;
			fread(buf, 4, 1, fp);//读取第一个数据块的前4字节的数据
			ChunkLen = (buf[0] << 24)| (buf[1] << 16)| (buf[2] << 8)| buf[3];//获取对应字段的长度值
			fread(buf, 4 + ChunkLen, 1, fp);
			printf("[+]ChunkName:%c%c%c%c		",buf[0], buf[1], buf[2], buf[3]);//输出数据块的名称
			if (strncmp((char*)buf, "IHDR", 4) == 0 | strncmp((char*)buf, "PLTE", 4) == 0 | strncmp((char*)buf, "IDAT", 4) == 0| strncmp((char*)buf, "IENF", 4) == 0)
			{
				printf("Critical Chunk\n");
			}
			else
			{
				printf("Ancillary Chunk\n");
			}
			printf("ChunkOffset:0x%08x	\n", ChunkOffset);//输出数据块的偏移值
			printf("ChunkLength:%d      \n", ChunkLen);//输出对应数据块的长度
			ChunkOffset += ChunkLen + 0xC;             //调整偏移到下一个数据块,因为Chunk Type Code的长度为12
			fread(buf, 4, 1, fp);//继续读取4字节数据，这个是crc32的数据
			ChunkCRC32 = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
			printf("ChunkCRC32: %08X		\n", ChunkCRC32);//输出CRC32校验码
			ChunkLen = ftell(fp);//已经读取过数据的大小
			if (ChunkLen == FileSize )  //最后一个数据块的大小固定为12个字节
			{
				printf("\n----------------------------------------------------\n");
				printf("Total ChunkNumber:%d\n", chunknumber);
				break;
			}
		}				
}
