#include "filesystem.h"
#include "command.h"

int main(void)
{
	SuperBlock spBlock;
	initsuperBlock(&spBlock);

	Inode inode[512];

	Data dataBlock[1024];


	while(1)
	{
		char cmd[5][10] = {0};
		printBit(spBlock.usableInode[0]);
		printf("[\\]$ ");
		for(int i = 0; i < 5; i++)
		{
			scanf("%s", cmd[i]);
			if(getchar() == '\n')	break;
		}


		cmd_judge(cmd, &spBlock, inode, dataBlock); 
	}
		
	return 0;
}
