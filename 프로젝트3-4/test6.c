#include "filesystem.h"

typedef struct tagTNode TNode; 

struct tagTNode 
{
	TNode *pPrev;
	char data[4];
	TNode *pNext;
};

typedef struct tagSNode SNode;

struct tagSNode
{
	char data[4];
	SNode *pNext;
};

TNode *createTNode(long long idNum)
{
	TNode *pTNode = (TNode *) malloc(sizeof(TNode));

	pTNode->pPrev = NULL;
	*pTNode = (struct tagTNode) {.data={0}};
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

void saveNuminTNode(TNode *pTNode, char *pStr)
{
	int i=0;

	for(i=0;i<4;i++)
		pTNode->data[i] = pStr[i];

}

int getNumfromTNode(TNode *pTNode)
{
	return atoi(pTNode->data);
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
	pHead->pNext = pRemo->pNext;

	free(pRemo);
}

void initDirectory(Inode *pInode, Data *pData, long long idNum, TNode *Pwd, char *name)
{
	long long Dire = pInode[idNum].direct;

	pData[Dire].directory = (struct tagDirectory){{[0]=idNum}};
	strcpy(pData[Dire].directory.name, name);
	pData[Dire].directory = (struct tagDirectory){{[1]=".."},{[0]=atoi(Pwd->data)}};
}

void makeDirectory(SuperBlock *pSb, Inode *pInode, Data *pData, TNode *Pwd)
{
	long long idNum;
	TNode *pTNode = NULL;

	findusableInode(pSb->usableInode, &arrNum, &bitNum);
	markInode(pSb->usableInode, arrNum, bitNum);
	idNum = arrNum*64 + bitNum - 1;
	allocdbinIDdirect(pSb, pInode, pData);

	pTNode = createTNode(idNum);

	insertleftTNode(Pwd, pTNode);
}

int main(void)
{
	SuperBlock spblock ;

	Inode inode[512];

	Data dataBlock[1024];

	TNode TRoot = {NULL,{0},NULL};
	TNode *pTNode = NULL;
	TNode *pwd = &TRoot;
	TNode *uPwd = &TRoot;
/*
	SNode SHBf = {{0}, NULL}; 
	SNode SHAf = {{0}, NULL};
	SNode *pSNode = NULL;
*/
	int bitNum;
	int arrNum;

	findusableInode(spblock.usableInode,&arrNum,&bitNum);
	markInode(spblock.usableInode, arrNum,bitNum);
	
	findusabledataBlock(spblock.usabledataBlock, &arrNum,&bitNum);
	markInode(spblock.usabledataBlock, arrNum,bitNum);

	inode[0].direct = 0;

	char c = 0;
	int i = 0;
/*
	while((c=getchar()) != '\n')
	{
		if(c == '/')
		{
			pSNode = createSNode();
			pushSNodetoStack(&SHBf, pSNode);
			i=0;
			continue;
		}

		savedatatoSNode(pSNode, c, i );

		i++;
	}
	*/
	/*
	for(SNode *pSNodeI =SHBf.pNext;pSNodeI!= NULL;pSNodeI=pSNodeI->pNext)
	{
		for(int i=0;i<4;i++)
			printf("%c",pSNodeI->data[i]);
		putchar('\n');
	}
	*/
	/*
	for(SNode *pSNodeI =SHBf.pNext;pSNodeI!= NULL;pSNodeI=SHBf.pNext)
	{
		pSNode = createSNode();
		for(int i=0;i<4;i++)
			pSNode->data[i] = pSNodeI->data[i];
		deleteSNode(&SHBf,pSNodeI);
		pushSNodetoStack(&SHAf, pSNode);
	}


	for(SNode *pSNodeI =SHAf.pNext;pSNodeI!= NULL;pSNodeI=pSNodeI->pNext)
	{
		printf("pSNode:%p, pSNode->pNext:%p\n",pSNodeI,pSNodeI->pNext);
		for(int i=0;i<4;i++)
			printf("%c",pSNodeI->data[i]);
		putchar('\n');
	}	
	*/
	//pTNode = createTNode();
	//insertleftTNode(&TRoot, pTNode);

	//saveNuminTNode(TRoot.pPrev, "123");

	//getNumfromTNode(TRoot.pPrev);
	/*
	   pTNode = createTNode();
	   insertrightTNode(TRoot.pPrev, pTNode);	
	   pTNode = createTNode();
	   insertrightTNode(TRoot.pPrev->pNext, pTNode);

	   printf("TRoot.pPrev:%p\n",TRoot.pPrev);
	   printf("TRoot.pPrev->pNext:%p\n",TRoot.pPrev->pNext);
	   printf("TRoot.pPrev->pNext->pNext:%p\n",TRoot.pPrev->pNext->pNext);

	//deleteTNode(TRoot.pPrev->pNext,TRoot.pPrev->pNext->pNext);
	deleteTNode(TRoot.pPrev,TRoot.pPrev->pNext);
	putchar('\n');

	printf("TRoot.pPrev:%p\n",TRoot.pPrev);
	printf("TRoot.pPrev->pNext:%p\n",TRoot.pPrev->pNext);
	printf("TRoot.pPrev->pNext->pNext:%p\n",TRoot.pPrev->pNext->pNext);
	*/


	return 0;
}
