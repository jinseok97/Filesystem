#include "filesystem.h"

void procArgument(SNode *pHead)
{
	int i=0;
	char c;
	SNode *pSNode;

	while((c=getchar()) != '\n')
	{
		if(i==0 && c=='/')
		{
			pSNode=createSNode();
			savedatatoSNode(pSNode, c, i );
			pushSNodetoStack(pHead, pSNode);
			continue;
		}
		if(i==0)
			pSNode=createSNode();
		if(c=='/' || c==' ')	
		{
			pushSNodetoStack(pHead, pSNode);
			i=0;
			continue;
		}

		savedatatoSNode(pSNode, c, i );
		i++;
	}

	pushSNodetoStack(pHead, pSNode);
}

void upsidedownStack(SNode *SHBf, SNode *SHAf)
{
	SNode *pSNode = NULL;

	for(SNode *pSNodeI =SHBf->pNext;pSNodeI!= NULL;pSNodeI=SHBf->pNext)
	{
		pSNode = createSNode();
		for(int i=0;i<20;i++)
			pSNode->data[i] = pSNodeI->data[i];
		deleteSNode(SHBf,pSNodeI);
		pushSNodetoStack(SHAf, pSNode);
	}


}

void initDirectory(Inode *pInode, Data *pData, long long idNum, TNode *Pwd, char *name)
{
	int Dire = (int) pInode[idNum].direct;

	pData[Dire].directory.idNum[0]= idNum;
	pData[Dire].directory = (struct tagDirectory){{[1]=".."},{[1]=Pwd->idNum}};
}

int confirminfoPwd(Inode *pInode, Data *pData,TNode *Pwd)
{
	int idNum = Pwd->idNum;
	int dbNum = pInode->direct;

	for(int i=0;i<16;i++)
		if(pData[dbNum].directory.idNum[i] < 0 || pData[dbNum].directory.idNum[i] > 255)
			return i;
}

int addinfoPwd(Inode *pInode, Data *pData, TNode *Pwd, long long *idNum)
{
	int idpNum = Pwd->idNum;
	int dbNum = pInode[idpNum].direct;
	int index;

	index = confirminfoPwd(pInode, pData, Pwd);

	pData[dbNum].directory.idNum[index] = (*idNum);
	*idNum = idpNum ;

	return index;
}

void preorderTraverse(TNode *pTNode, Inode *pInode, Data *pData)
{
	if(pTNode == NULL)	{ return ; }

	long long idNum = pTNode->idNum;
	long long dbNum = pInode[idNum].direct;
	printf("%s\n",pData[dbNum].directory.name);
	preorderTraverse(pTNode->pPrev, pInode, pData);
	preorderTraverse(pTNode->pNext, pInode, pData);
}	

void makeDirectory(SuperBlock *pSb, Inode *pInode, Data *pData, TNode *Pwd, char *name)
{
	long long idNum;
	int dbNum;
	int arrNum;
	int bitNum;
	int index;

	TNode *pTNode = NULL;

	DNode Head = {NULL, NULL}; 
	DNode *pTail = &Head;
	DNode *pDNode = NULL; 

	idNum = Pwd->idNum;
	dbNum = pInode[idNum].direct ;
	for(int i=0;i<16;i++)
		if(strcmp(pData[dbNum].directory.name[i],name) == 0)
		{
			printf("이미 같은 디렉터리가 존재 합니다.\n");
			return;
		}

	findusableInode(pSb->usableInode, &arrNum, &bitNum);
	markInode(pSb->usableInode, arrNum, bitNum);
	idNum = arrNum*64 + bitNum - 1;
	allocdbinIDdirect(pSb, pInode + idNum, pData);

	pTNode = createTNode(idNum);
	insertleftTNode(Pwd, pTNode);

	initDirectory(pInode, pData, idNum, Pwd, name);
	strcpy(pData[pInode[idNum].direct].directory.name[0], name);

	index = addinfoPwd(pInode,pData, Pwd, &idNum);

	strcpy(pData[pInode[idNum].direct].directory.name[index], name);

}

