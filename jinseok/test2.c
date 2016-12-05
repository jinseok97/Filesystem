#include "filesystem.h"
/*
   int allocbitDbtoDr(Data *pData, long long dbNum)
   {
   int flag = 0;

   flag = allocDbtoDb(pData, dbNum);

   return flag;
   }

   int allocbitDrtoSin(Data *pData, long long dbNum)
   {
   int flag = 0;

   flag = allocDbtoDb(pData, dbNum);

   return flag;
   }
   */
/*
bool isBreak(int flag)
{
	if(flag == 1)	
	{
		printf("해당 데이터 블럭에 추가적으로 데이터 블럭을 할당할 수 없습니다.\n");
		return 1;
	}
	if(flag == 2)
	{
		printf("모든 데이터 블럭을 다 할당 하였습니다.\n");
		return 1;
	}

	return 0;
}
*/
int main(void)
{
	Inode inode;

	Data dataBlock[1024];

	int flag = 0;
	long long result = 0;

	inode.sindirect = 0;
	initdataBlock(dataBlock + 0);
/*
	for(int i=0;i<103;i++)
	{
		flag = allocbitDrtoSin(dataBlock + 0, i);

		if(isBreak(flag))	{ break ;}
	}
	for(int i=1;i<103;i++)
	{
		if(isBreak(flag))	{ break ;}

		result = readbitArr(dataBlock + 0, i);	
		printf("%llu\n",result);

	}
	*/
	return 0;

}
