

		//main.c에 추가//
		//char tmpname[10]; 선언 필요
		else if(strcmp("mycpfrom", SHAf.pNext -> data) == 0)
		{
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			if(pSNode == NULL)		
				flag = 1;
			else 
			{
				if(strcmp(pSNode->data,"/") == 0)
				{
					for(;pSNode->pNext != NULL;)
					{
						absolutePath(inode, dataBlock, &Root, &tPwd, &tUwd,pSNode);
						if(pSNode->pNext != NULL)
						{
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
				else 
				{
					for(;pSNode->pNext!=NULL;)
					{
						relativePath(inode,&Root,&tPwd,&tUwd,dataBlock,pSNode);
						if(pSNode->pNext != NULL)
						{
							strncpy(tmpname, pSNode -> data, 4);
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
				tmpname[0]='\0';
			}
			if(flag == 1)
				printf("인자를 입력하세요\n");
			else
			{
				printf("1 : %s\n", tmpname);
				printf("2 : %s\n", pSNode -> data);
				f_mycpfrom(tmpname, pSNode -> data, &spblock, inode, dataBlock, tPwd);
			}

		}




//f_mycpfrom 함수//
void f_mycpfrom(char *name1, char *name2, SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{								//mycpfrom orig_fs.file my_fs.file
	printf("'mycpfrom' call\n");

	TNode *pTNode = NULL;
	DNode Head = {NULL, NULL};
	DNode *pTail = &Head;
	DNode *pDNode = NULL;
	int start, end;

	long long idNum = pwd -> idNum;
	int tmpfileSize = ind[idNum].fileSize;
	int dbNum, tmpidNum, indNum;

    FILE *ifp;
    ifp = fopen(name1, "r");
    if(ifp == NULL)
    {
        printf("'%s' is not found.\n", name1);
        fclose(ifp);
        return ;
    }

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
			if(!strncmp(pIndex -> pData -> directory.name[i], name2, 4))
			{
				rm(pSB, ind, pDB, pwd, name2);
				printf("old file\n");
			}
		}
	}

	//파일이 없을 때 혹은 삭제 후
	int index = addinfoPwd(ind, pTail -> pData, pwd, &idNum);

	indNum = prepareInode(pSB, ind, 1, 0);
	strncpy(pTail -> pData -> directory.name[index], name2, 4);
	pTail -> pData -> directory.idNum[index] = indNum;

    char c;
	int flag, DBnum, i;	
    while((c = fgetc(ifp)) != EOF)
    {
		printf("ind[indNum].fileSize:%d\n",ind[indNum].fileSize);
		if(((ind + indNum) -> fileSize) % 128 == 0)
		{
			flag = allocdbinIDdirect(pSB, &ind[indNum], pDB);

			if(flag == 1)
				flag = allocdbinIDsindirect(pSB, ind+indNum, pDB);

			if(flag == 1)
			{
				flag = allocdbinIDdlindirect(pSB, &ind[indNum], pDB);
				if(isBreak(flag))	{break;}
			}
		}
		storeDatainBlock(&ind[indNum], pDB, c);

		i++;
	}

	fclose(ifp);
}



//
void storeDatainBlock(Inode *pInode, Data *pData, char c)
{
	int dbNum;
	dbNum = readDbNuminID(pInode, pData, (pInode -> fileSize) / 128);
	pData[dbNum].file[(pInode -> fileSize) % 128] = c;
	pInode -> fileSize += 1;
}	