void removeDirectory(SuperBlock *pSb,Inode *pInode, Data *pData,TNode *Pwd, char *name)
{
	long long idNum = Pwd->idNum;
	long long dbNum = pInode[idNum].direct;
	int i;
	TNode *pTNode = NULL ;
	TNode *pPrev = NULL;
	for(i=0;i<16;i++)
		if(strcmp(pData[dbNum].directory.name[i],name) == 0)
			break;
	if(i==16)	
	{
		printf("존재 하지 않는 디렉터리입니다.\n");
		return ;
	}
	idNum = pData[dbNum].directory.idNum[i];
	unmarkInode(pSb->usableInode, idNum/64, (idNum + 1) %64);
	for(int i=0;i<16;i++)
		printBit(pSb->usableInode[i]);

	//dbNum = pInode[PidNum].direct;
	pData[dbNum].directory.idNum[i]=-1;
	for(int j=0;j<4;j++)
		pData[dbNum].directory.name[i][j] = 0;


	pPrev = Pwd;
	for(pTNode=Pwd->pPrev;pTNode != NULL;pTNode=pTNode->pNext)
	{
		printf("%d %d\n",pTNode->idNum,idNum);
		if(pTNode->idNum == idNum)
		{
			printf("%p %p %p\n",pTNode, Pwd->pPrev, Pwd->pPrev->pNext);
			if(pTNode == Pwd->pPrev)
				deleteTNodePrev(pPrev, pTNode);
			else
				deleteTNodeNext(pPrev, pTNode);
			break;
		}
		pPrev = pTNode;
	}

}

void preorderTraverseP(TNode *pTNode, Inode *pInode, Data *pData, long long idNumP, TNode **pUwd)
{
	if(pTNode == NULL)	{ return ; }

	long long idNum = pTNode->idNum;


	if(idNum == idNumP)
		(*pUwd) = pTNode;
	preorderTraverseP(pTNode->pPrev, pInode, pData, idNumP, pUwd);
	preorderTraverseP(pTNode->pNext, pInode, pData, idNumP, pUwd);
}	

void storePwdName(Inode *pInode, Data *pData, TNode *Pwd, SNode *pHead)
{
	SNode *pStack = NULL;

	long long idNum = Pwd->idNum;
	long long dbNum = pInode[idNum].direct;


	for(int i=0;i<16;i++)
		if(pData[dbNum].directory.idNum[i] == idNum)
		{
			pStack=createSNode();
			strcpy(pStack->data,pData[dbNum].directory.name[i]);
			pushSNodetoStack(pHead, pStack);
			return ;
		}
}

bool findUpwd(Inode *pInode,Data *pData,TNode *pRoot,TNode **pUwd, SNode *pHead)
{
	SNode *pStack = NULL ;

	long long idNum = (*pUwd)->idNum;
	long long dbNum = pInode[idNum].direct;

	int i=0;

	for(i=0;i<16;i++)
	{
		if(strcmp(pData[dbNum].directory.name[i],"..")==0)
		{
			idNum = pData[dbNum].directory.idNum[i];
			break;
		}
	}
	if(i==16)		{  return 1; }
	preorderTraverseP(pRoot, pInode, pData, idNum, pUwd);
	return 0;
}

void printPwd(Inode *pInode, Data *pData,TNode *pRoot, TNode *Pwd, TNode **pUwd)
{
	SNode Head = {{0},NULL};
	SNode *pStack = NULL;
	bool flag = 0;

	storePwdName(pInode, pData, Pwd, &Head);

	while(1)
	{
		Pwd = (*pUwd);
		flag=findUpwd(pInode, pData, pRoot, pUwd, &Head);
		if(flag == 1 )		
			break;
		storePwdName(pInode, pData, Pwd, &Head);
	}

	for(pStack = Head.pNext; pStack!=NULL;pStack=pStack->pNext)
		printf("%s\n",pStack->data);
}

void absolutePath(Inode *pInode,Data *pData,TNode *pRoot, TNode **tPwd, SNode *pSNode)
{
	long long idNum = (*tPwd==NULL) ? pRoot->idNum : (*tPwd)->idNum;
	long long dbNum = pInode[idNum].direct;

	int i;

	for(i=0;i<16;i++)
	{
		if(strcmp(pData[dbNum].directory.name[i],pSNode->data)==0)
		{
			idNum = pData[dbNum].directory.idNum[i];
			break;
		}
	}
	if(i==16)		{  return ; }
	preorderTraverseP(pRoot, pInode, pData, idNum, tPwd);

}

void relativePath(Inode *pInode, TNode *pRoot, TNode **tPwd, TNode **tUwd, Data *pData, SNode *pSNode)
{

	long long idNum = (*tPwd)->idNum;
	long long dbNum = pInode[idNum].direct;

	int i;

	if(strcmp("..",pSNode->data)==0)
	{
		(*tPwd) = (*tUwd); 
		idNum = (*tPwd)->idNum;
	}
	else
	{
		for(i=0;i<16;i++)
		{
			if(strcmp(pData[dbNum].directory.name[i],pSNode->data)==0)
			{
				idNum = pData[dbNum].directory.idNum[i];
				break;
			}	
		}
		if(i==16)		{  return ; }
		preorderTraverseP(pRoot, pInode, pData, idNum, tPwd);

	}	


	dbNum = pInode[idNum].direct;

	for(i=0;i<16;i++)
	{
		if(strcmp(pData[dbNum].directory.name[i],"..")==0)
		{
			idNum = pData[dbNum].directory.idNum[i];
			break;
		}	
	}
	if(i==16)		{  return ; }
	preorderTraverseP(pRoot, pInode, pData, idNum, tUwd);

	return ;
}

