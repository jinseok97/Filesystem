#include "command.h"
#include "filesystem.h"

int findInode(char *fileName, Data *pDB)
{
	/*pDB 는 현재 디렉터리의 데이터블럭*/
    int i;
    for(i = 0; i < 16; i++)
        if(!strcmp(pDB -> directory[i][1], fileName))
            return atoi(pDB -> directory[i][0]);
    
    return 512;
}

int prepareInode(SuperBlock *pSB, Inode *ind, int fType, int fSize/*direct*/)
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
//	ind[indNum].direct ... 지정 필요
//	allocdbinIDdirect(pSB, ind, pData);
    
    
/*	printf("iNode[%d]\n", indNum);
    printf("fType : %d\n", ind[indNum].fileType);
    printf("fSize : %d\n", ind[indNum].fileSize);
    printf("fTime : %d\n", ind[indNum].fileTime);	*/
    return indNum;
}

void cmd_judge(char cmd[][10], SuperBlock *pSB, Inode *ind, Data *pDB)
{
	if(!strcmp(cmd[0], "mytouch"))
		f_mytouch(cmd, pSB, ind, pDB);
	else if(!strcmp(cmd[0], "mycpfrom"))
		f_mycpfrom(cmd, pSB, ind, pDB);
	else if(!strcmp(cmd[0], "mycpto"))
		f_mycpto(cmd, pSB, ind, pDB);
	else if(cmd[0][0] != 'm' || cmd[0][1] != 'y')
		f_command(cmd);
	else
		printf("mysh : %s : command not found\n", cmd[0]);

//	else if(!strcmp(cmd[0], "mycp"))	
//		f_mycp(cmd, pSB, ind, pDB);
}

void f_mytouch(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB)
{
	printf("**mytouch call\n");
// 	int indNum = findInode(cmd_line[1], &pDB[pwd]/*현재디렉터리*/);
	int indNum = 512;
    
    if(indNum == 512)
        prepareInode(pSB, ind, 1, 0);
		//pDB[pwd] -> directory[n][0/1]에 ind, filename 추가
    else
        ind[indNum].fileTime = time(NULL);
}

void f_mycpfrom(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB)
{								//mycpfrom orig_fs.file my_fs.file
	printf("**mycpfrom call\n");
    FILE *ifp;
    int fileSize = 0;
    char c;
    
    ifp = fopen(cmd_line[1], "r");
    if(ifp == NULL)
    {
        printf("'%s' is not found.\n", cmd_line[1]);
        fclose(ifp);
        return ;
    }
    
    while(1)
    {
        if((c = getc(ifp)) == EOF)	break;
        fileSize++;
    }
    
    prepareInode(pSB, ind, 1, fileSize);
    
    fclose(ifp);
}

void f_mycpto(char cmd_line[][10], SuperBlock *pSB, Inode *ind, Data *pDB)
{								//mycpto my_fs.file orig_fs.file
	printf("**mycpto call\n");
	FILE *ofp;
/*	int indNum = findInode(cmd_line[1], &pDB[pwd]);
	if(indNum != 512)
	{
		fopen(ofp, cmd_line[2], "w");
		//데이터 링크드리스트 출력//	
			fprintf(ofp, "%s", pDB[NUM].file);
		fclose(ofp);
	}
	else	*/
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
