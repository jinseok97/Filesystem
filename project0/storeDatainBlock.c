#include "command.h"
#include "filesystem.h"


void storeDatainBlock(Inode *pInode, Data *pData, char c)
{
	int dbNum;
	dbNum = readDbNuminID(pInode, pData, (pInode -> fileSize) / 128);
	pData[dbNum].file[(pInode -> fileSize) % 128] = c;
	pInode -> fileSize += 1;
}	