void mycd(TNode **pPwd,TNode **pUwd,TNode *tPwd, TNode *tUwd)
{
	(*pPwd) = tPwd;
	(*pUwd) = tUwd;
}

int main(void)
{
	SuperBlock spblock ;

	Inode inode[512];
	inode[0].direct = 0;

	Data dataBlock[1024] ;

	initDataBlock(dataBlock);

	dataBlock[0].directory.name[0][0] = '/';
	dataBlock[0].directory.name[0][1] = 0;
	dataBlock[0].directory.idNum[0] = 0;

	TNode Root ={NULL,0,NULL} ;
	TNode *pTNode = NULL;
	TNode *pwd = &Root;
	TNode *uPwd = &Root;
	TNode *tPwd = pwd;
	TNode *tUwd = uPwd;

	SNode SHAf = {{0},NULL};
	SNode SHBf = {{0},NULL};
	SNode *pSNode = NULL;

	int idNum;
	int dbNum;
	initsuperBlock(&spblock);
	markInode(spblock.usableInode, 0, 1);
	markdataBlock(spblock.usabledataBlock, 0, 1);


	while(1)
	{
		printf("[%s]$ ",dataBlock[0].directory.name[0]);
		procArgument(&SHBf);
		upsidedownStack(&SHBf,&SHAf);

		if(strcmp("mymkdir",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ;)
			{
				deleteSNode(&SHAf, pSNode);
				pSNode = SHAf.pNext;
				if(pSNode == NULL)	{ break ; }
				makeDirectory(&spblock, inode, dataBlock, pwd, pSNode->data);
			}
		}
		else if(strcmp("myrmdir",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ; )
			{
				deleteSNode(&SHAf, pSNode);
				pSNode = SHAf.pNext;
				if(pSNode == NULL)	{ break ; }
				removeDirectory(&spblock, inode, dataBlock, pwd, pSNode->data);
			}
		}
		else if(strcmp("mytree",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ;)
			{
				deleteSNode(&SHAf, pSNode);
				pSNode = SHAf.pNext;
				preorderTraverse(pwd, inode, dataBlock);
			}
		}
		else if(strcmp("pwd",SHAf.pNext->data)==0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ; )
			{
				deleteSNode(&SHAf, pSNode);
				pSNode = SHAf.pNext;
				printPwd(inode, dataBlock, &Root, pwd, &uPwd);
			}
		}
		else if(strcmp("cd",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext;pSNode !=NULL;pSNode=pSNode->pNext)
				printf("pSNode->data:%s\n",pSNode->data);
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			if(pSNode == NULL)		
			{
				tPwd = &Root;
				tUwd = &Root;
			}
			else 
			{
				if(strcmp(pSNode->data,"/") == 0)
				{


					for(;pSNode != NULL;)
					{
						absolutePath(inode, dataBlock, &Root, &tPwd, pSNode);
						deleteSNode(&SHAf, pSNode);
						pSNode = SHAf.pNext;
					}
				}
				else 
				{
					for(;pSNode!=NULL;)
					{
						relativePath(inode,&Root,&tPwd,&tUwd,dataBlock,pSNode);
						deleteSNode(&SHAf, pSNode);
						pSNode = SHAf.pNext;
					}
				}
			}
			mycd(&pwd, &uPwd, tPwd,tUwd);
			idNum = pwd->idNum;
			dbNum = inode[idNum].direct;
			printf("pwd:%s\n",dataBlock[dbNum].directory.name[0]);
			idNum = uPwd->idNum;
			dbNum = inode[idNum].direct;
			printf("uPwd:%s\n",dataBlock[dbNum].directory.name[0]);
		}
		else if(strcmp("byebye",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ;pSNode = pSNode->pNext)
				deleteSNode(&SHAf, pSNode);
			return 0;
		}

		for(pSNode =SHAf.pNext; pSNode != NULL ;pSNode = pSNode->pNext)
			deleteSNode(&SHAf, pSNode);

		for(int i=0;i<4;i++)
		{
			idNum = Root.idNum;
			dbNum = inode[idNum].direct;
			printf("[]name:%s, idNum:%d\n",dataBlock[dbNum].directory.name[i], dataBlock[dbNum].directory.idNum[i]);
		}
		putchar('\n');
	}

	for(TNode *pIndex = Root.pPrev;pIndex != NULL; pIndex=pIndex->pNext)
		printf("pIndex:%p pIndex->pNext:%p\n",pIndex,pIndex->pNext);
	return 0;
}
