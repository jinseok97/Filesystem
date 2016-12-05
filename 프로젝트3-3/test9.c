#include "filesystem.h"


int main(void)
{
	FILE *ofp = fopen("myfs","wb");
	FILE *ifp = fopen("myfs","rb");

	SuperBlock spblock;

	initsuperBlock(&spblock);

	fread(&spblock, sizeof(SuperBlock), 1, ifp);
	fflush(ifp);
	for(int i=0;i<16;i++)
	printBit(spblock.usableInode[i]);
	return 0;
}
