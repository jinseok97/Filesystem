#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "filesystem.h"

unsigned long long movelluBit(int n)
{
	int i;
	unsigned long long bit = 1;

	for(i=n;i<64;i++)
		bit <<= 1;

	return bit ;
}


void printBit(unsigned long long a)
{
	int i;
	int n = sizeof(unsigned long long) * 8;
	unsigned long long mask = movelluBit(1);


	for(i=1;i<=n;++i)
	{
		putchar(((a & mask) == 0) ? '0' : '1');
		a<<=1;
		if(i % 8 == 0 && i < n)
			putchar(' ');
	}

	putchar('\n');

}

void initsuperBlock(SuperBlock *pSb)
{
	for(int i=0;i<16;i++)
	{
		pSb->usableInode[i] = -1;
		pSb->usabledataBlock[i] = -1;
	}
}

bool findusableInode(/*unsigned*/ long long *arrInode, int* findarrNum, int *findbitNum)
{
	int i, j;
	/*unsigned*/ long long mask = movelluBit(1);
	/*unsigned*/ long long curInode;

	for(i=0;i<UI;i++)
	{
		curInode = arrInode[i];
		for(j=1;j<=64;j++)
		{
			if((curInode & mask) == mask)// & mask))
			{
				*findarrNum = i ;
				*findbitNum = j ;
				return 1 ;
			}
			curInode <<= 1;
		}
	}

	return 0;
}

void markInode(/*unsigned*/ long long * arrInode, int arrNum, int bitNum)
{
	/*unsigned*/ long long marking = movelluBit(bitNum);

	arrInode[arrNum] /*+*/^= marking ;

}

void unmarkInode(/*unsigned*/ long long *arrInode, int arrNum, int bitNum)
{
	/*unsigned*/ long long unmarking = movelluBit(bitNum);

	arrInode[arrNum] |= unmarking ;
}

bool findusabledataBlock(/*unsigned*/ long long *arrData, int * findarrNum, int *findbitNum)
{
	int i, j;
	/*unsigned*/ long long mask = movelluBit(1);
	/*unsigned*/ long long curData;

	for(i=0;i<UD;i++)
	{
		curData = arrData[i];
		for(j=1;j<=64;j++)
		{
			if(((curData &mask) == mask)) //^ mask) & mask) )
			{
				*findarrNum = i ;
				*findbitNum = j ;
				return 1 ;
			}
			curData <<= 1;
		}
	}

	return 0;

}

void markdataBlock(/*unsigned*/ long long * arrData, int arrNum, int bitNum)
{
	/*unsigned*/ long long marking = movelluBit(bitNum);

	arrData[arrNum] /*+*/^= marking ;
}

void unmarkdataBlock(/*unsigned*/ long long *arrD, int arrNum, int bitNum)
{
	/*unsigned*/ long long unmarking = movelluBit(bitNum);

	arrD[arrNum] /*^*/|= unmarking ;
}

void initdataBlock(Data *pDb)
{
	int abc;
	for(int i=0;i<16;i++)
		pDb->dataArr[i] = -1;
}

bool checkbitArr(long long *pDb, long long *arrNum, long long *bitNum)
{
	long long mask10 = 0;
	long long tmp[16];
	long long result = 0;

	int k = 1;
	int abc;



	for(int i=1;i<11;i++)
		mask10 +=  (long long) movelluBit(i);

	for(int i=0;i<16;i++)
		tmp[i] = pDb[i];

	while(1)
	{
		result = (mask10 & tmp[0]);
		if((result&mask10) == mask10)
		{
			*arrNum =(k*10)/64 ;
			*bitNum =(k*10)%64 ;
			break;
		}

		linkelementBit(tmp);
		if(k==102)		{ return 1; }
		k++ ;
	}

	return 0;
}

