#include "filesystem.h"

int main(void)
{
	SuperBlock spblock;
	Inode inode;
	Data dataBlock[1024];
	char c ;
	int i =0;
	int flag = 0;
	int SinD;
	int Dire;

	initsuperBlock(&spblock);
	inode.fileSize = 0;

	while((c=getchar()) != '\n')
	{
		if(i % 128 == 0)
		{
			flag=allocdbinIDdirect(&spblock,&inode, dataBlock);

			if(flag == 1)
				flag=allocdbinIDsindirect(&spblock,&inode,dataBlock);

			if(flag == 1)
			{
				flag=allocdbinIDdlindirect(&spblock, &inode, dataBlock);	
				if(isBreak(flag))	{	break;	}
			}

		}

		if(inode.fileSize / 128 == 1)
			dataBlock[inode.direct].file[i] = c;

		if((inode.fileSize / 128 >= 2) && (inode.fileSize / 128 <=104))
		{
			SinD = readbitArr(dataBlock + inode.sindirect, inode.fileSize/128 - 2);
			dataBlock[SinD].file[i%128] = c;
		}

		if((inode.fileSize / 128 >= 105) && (inode.fileSize /128 <=10611))
		{
			SinD = readbitArr(dataBlock + inode.dlindirect, (inode.fileSize/128 - 105 - 1) / 103 + 1);
			Dire = readbitArr(dataBlock + SinD, (inode.fileSize/128 - 105 - 1) % 103 );

			dataBlock[Dire].file[i%128] = c;
		}

		i++;
	}

	return 0;

}
