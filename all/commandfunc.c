#include "command.h"
#include "filesystem.h"

int findInode(char *fileName, Data *pDB)
{
    int i;
    for(i = 0; i < 16; i++)
        if(!strncmp(pDB -> directory.name[i], fileName, 4))
            return pDB -> directory.idNum[i];
    
    return 512;
}

int findemptyDir_line(Data *pDB)
{
	for(int i = 0; i < 16; i++)
		if(pDB -> directory.name[i][0] == 0)
			return i;

	printf("full directory\n");
	return -1;
}

int prepareInode(SuperBlock *pSB, Inode *ind, int fType, int fSize)
{
    int findarrNum = -1;
    int findbitNum = -1;
    int indNum = 0;
    
    findusableInode(pSB -> usableInode, &findarrNum, &findbitNum);
    markInode(pSB -> usableInode, findarrNum, findbitNum);
    indNum = findarrNum * 64 + findbitNum - 1;
    
    ind[indNum].fileType = fType;
    ind[indNum].fileSize = fSize;
    ind[indNum].fileTime = time(NULL);	//"filesystem.h" time_t * -> time_t로 수정
    
    
/*	printf("iNode[%d]\n", indNum);
    printf("fType : %d\n", ind[indNum].fileType);
    printf("fSize : %d\n", ind[indNum].fileSize);
    printf("fTime : %d\n", ind[indNum].fileTime);	*/
    return indNum;
}

void cmd_judge(char cmd[][10], SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{
	if(!strcmp(cmd[0], "mytouch"))
		f_mytouch(cmd, pSB, ind, pDB, pwd);
	else if(!strcmp(cmd[0], "mycp"))	
		f_mycp(cmd, pSB, ind, pDB, pwd);
	else if(!strcmp(cmd[0], "mycpfrom"))
		f_mycpfrom(cmd, pSB, ind, pDB, pwd);
	else if(!strcmp(cmd[0], "mycpto"))
		f_mycpto(cmd, pSB, ind, pDB, pwd);
	else if(cmd[0][0] >= '0' && cmd[0][0] <= '9')
	{	//아이노드검사
		int i = atoi(cmd[0]);
		printf("			INODE PRINT\n	[#%d IND] ", i);
		printf("Type : %d | Size : %d | Time : %d\n", ind[i].fileType, ind[i].fileSize, ind[i].fileTime);
		printBit(pSB -> usableInode[i / 64]);
	}
	else if(cmd[0][0] != 'm' || cmd[0][1] != 'y')
		f_command(cmd);
	else
		printf("mysh : %s : command not found\n", cmd[0]);

}

void f_mytouch(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{
//	printf("**mytouch call : ");
	short wd = ind[pwd -> idNum].direct;
 	int indNum = findInode(cmd_line[1], &pDB[wd]);
    
    if(indNum == 512)
	{
		printf("touch new file\n");
//		printf("check : %d\n", check);

		int check = findemptyDir_line(&pDB[wd]);
		if(check == -1)		return ;
        pDB[wd].directory.idNum[check] = prepareInode(pSB, ind, 1, 0);
		for(int i = 0; i < 4; i++)				
			pDB[wd].directory.name[check][i] = cmd_line[1][i];

		ind[indNum].direct = -1;
		ind[indNum].sindirect = -1;
		ind[indNum].dlindirect = -1;
	}
    else
	{
		printf("touch old file\n");
        ind[indNum].fileTime = time(NULL);
	}
//	printf("indNum = %d\n", pDB[wd].directory.idNum[check]);
}

void f_mycp(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{
	short wd = ind[pwd -> idNum].direct;
	int indNum1 = findInode(cmd_line[1], &pDB[wd]);
	if(indNum1 == 512)
	{
		printf("'%s' is not found.\n", cmd_line[1]);
		return ;
	}

	int indNum2 = findInode(cmd_line[2], &pDB[wd]);
	if(indNum2 == 512)
	{
		int check = findemptyDir_line(&pDB[wd]);
		if(check == -1)		return ;
		indNum2 = prepareInode(pSB, ind, ind[indNum1].fileType, ind[indNum1].fileSize);	
		pDB[wd].directory.idNum[check] = indNum2;
		for(int i = 0; i < 4; i++)				
			pDB[wd].directory.name[check][i] = cmd_line[2][i];
	}
	else
	{
		ind[indNum2].fileSize = ind[indNum1].fileSize;
		ind[indNum2].fileTime = time(NULL);
	}

	//데이터블럭 복사//
}

void f_mycpfrom(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{								//mycpfrom orig_fs.file my_fs.file
	printf("**mycpfrom call\n");
	short wd = ind[pwd -> idNum].direct;
	int DBnum;
    FILE *ifp;
    ifp = fopen(cmd_line[1], "r");
    if(ifp == NULL)
    {
        printf("'%s' is not found.\n", cmd_line[1]);
        fclose(ifp);
        return ;
    }

	int indNum = findInode(cmd_line[2], &pDB[wd]);
	if(indNum == 512)
	{
		int check = findemptyDir_line(&pDB[wd]);
		if(check == -1)		return ;
		indNum = prepareInode(pSB, ind, 1, 0);
		pDB[wd].directory.idNum[check] = indNum;
		for(int i = 0; i < 4; i++)
			pDB[wd].directory.name[check][i] = cmd_line[2][i];
	}

    char c;
	int flag;					//수정이 필요(데이터블럭할당)
    for(int i = 0; (c = getc(ifp)) != EOF; i++)
    {
		if(i % 128 == 0)
		{
			flag = allocdbinIDdirect(pSB, ind, pDB);

			if(flag == 1)
				flag = allocdbinIDdlindirect(pSB, ind, pDB);

			if(flag == 1)
			{
				flag = allocdbinIDdlindirect(pSB, ind, pDB);
				if(isBreak(flag))	break;
			}
		}
		DBnum = readDbNuminID(ind, pDB, i / 128 + 1);
		printf("DBnum : %d\n", DBnum);
		pDB[DBnum].file[i % 128] = c;
	}


    fclose(ifp);
}

void f_mycpto(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB, TNode *pwd)
{								//mycpto my_fs.file orig_fs.file
	printf("**mycpto call\n");
	short wd = ind[pwd -> idNum].direct;
	int indNum = findInode(cmd_line[1], &pDB[wd]);
	FILE *ofp;
	if(indNum != 512)
	{
		ofp = fopen(cmd_line[2], "w");
		fprintf(ofp, "datablock linkedlist\n");	//데이터블럭 링크드리스트 출력
		fclose(ofp);
	}
	else	
		printf("'%s' is not found.\n", cmd_line[1]);
}

void f_command(char cmd_line[][10])
{
	if(!strcmp(cmd_line[0], "byebye"))
		exit(1);

	strcat(cmd_line[0], " ");			//배열크기 고려안했음
	strcat(cmd_line[0], cmd_line[1]);	//"ls""-a" > "ls -a"

	system(cmd_line[0]);
}