void linkelementBit(long long *tmp)
{
	long long mask1=movelluBit(1);

	tmp[0]<<=10;
	for(int i=1;i<16;i++)
		for(int j=0;j<10;j++)
		{
			if((mask1 & tmp[i]) == mask1)
				tmp[i-1] += movelluBit(64-(10-j)+1);
			tmp[i] <<= 1;
		}
}

void storedbNum(long long *pDb, long long arrNum, long long bitNum, long long dbNum)
{
	if(arrNum == -1 || bitNum == -1)	{ return ; }

	if(bitNum >= 10)
		for(int i=0;i<10;i++)
		{
			if(dbNum % 2 == 0)
			{
				pDb[arrNum] -= movelluBit(bitNum - i);
			}
			dbNum /= 2;
		}

	if(bitNum < 10)
	{
		for(int i=0;i<bitNum;i++)
		{
			if(dbNum%2 == 0)
				pDb[arrNum] -= movelluBit(bitNum - i);
			dbNum /= 2;
		}
		for(int i=0;i<10-bitNum;i++)
		{
			if(dbNum%2 == 0)
				pDb[arrNum-1] -= movelluBit(64 - i);
			dbNum /= 2;
		}

	}
}

int allocDbtoDb(Data *pData, long long dbNum)
{
	long long arrNum = -1, bitNum = -1;
	bool isFull = 0;

	if(dbNum >= 1024)	{ return 2;}
	isFull = checkbitArr(pData->dataArr, &arrNum, &bitNum);

	if(isFull)		{ return 1 ; }

	storedbNum(pData->dataArr,arrNum,bitNum,dbNum);

	return 0;
}

long long readbitArr(Data *pData, int k)
{
	long long mask1 = movelluBit(1);
	long long mask10 = 0;
	long long result = 0;
	long long mid;
	long long tmp[16];


	for(int i=1;i<=10;i++)
		mask10 += movelluBit(i);

	for(int i=0;i<16;i++)
		tmp[i] = pData->dataArr[i];

	for(int i=1;i<k;i++)
		linkelementBit(tmp);

	mid=(tmp[0] & mask10) ;

	for(int i=0;i<10;i++)
	{
		if((mask1 & mid)== mask1)
			result += movelluBit(64-(10-i)+1);
		mid<<=1;
	}

	return result;
}

int allocbitDbtoDr(Data *pData, long long dbNum)
{
	int flag = 0;

	flag = allocDbtoDb(pData, dbNum);

	return flag;
}

int allocbitDrtoSin(Data *pData, long long dbNum)
{
	int flag = 0;

	flag = allocDbtoDb(pData, dbNum);

	return flag;
}

bool isBreak(int flag)
{
	if(flag == 1)	
	{
		printf("추가적으로 데이터 블럭을 할당할 수 없습니다.\n");
		return 1;
	}
	if(flag == 2)
	{
		printf("모든 데이터 블럭을 다 할당 하였습니다.\n");
		return 1;
	}
	if(flag == 3)
	{
		printf("이전 direct가 아직 다 할당 되지 않았습니다.\n");
		return 1;
	}

	return 0;
}

void initDataBlock(Data *pData)
{
	for(int i=0;i<16;i++)
		pData->dataArr[i] = -1;
}

int preparedataBlock(Data *pData,long long *usabledataBlock,long long *dbNum)
{
	int findarrNum = -1;
	int findbitNum = -1;
	bool Dofind = 0;

	Dofind = findusabledataBlock(usabledataBlock, &findarrNum, &findbitNum);

	if(Dofind == 0)		{ return 2; }

	markdataBlock(usabledataBlock, findarrNum, findbitNum);

	(*dbNum) = findarrNum * 64 + findbitNum - 1 ;
	//printf("dbNum:%d\n",*dbNum);

	initDataBlock(pData+(*dbNum));

	return 0;
}

int allocdbinIDdirect(SuperBlock *pSb, Inode *pInode, Data *pData)
{
	long long dbNum;
	int flag = 1;

	if(pInode->fileSize != 0)		{ return 1 ; }

	flag = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);

	if(flag == 2)	{ return flag ; }

	pInode->direct = (short) dbNum;
	//pInode->fileSize += 128;

	return flag;
}

