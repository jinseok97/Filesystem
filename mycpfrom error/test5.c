#include "filesystem.h"
#include <string.h>

void storeDatainBlock(Inode *pInode,Data *pData, char c)
{
	int dbNum; 

	dbNum=readDbNuminID(pInode, pData, (pInode->fileSize)/128);

	pData[dbNum].file[(pInode->fileSize)%128] = c;

	pInode->fileSize += 1;

}


int main(void)
{
	SuperBlock spblock;

	Inode inode;

	Data dataBlock[1024];

	char c;

	int dbNum;

	int flag;
	int i = 0;

	initsuperBlock(&spblock);	

	inode.fileSize = 0 ;

	DNode Head = {NULL, NULL} ;
	DNode *pTail = &Head;
	DNode *pDNode = NULL;

	while((c=getchar()) != '\n')
	{
		if(inode.fileSize % 128 == 0)
		{
			flag=allocdbinIDdirect(&spblock,&inode, dataBlock);

			if(flag == 1)
				flag=allocdbinIDsindirect(&spblock,&inode,dataBlock);

			if(flag == 1)
			{
				flag=allocdbinIDdlindirect(&spblock, &inode, dataBlock);	
				if(isBreak(flag))	{	break;	}
			}
		}

				storeDatainBlock(&inode, dataBlock, c);
		
		i++;
	}



	printf("inode.sindirect:%d\n",inode.sindirect);
	printf("inode.dlindirect:%d\n",inode.dlindirect);
	printf("inode.fileSize:%d\n",inode.fileSize);


	for(int i=0;i<=(inode.fileSize-1)/128;i++)
	{
		dbNum=readDbNuminID(&inode, dataBlock, i);
		pDNode = createDBNode(dataBlock + dbNum);
		insertDBNode(&pTail, pDNode);
	}

	for(DNode *pIndex=Head.pNext;pIndex!=NULL;pIndex=pIndex->pNext)
		for(int i=0;i<128;i++)
			printf("[%d][%d]:%c\n",pIndex->pData-dataBlock,i,pIndex->pData->file[i]);


	return 0;
}
