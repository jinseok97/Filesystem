pSNode -> data / pSNode -> next -> data
void f_mycpfrom(char *name1, char *name2, SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{								//mycpfrom orig_fs.file my_fs.file
	printf("'mycpfrom' call\n");

	TNode *pTNode = NULL;
	DNode Head = {NULL, NULL};
	DNode *pTail = &Head;
	DNode *pDNode = NULL;

	int idNum = pwd -> idNum;
	int tmpfileSize = pInode[idNum].fileSize;
	int dbNum, tmpidNum, indNum;

    FILE *ifp;
    ifp = fopen(cmd_line[1], "r");
    if(ifp == NULL)
    {
        printf("'%s' is not found.\n", cmd_line[1]);
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
				tmpidNum = pIndex -> pData -> directory.idNum[i];
				ind[tmpidNum].time = time(NULL);
				ind[tmpidNum].fileSize = 0;
				printf("old file\n");
				return ;
				//이미 파일 존재할 때 초기화 필요
			}
		}
	}

	//파일이 없을 때
	index = addinfoPwd(ind, pTail -> pData, pwd, &idNum);

	indNum = prepareInode(pSB, ind, 1, 0);
	strncpy(pTail -> pData -> directory.name[index], name2);
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