int allocdbinIDsindirect(SuperBlock *pSb, Inode *pInode, Data *pData)
{
	int flag = 0;
	long long dbNum;
	bool Doprepare = 0;
	if(pInode->fileSize/128 < 1)	{ return 3; }	
	if(pInode->fileSize/128 >= 103/*104*/)	{ return 1; }
	if(pInode->fileSize/128 == 1)
	{
		//printf("beforedbNum:%lld\n",dbNum);
		flag = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);
		//printf("afterdbNum:%lld\n",dbNum);

		if(flag == 2)	{ return flag; }
		pInode->sindirect = (short) dbNum;
		//pInode->fileSize += 128 ;
	}

	//printf("beforedbNum:%lld\n",dbNum);
	flag = preparedataBlock(pData,pSb->usabledataBlock, &dbNum);
	//printf("afterdbNum:%lld\n",dbNum);

	if(flag == 2)	{ return flag; }

	allocbitDbtoDr(pData + pInode->sindirect, dbNum);

/*	for(int i=0;i<16;i++)
		printBit(pData[pInode->sindirect].dataArr[i]);
*/
	//pInode->fileSize += 128 ;

	return flag;
}


int allocdbinIDdlindirect(SuperBlock *pSb, Inode *pInode, Data *pData)
{
	int flag = 0;
	long long dbNum;
	int directIndex;
	static int k = 0;
	int abc;

	if(pInode->fileSize/128 < 103)		{ return 3 ; }
	if(pInode->fileSize/128 >= 10507/*10611*/)	{ return 1 ; }

	if(pInode->fileSize/128 == 103/*104*/)		
	{
		flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
		if(flag == 2)	{ return flag; }
		pInode->dlindirect = (short) dbNum;
		//pInode->fileSize += 128 ;
	}

	if(((pInode->fileSize)/128 - 103/*105*/)%102/*103*/ == 0 )
	{
		flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
		if(flag == 2)	{ return flag; }
		allocbitDrtoSin(pData + pInode->dlindirect, dbNum);
		//pInode->fileSize += 128;
		flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
		if(flag == 2)	{ return flag; }

		directIndex = readbitArr(pData + pInode->dlindirect, k);
		allocbitDbtoDr(pData+directIndex , dbNum);

		k++;
	}

	if(((pInode->fileSize)/128-103/*105*/)%102 != 0)
	{
		flag = preparedataBlock(pData, pSb->usabledataBlock, &dbNum);
		if(flag == 2)	{ return flag; }

		directIndex = readbitArr(pData + pInode->dlindirect, k);
		allocbitDbtoDr(pData+directIndex , dbNum);
		//pInode->fileSize += 128;
	}

	return 0;
}

long long readIDDire(Inode *pInode)
{
	long long dbNum = pInode->direct;

	return dbNum;
}

long long readDireinSin(Data *pData, long long SinD,int k)
{
	long long dbNum;

	dbNum = readbitArr(pData+SinD, k);
}

long long readIDSinD(Inode *pInode, Data *pData, int k)
{
	long long SinD = pInode -> sindirect;
	long long dbNum ;

	dbNum = readDireinSin(pData, SinD,k - 1);

	return dbNum;
}

long long readSinDinDlin(Data *pData, long long Dlin,int k)
{
	long long SinD = readbitArr(pData+Dlin, k/103 + 1);
	long long dbNum;

	dbNum = readDireinSin(pData, SinD, k%103);

	return dbNum;

}

long long readIDDlin(Inode *pInode, Data *pData, int k)
{
	long long Dlin = pInode->dlindirect;
	long long dbNum;

	dbNum = readSinDinDlin(pData, Dlin, k - 105 -1);

	return dbNum;
}

