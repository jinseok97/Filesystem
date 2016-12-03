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

	pInode[idNum].fileType = 1;
	pInode[idNum].time = time(NULL);
	pInode[idNum].fileSize = 16; 
	//pData[Dire].directory.idNum[0]= idNum;
	pData[Dire].directory = (struct tagDirectory){{[1]=".."},{[1]=Pwd->idNum}};

}

int confirminfoPwd(Inode *pInode, Data *pData,TNode *Pwd)
{
	int idNum = Pwd->idNum;
	int dbNum = pInode->direct;

	int arrNum=(pInode[idNum].fileSize/8);
	/*
	   for(int i=0;i<16;i++)
	   {
	   if(idNum == 0&&(pData[dbNum].directory.idNum[i] < 0 || pData[dbNum].directory.idNum[i] > 255))
	   return i;
	   if(pData[dbNum].directory.idNum[i] <= 0 || pData[dbNum].directory.idNum[i] > 255))
	   }
	   */
	return arrNum;
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

void preorderTraverse(TNode *pTNode, Inode *pInode, Data *pData, int i)
{
	if(pTNode == NULL)	{ return ; }

	long long idNum = pTNode->idNum;
	long long dbNum = pInode[idNum].direct;

	if(i==0)
		printf("%s\n",pData[dbNum].directory.name[0]);
	else
	{
		for(int j=0;j<i;j++)
			printf("--");
		printf("*%s\n",pData[dbNum].directory.name[0]);
	}
	preorderTraverse(pTNode->pPrev, pInode, pData,i+1);
	preorderTraverse(pTNode->pNext, pInode, pData,i);
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

	idNum = Pwd->idNum;									//현재 디렉터리의 Inode
	dbNum = pInode[idNum].direct ;
	for(int i=2;i<16;i++)
		if(strcmp(pData[dbNum].directory.name[i],name) == 0)
		{
			printf("이미 같은 디렉터리가 존재 합니다.\n");
			return;
		}

	findusableInode(pSb->usableInode, &arrNum, &bitNum);
	markInode(pSb->usableInode, arrNum, bitNum);
	idNum = arrNum*64 + bitNum - 1;										//새로 할당된 디렉터리의 Inode
	allocdbinIDdirect(pSb, pInode + idNum, pData);						//Inode에 데이터 블럭 할당 

	pTNode = createTNode(idNum);										//트리구조에 새로 만든 디렉터리 추가
	insertleftTNode(Pwd, pTNode);

	initDirectory(pInode, pData, idNum, Pwd, name);						//현재 디렉터리 초기화

	strcpy(pData[pInode[idNum].direct].directory.name[0], name);
	pData[pInode[idNum].direct].directory.idNum[0] = idNum;

	index = addinfoPwd(pInode,pData, Pwd, &idNum);						//현재 디렉터리 정보 갱신
	strcpy(pData[pInode[idNum].direct].directory.name[index], name);
	pInode[Pwd->idNum].fileSize += 8;
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
	pInode[idNum].fileSize -= 8;
	idNum = pData[dbNum].directory.idNum[i];					//지우고자 하는 디렉터리의 아이노드 번호
	unmarkInode(pSb->usableInode, idNum/64, (idNum + 1) %64);	//지우고자 하는 디렉터리에 할당된 데이터블럭 할당 해제
	//	for(int i=0;i<16;i++)
	//		printBit(pSb->usableInode[i]);

	//dbNum = pInode[PidNum].direct;
	pData[dbNum].directory.idNum[i]=-1;							//지우고자 하는 디렉터리에 할당된 데이터 블럭 초기화
	for(int j=0;j<4;j++)
		pData[dbNum].directory.name[i][j] = 0;


	pPrev = Pwd;
	for(pTNode=Pwd->pPrev;pTNode != NULL;pTNode=pTNode->pNext)
	{
		if(pTNode->idNum == idNum)
		{
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
			printf("name:%s\n",pData[dbNum].directory.name[i]);
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
	if(strcmp(pData[dbNum].directory.name[0],"/")==0)	{ return 1; }

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

	int idNum = Pwd->idNum;
	int dbNum = pInode[idNum].direct;

	if(strcmp(pData[dbNum].directory.name[0],"/") != 0)
		storePwdName(pInode, pData, Pwd, &Head);

	while(1)
	{
		Pwd = (*pUwd);
		flag=findUpwd(pInode, pData, pRoot, pUwd, &Head);
		if(flag == 1 )		{break;}
		storePwdName(pInode, pData, Pwd, &Head);
	}

	if(Head.pNext == NULL)
		printf("/\n");
	for(pStack = Head.pNext; pStack!=NULL;pStack=pStack->pNext)
	{
		printf("/%s",pStack->data);
		if(pStack->pNext == NULL)
			putchar('\n');
	}
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

void myls(Inode *pInode, Data *pData, TNode *tPwd, int flag )
{
	long long idNum = tPwd->idNum;
	long long dbNum = pInode[idNum].direct;
	long long tidNum = idNum;
	int i=0;
	struct tm *time =localtime(&(pInode[idNum].time));


	//	for(int i=0; i<16; i++)
	//		printf("%d %s\n",pData[dbNum].directory.idNum[i],pData[dbNum].directory.name[i]);
	if(flag == 0)												//myls 인자 없는 경우
		for(i=0;i<(pInode[idNum].fileSize/8);i++)
		{
			if(i==0)
				printf(".\n");  								//현재 디렉터리
			else
				printf("%s\n",pData[dbNum].directory.name[i]);	
		}
	if(flag == 1)												//myls -i 아이노드 번호 출력
		for(i=0;i<(pInode[idNum].fileSize/8);i++)
		{
			if(i==0)
				printf("%d .\n",pData[dbNum].directory.idNum[i]);	//현재 디렉터리
			else 
				printf("%d %s\n",pData[dbNum].directory.idNum[i],pData[dbNum].directory.name[i]);
		}
	if(flag == 2)												//myls -l 상세 정보 출력
		for(i=0;i<(pInode[idNum].fileSize/8);i++)
		{
			tidNum = pData[dbNum].directory.idNum[i];
			time = localtime(&(pInode[tidNum].time));

			if(pInode[idNum].fileType == 1)
				printf("d %4d ",0);
			else 
				printf("- %4d ",pInode[idNum].fileSize);

			printf("%d/%d/%d ",time->tm_year + 1900, time->tm_mon+1, time->tm_mday);
			printf("%d:%d:%d ",time->tm_hour, time->tm_min, time->tm_sec);

			if(i==0)
				printf(".\n");
			else
				printf("%s\n",pData[dbNum].directory.name[i]);
		}

}

int main(void)
{
	SuperBlock spblock ;

	Inode inode[512];
	inode[0].direct = 0;
	inode[0].fileType = 1;
	inode[0].fileSize = 16;
	inode[0].time = time(NULL);

	Data dataBlock[1024] ;

	initDataBlock(dataBlock);

	dataBlock[0].directory.name[0][0] = '/';
	dataBlock[0].directory.name[0][1] = 0;
	dataBlock[0].directory.name[1][0] = '.';
	dataBlock[0].directory.name[1][1] = '.';
	dataBlock[0].directory.name[1][2] = 0;
	dataBlock[0].directory.idNum[0] = 0;
	dataBlock[0].directory.idNum[1] = 0;

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
	int flag = 0;

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
			printf("Root.fileSize:%d\n",inode[0].fileSize);
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
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;

			if( pSNode != NULL && strcmp(pSNode->data,"/") == 0)
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


			if(tPwd->idNum == Root.idNum)
				preorderTraverse(&Root, inode, dataBlock,0);
			else 
				preorderTraverse(tPwd,inode,dataBlock,0);
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
		}
		else if(strcmp("myls",SHAf.pNext->data) == 0)
		{
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			if(pSNode != NULL &&strcmp(pSNode->data,"-i") == 0)
			{
				flag = 1;
				deleteSNode(&SHAf, SHAf.pNext);
				pSNode = SHAf.pNext;
			}
			else if(pSNode != NULL && strcmp(pSNode->data,"-l")==0)
			{
				flag = 2;
				deleteSNode(&SHAf, SHAf.pNext);
				pSNode = SHAf.pNext;
			}

			if(pSNode == NULL)		
			{
				tPwd = pwd;
				tUwd = uPwd;
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
			myls(inode,dataBlock, tPwd, flag);
			flag = 0;


		}
		else if(strcmp("byebye",SHAf.pNext->data) == 0)
		{
			for(pSNode =SHAf.pNext; pSNode != NULL ;pSNode = pSNode->pNext)
				deleteSNode(&SHAf, pSNode);
			break;
		}

		for(pSNode =SHAf.pNext; pSNode != NULL ;pSNode = pSNode->pNext)
			deleteSNode(&SHAf, pSNode);
		/*

		   for(int i=0;i<4;i++)
		   {
		   idNum = Root.idNum;
		   dbNum = inode[idNum].direct;
		   printf("[]name:%s, idNum:%d\n",dataBlock[dbNum].directory.name[i], dataBlock[dbNum].directory.idNum[i]);
		   }
		   putchar('\n');
		   */
	}

	//for(TNode *pIndex = Root.pPrev;pIndex != NULL; pIndex=pIndex->pNext)
	//	printf("pIndex:%p pIndex->pNext:%p\n",pIndex,pIndex->pNext);
	printf("Root.fileSize:%d\n",inode[0].fileSize);
	return 0;
}
