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
	dataBlock[0].directory.name[0][0] = '/';
	dataBlock[0].directory.idNum[0]= 0;
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
				printf("	[#%d IND] ", i);
				printf("Type : %d | ", inode[i].fileType);
				printf("Size : %d | ", inode[i].fileSize);
				printf("Time : %ld\n", inode[i].fileTime);
			}
			printf("	  	      ROOTDIRECTORY\n");
			for(int i = 0; i < 10; i++)
			{
				printf("		    %4d", dataBlock[0].directory.idNum[i]);
				printf("      %4s\n", dataBlock[0].directory.name[i]);
			}
			printBit(spBlock.usableInode[0]);
			printBit(spBlock.usabledataBlock[0]);
		}
		#endif
	}
		
	return 0;
}