long long readDbNuminID(Inode *pInode ,Data *pData, int allocIndex)
{
	long long dbNum;

	if(allocIndex == 1)
		dbNum = readIDDire(pInode);
	if((allocIndex >= 2) && (allocIndex <=104))
		//dbNum=readIDSinD(pInode, pData, pInode->fileSize/128);
		dbNum=readIDSinD(pInode, pData, allocIndex);
	if((allocIndex >= 105) && (allocIndex <=10611))
		//dbNum=readIDDlin(pInode, pData, pInode->fileSize/128);
		dbNum=readIDDlin(pInode, pData, allocIndex);

	return dbNum;
}

DNode *createDBNode(Data *pData)
{
	DNode *pDNode = (DNode *)malloc(sizeof(struct tagDNode));

	pDNode->pNext = NULL;
	pDNode->pData = pData ;

	return pDNode;
}

void insertDBNode(DNode **pTail, DNode *pDNode)
{
	pDNode->pNext = (*pTail)->pNext;
	(*pTail)->pNext = pDNode;

	*pTail = pDNode;
}

void deleteDBNode(DNode *pPrev, DNode **pTail)
{
	DNode *pDNode = *pTail ;

	pPrev->pNext = pDNode->pNext;
	(*pTail) = pPrev;

	free(pDNode);
}

DNode *findDBPrevNode(DNode *pHead, DNode *pFind)
{
	DNode *pDNode = pHead;

	for(pDNode;pDNode!=NULL;pDNode=pDNode->pNext)
		if(pDNode->pNext == pFind)	{	return pDNode ; }
}

TNode *createTNode(long long idNum)
{
	TNode *pTNode = (TNode *) malloc(sizeof(TNode));

	pTNode->pPrev = NULL;
	pTNode->idNum = idNum;
	pTNode->pNext = NULL;

	return pTNode;
}

void insertleftTNode(TNode *pMain, TNode *pSub)
{
	if(pMain->pPrev != NULL)
		pSub->pNext = pMain->pPrev;

	pMain->pPrev = pSub;
}
/*
void insertrightTNode(TNode *pMain, TNode *pSub)
{
	if(pMain->pNext != NULL)		{  return ; }

	pMain->pNext = pSub;
}
*/
void deleteTNodeNext(TNode *pPrev, TNode *pRemo)
{
	pPrev->pNext = pRemo->pNext ;

	free(pRemo);
}

void deleteTNodePrev(TNode *pPrev, TNode *pRemo)
{
	pPrev->pPrev = pRemo->pNext ;

	free(pRemo);
}

void saveNuminTNode(TNode *pTNode, long long idNum)
{
	int i=0;

		pTNode->idNum = idNum;

}

long long getNumfromTNode(TNode *pTNode)
{
	return pTNode->idNum;
}

void deleteTNode(TNode *pPrev, TNode *pRemo)
{
	if(pPrev->pPrev == pRemo)
		deleteTNodePrev(pPrev, pRemo);

	if(pPrev->pNext == pRemo)
		deleteTNodeNext(pPrev, pRemo);
}

SNode *createSNode(void)
{
	SNode *pSNode = (SNode *) malloc(sizeof(SNode));

	*pSNode = (struct tagSNode) {.data={0}};
	pSNode->pNext = NULL;

	return pSNode;
}

void pushSNodetoStack(SNode *pHead, SNode *pSNode)
{
	pSNode->pNext = pHead->pNext;
	pHead->pNext  = pSNode; 
}

void savedatatoSNode(SNode *pSNode, char c, int index)
{
	pSNode->data[index] = c;
}	

void deleteSNode(SNode *pHead, SNode *pRemo)
{
//	printf("pHead->pNext:%s\n",pHead->pNext->data);
//	printf("pRemo:%s\n",pRemo->data);
	pHead->pNext = pRemo->pNext;
//	printf("pHead->pNext:%p\n",pHead->pNext->data);

	free(pRemo);
}
