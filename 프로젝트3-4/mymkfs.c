#include <stdio.h>
#include "filesystem.h"

int main(void)
{
	FILE *fp ;

	SuperBlock spblock;

	Inode inode[512];

	Data dataBlock[1024];

	initsuperBlock(&spblock);

	for(int i=0;i<512;i++)
		initInode(inode+i);
	for(int i=0;i<512;i++)
		initdataBlock(dataBlock+i);

	fp=fopen("myfs","w");

	for(int i=0;i<64;i++)
		fprintf(fp,"%lld",spblock.usableInode[i]);
	putchar('\n');

	for(int i=0;i<64;i++)
		fprintf(fp,"%lld",spblock.usabledataBlock[i]);
	putchar('\n');

	for(int i=0;i<512;i++)
		fprintf(fp,"%d %d %hd %hd %hd\n",inode[i].fileType,inode[i].time,inode[i].fileSize,inode[i].direct,inode[i].sindirect, inode[i].dlindirect);

	for(int i=0;i<1024;i++)`
	{
		for(int j=0;j<128;j++)
			fprintf(fp,"%c",dataBlock[i].file[j]);
		putchar('\n');
	}
}
