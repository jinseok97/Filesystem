#include "filesystem.h"
#include "command.h"

int main(void)
{
	SuperBlock spBlock;
	initsuperBlock(&spBlock);
	Inode inode[512] = {0};
	Data dataBlock[1024] = {0};

	/* 루트 디렉터리 */
	markInode(spBlock.usableInode, 0, 1);
	markdataBlock(spBlock.usabledataBlock, 0, 1);
	inode[0].direct = 0;
	TNode Root = {NULL, 0, NULL};
	TNode *pwd = &Root;

	while(1)
	{
		char cmd[5][10] = {0};
		printf("[\\]$ ");
		for(int i = 0; i < 5; i++)
		{
			scanf("%s", cmd[i]);
			if(getchar() == '\n')	break;
		}

		cmd_judge(cmd, &spBlock, inode, dataBlock, pwd); 
		#if DEBUG
		{
			printf("			INODEPRINT\n");
			for(int i = 0; i < 10; i++)
			{
				printf("   [#%d IND] ", i);
				printf("Type : %d | ", inode[i].fileType);
				printf("Size : %3d | ", inode[i].fileSize);
				printf("DB : %d", inode[i].direct);
				printf(" %d", inode[i].sindirect);
				printf(" %d |", inode[i].dlindirect);
				printf("Time : %ld\n", inode[i].time);
			}
			printf("	  	      ROOTDIRECTORY\n");
			for(int i = 0; i < 10; i++)
			{
				printf("		    %4d", dataBlock[0].directory.idNum[i]);
				printf("      ");
				for(int j = 0; j < 4; j++)
				{
					printf("%c", dataBlock[0].directory.name[i][j]);
				}
				putchar('\n');
			}
			printBit(spBlock.usableInode[0]);
			printBit(spBlock.usabledataBlock[0]);
		}
		#endif
	}
		
	return 0;
}
