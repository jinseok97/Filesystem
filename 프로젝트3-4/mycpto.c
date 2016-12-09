



		//main.c 에 추가//
		//char tmpname[10]; 선언 필요
		else if(strcmp("mycpto", SHAf.pNext -> data) == 0)
		{
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			if(pSNode == NULL)		
				flag = 1;
			else 
			{
				if(strcmp(pSNode -> data,"/") == 0)
				{
					for(; pSNode -> pNext != NULL;)
					{
						absolutePath(inode, dataBlock, &Root, &tPwd, &tUwd,pSNode);
						if(pSNode -> pNext != NULL)
						{
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
				else 
				{
					for(; pSNode -> pNext != NULL;)
					{
						relativePath(inode, &Root, &tPwd, &tUwd, dataBlock, pSNode);
						if(pSNode -> pNext != NULL)
						{
							strncpy(tmpname, SHAf.pNext -> data, 4);
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
			}
			if(flag == 1)
				printf("인자를 입력하세요\n");
			else
			{
				printf("1 : %s\n", tmpname);
				printf("2 : %s\n",pSNode->data);
				f_mycpto(tmpname, pSNode -> data, &spblock,inode,dataBlock,tPwd);
			}

		}





//f_mycpto 함수 : 살짝 오류가 있음(밀려서 출력됨)
void f_mycpto(char *name1, char *name2, SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{
	printf("'mycpto' call\n");

	TNode *pTNode = NULL;
	DNode Head = {NULL, NULL};
	DNode *pTail = &Head;
	DNode *pDNode = NULL;
	int start, end;

	long long idNum = pwd -> idNum;
	int tmpfileSize = ind[idNum].fileSize;
	int dbNum, tmpidNum, indNum;

	for(int i = 0; i <= (ind[idNum].fileSize) / 128; i++)
	{
		dbNum = readDbNuminID(ind+pwd -> idNum, pDB, i);
		pDNode = createDBNode(pDB + dbNum);
		insertDBNode(&pTail, pDNode);
	}

	for(DNode *pIndex = Head.pNext; pIndex != NULL; pIndex = pIndex -> pNext)
	{
		if(pIndex == Head.pNext)
			start = 2;
		else
			start = 1;

		end = (tmpfileSize > 128) ? 128 / 8 : (tmpfileSize) / 8;
		if(tmpfileSize >= 128)
			tmpfileSize -= 128;

		for(int i = start; i < end; i++)
		{
			if(strncmp(pIndex -> pData -> directory.name[i], name1, 4))
			{
				printf("'%s' is not found in myfs.\n", name1);
				return ;
			}
			else	//"name1"의 아이노드 저장
				tmpidNum = pIndex -> pData -> directory.idNum[i];
		}
	}
	
	FILE *ofp;
	ofp = fopen(name2, "w");

	for(int i = 0; i < (ind[tmpidNum].fileSize / 128); i++)
	{
		dbNum = readDbNuminID(ind + tmpidNum, pDB, i);
		pDNode = createDBNode(pDB + dbNum);
		insertDBNode(&pTail, pDNode);
	}

	for(DNode *pIndex = Head.pNext; pIndex != NULL; pIndex = pIndex -> pNext)
		for(int i = 0; i < 128; i++)
		{
			putchar(pIndex -> pData -> file[i]);	//확인
			putc(pIndex -> pData -> file[i], ofp);
		}

}



