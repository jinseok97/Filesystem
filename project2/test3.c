#include "filesystem.h"

/*
   void initDataBlock(Data *pData)
   {
   for(int i=0;i<16;i++)
   pData->dataArr[i] = -1;
   }

   bool preparedataBlock(Data *pData,long long *usabledataBlock,int *dbNum)
   {
   int findarrNum = -1;
   int findbitNum = -1;
   bool Dofind = 0;

   Dofind = findusabledataBlock(usabledataBlock, &findarrNum, &findbitNum);

   if(Dofind == 0)		{ return 0; }

   markdataBlock(usabledataBlock, findarrNum, findbitNum);

 *dbNum = findarrNum * 64 + findbitNum - 1 ;

 initDataBlock(pData+(*dbNum));

 return 1;
 }

 bool allocdbinIDdirect(SuperBlock *pSb, Inode *pInode, Data *pData)
 {
 int dbNum;
 bool Doprepare = 0;

 if((pInode->fileSize)/128 != 0)		{ return 1 ; }

 Doprepare = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);

 if(Doprepare == 0)	{ return 1 ; }

 pInode->direct = dbNum;
 pInode->fileSize += 128 ;

 return 0;
 }


 int allocdbinIDsindirect(SuperBlock *pSb, Inode *pInode, Data *pData)
 {
 int flag = 0;
 long long dbNum;
 bool Doprepare = 0;

 if((pInode->fileSize)/128 < 1)		{ return 0 ; }
 if((pInode->fileSize)/128 >= 104)	{ return 1 ; }

 if((pInode->fileSize)/128 == 1)
 {
 flag = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);
 if(flag == 2)	{ return flag; }
 pInode->sindirect = (short) dbNum;
 }


 flag = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);
 if(flag == 2)	{ return flag; }
 flag = allocbitDbtoDr(pData, dbNum);
 if(flag==1)	{ unmarkdataBlock(pSb->usabledataBlock, (dbNum+1)/64, (dbNum+1)%64);}

 return flag;
 }

*/
/*
   int allocdbinIDdlindirect(SuperBlock *pSb, Inode *pInode, Data *pData)
   {
   int flag = 0;
   long long dbNum;
   int directIndex;
   static int k = 0;
   int abc;

   if(pInode->fileSize/128 < 104)		{ return 3 ; }
   if(pInode->fileSize/128 > 10611)	{ return 1 ; }

   if(pInode->fileSize/128 == 104)		
   {
   flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
   if(flag == 2)	{ return flag; }
   pInode->dlindirect = (short) dbNum;
   pInode->fileSize += 128 ;
   }

   if(((pInode->fileSize)/128 - 105)%103 == 0 )
   {
   flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
   if(flag == 2)	{ return flag; }
   allocbitDrtoSin(pData + pInode->dlindirect, dbNum);
   pInode->fileSize += 128;
   k++;
   }

   if(((pInode->fileSize)/128-105)%103 != 0)
   {
   flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
   if(flag == 2)	{ return flag; }
   directIndex = readbitArr(pData + pInode->dlindirect, k);
   allocbitDbtoDr(pData+directIndex , dbNum);
   pInode->fileSize += 128;
   }

   }
   */
int main(void)
{
	SuperBlock spblock ;

	Inode inode;

	Data dataBlock[1024];

	int findarrNum = -1;
	int findbitNum = -1;

	int flag = 0;
	int resultin =0;
	int result = 0;
	int abc;


	initsuperBlock(&spblock);


	inode.fileSize = 0;

	while(1)
	{
	   	flag=allocdbinIDdirect(&spblock,&inode, dataBlock);

		if(flag == 1)
	 	flag=allocdbinIDsindirect(&spblock,&inode,dataBlock);

		if(flag == 1)
		{
		flag=allocdbinIDdlindirect(&spblock, &inode, dataBlock);	
		if(isBreak(flag))	{ break; }
		}
	}

	/*
	   printf("inode.dlindirect:%hd\n",inode.dlindirect);
	   for(int i=0;i<16;i++)
	   {
	   printf("inode.dlindrect.sindirect[%d]:",i);
	   printBit(dataBlock[inode.dlindirect].dataArr[i]);
	   }

	   for(int i=1;i<103;i++)
	   {
	   result = readbitArr(dataBlock + inode.dlindirect,i);
	   printf("\ninode.dlindirect.sindirect.direct[%d]\n",i);
	   for(int j=0;j<16;j++)
	   printBit(dataBlock[result].dataArr[j]);
	   }
	   */
	printf("inode.fileSize:%d\n",inode.fileSize);
	printf("datablockNum:%d\n",inode.fileSize/128);

	/*
	   for(int i=1;i<11;i++)
	   {
	   resultin = readbitArr(dataBlock + inode.dlindirect, i);
	   printf("resultin:%d\n",resultin);
	   for(int j=1;j<103;j++)
	   {
	   printf("i:%d\n",i);
	   result = readbitArr(dataBlock + resultin, j);
	   printf("result:%d\n",result);
	   }
	   }
	   */


	/*
	   for(int i=0;i<16;i++)
	   printBit(spblock.usabledataBlock[i]);
	   */
	isBreak(flag);

	/*
	   printf("inode.sindirect:%hd\n",inode.sindirect);
	   for(int i=0;i<16;i++)
	   {
	   printf("inode.sindirect.directArr[%d]:",i);
	   printBit(dataBlock[inode.sindirect].dataArr[i]);
	   }
	   */


	/*
	   inode.fileSize = 0;
	   inode.fileSize += 128;

	   while(1)
	   {
	   flag = allocdbinIDsindirect(&spblock,&inode,dataBlock);
	   if(isBreak(flag))	{ break; }
	   }

	   printf("inode.sindirect:%hd\n",inode.sindirect);

	   for(int i=0;i<16;i++)
	   {
	   printf("inode.sindirect.directArr[%d]:",i);
	   printBit(dataBlock[inode.sindirect].dataArr[i]);
	   }

	   for(int i=0;i<16;i++)
	   {
	   printf("spblock.usabledataBlock[%d]:",i);
	   printBit(spblock.usabledataBlock[i]);
	   }

	   printf("flag:%d\n",flag);
	   printf("fileSize:%d\n",inode.fileSize);
	   printf("dataBlockNum:%d\n",inode.fileSize/128);
	   */
	/*
	   printf("inode.direct:%hd\n",inode.direct);
	   flag = allocdbinIDdirect(&spblock,&inode, dataBlock);

	   printf("inode.direct:%hd\n",inode.direct);




	   isBreak(flag);
	   */
	/*
	   for(int i=0;i<103;i++)
	   isErr = allocdbinIDsindirect(&spblock,&inode,dataBlock);

	   isBreak(isErr);

	   printf("inode.sindirect:%lld\n",inode.sindirect);

	   for(int i=0;i<16;i++)
	   {
	   printf("dataArr[%d]:",i);
	   printBit(dataBlock[inode.sindirect].dataArr[i]);
	   }

	   for(int i=0;i<16;i++)
	   {
	   printf("usableDataBlock[%d]:",i);
	   printBit(spblock.usabledataBlock[i]);
	   }
	   */
	/*
	   for(int i=0;i<16;i++)
	   {
	   printf("Inode[%d]:",i);
	   printBit(spblock.usableInode[i]);
	   }
	   for(int i=0;i<16;i++)
	   {
	   printf("Datablock[%d]:",i);
	   printBit(spblock.usableDatablock[i]);
	   }
	   */
	return 0;
}
