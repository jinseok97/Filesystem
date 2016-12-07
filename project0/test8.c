#include "filesystem.h"

void storedatainDB(Inode *pInode, Data *pData, char c)
{
	long long dbNum;

	dbNum=readDbNuminID(pInode, pData, (pInode->fileSize)/128+1);
	pData[dbNum].file[(pInode->fileSize)%128] = c;
	pInode->fileSize++;
}

void initInode(Inode *pInode)
{
	pInode->fileType = 0;

	time_t now;
	now = time(NULL);
	pInode->t=localtime(&now);

	pInode->direct = 0;
	pInode->sindirect = 0;
	pInode->dlindirect = 0;
}

int main(void)
{
	FILE *ofp = fopen("myfs","wb");
	FILE *ifp = NULL;

	SuperBlock spblock ;

	long long tmp=0;

	Inode inode;

	Data dataBlock[1024];

	char c;

	int arrNum;
	int bitNum;

	int dbNum;
	int idNum;

	int flag;
	int i = 0;

	initsuperBlock(&spblock);	

	inode.fileSize = 0 ;

	DNode Head = {NULL, NULL} ;
	DNode *pTail = &Head;
	DNode *pDNode = NULL;
/*
	fseek(ofp,0,SEEK_SET);
	findusableInode(spblock.usableInode, &arrNum, &bitNum);
	markInode(spblock.usableInode, arrNum, bitNum);
	idNum = arrNum*64 + bitNum - 1;
	printf("idNum:%d\n",idNum);
	fwrite(&spblock,sizeof(SuperBlock),1,ofp);

	for(int i=0;i<16;i++)
		fprintf(ofp,"%lld ",spblock.usableInode[i]);
		
	fflush(ofp);
	fclose(ofp);

	initsuperBlock(&spblock);	
	ifp = fopen("myfs","rb");
	fread(&spblock,sizeof(SuperBlock),1,ifp);
	for(int i=0;i<16;i++)
		printBit(spblock.usableInode[i]);
	fclose(ifp);
*/

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

		storedatainDB(&inode, dataBlock, c);
		/*
		   if(inode.fileSize / 128 == 1)
		   dbNum = readIDDire(&inode);
		   if((inode.fileSize / 128 >= 2) && (inode.fileSize / 128 <=104))
		   dbNum=readIDSinD(&inode, dataBlock, inode.fileSize/128);
		   if((inode.fileSize / 128 >= 105) && (inode.fileSize /128 <=10611))
		   dbNum=readIDDlin(&inode, dataBlock, inode.fileSize/128);
		   */
		//dbNum=readDbNuminID(&inode, dataBlock, i/128+1);
		//dataBlock[dbNum].file[i%128] = c;
		//printf("dataBlock[%d].file[%d]:%c\n",dbNum,i%128,dataBlock[dbNum].file[i%128]);

		/*
		   printf("i:%d\n",i);
		   for(int j=0;j<i%128;j++)
		   printf("dataBlock[%d].file[%d]:%c\n",dbNum,j,dataBlock[dbNum].file[j]);
		   */
		i++;
	}
	for(int i=0;i<16;i++)
		printBit(spblock.usabledataBlock[i]);
	/*
	   for(int i=0;i<128;i++)
	   printf("dataBlock[%d].file[%d]:%c\n",15,i%128,dataBlock[15].file[i%128]);
	   */
	/*
	   for(int i=0;i<128;i++)
	   printf("[%d]:%c\n",i,dataBlock[1].file[i]);

	   printf("inode.fileSize/128:%d\n",inode.fileSize/128);
	   for(int i=1;i<=inode.fileSize/128;i++)
	   {
	   if(i==2 || i==105)		{ i++ ; }
	   if(i>105 && i%103 == 3) 
	   {  i++ ; }


	   dbNum=readDbNuminID(&inode, dataBlock, i);
	   pDNode = createDBNode(dataBlock + dbNum);
	   insertDBNode(&pTail, pDNode);

	   }

*/
	/*
	   int k=0;
	   for(DNode *pDNode = Head.pNext;pDNode != NULL;pDNode = pDNode->pNext)
	   {
	   for(int i=0;i<128;i++)
	   printf("%c",pDNode->pData->file[i]);
	   putchar('\n');
	   }
	   */	
	/*
	   int j=0;
	   for(DNode *pDNode = &Head; pDNode != NULL;pDNode = pDNode->pNext, j++)
	   printf("[%d]:pDNode: %p, pDNode->pNext: %p\n",pDNode->pData - dataBlock,pDNode, pDNode->pNext);

	   for(;pTail != &Head;)
	   deleteDBNode(findDBPrevNode(&Head, pTail),&pTail);


	   j=0;
	   for(DNode *pDNode = &Head; pDNode != NULL;pDNode = pDNode->pNext, j++)
	   printf("[%d]:pNode: %p, pNod->pNext: %p\n",pDNode->pData - dataBlock,pDNode, pDNode->pNext);
	   */

	return 0;
}
