#include "filesystem.h"

/*
void initdataBlock(Data *pDb)
{
	for(int i=0;i<16;i++)
		pDb->dataArr[i] = -1;
}

bool checkbitArr(long long *pDb, long long *arrNum, long long *bitNum)
{
	long long mask1=movelluBit(1);
	long long mask10 = 0;
	long long tmp[16];
	long long result = 0;

	int k = 1;

	for(int i=1;i<11;i++)
		mask10 +=  (long long) movelluBit(i);

	for(int i=0;i<16;i++)
		tmp[i] = pDb[i];

	while(1)
	{
		result = (mask10 & tmp[0]);
		if((result&mask10) == mask10)
		{
			*arrNum =(k*10)/64 ;
			*bitNum =(k*10)%64 ;
			break;
		}

		tmp[0] <<= 10;
		for(int i=1;i<16;i++)
			for(int j=0;j<10;j++)
			{
				if((mask1 & tmp[i]) != 0)
					tmp[i-1] += (long long) movelluBit(64-(10-j)+1);
				tmp[i] <<= 1;
			}
		if(k==102)		{ return 1; }
		k++ ;
	}

	return 0;
}

void storedbNum(long long *pDb, long long arrNum, long long bitNum, long long dbNum)
{
	if(arrNum == -1 || bitNum == -1)	{ return ; }

	if(bitNum >= 10)
		for(int i=0;i<10;i++)
		{
			if(dbNum % 2 == 1)
				pDb[arrNum] -= movelluBit(bitNum - i);
			dbNum /= 2;
		}
	if(bitNum < 10)
	{
		for(int i=0;i<bitNum;i++)
		{
			if(dbNum%2 == 1)
				pDb[arrNum] -= movelluBit(bitNum - i);
		dbNum /= 2;
		}
		for(int i=0;i<10-bitNum;i++)
		{
			if(dbNum%2 == 1)
				pDb[arrNum-1] -= movelluBit(64 - i);
		dbNum /= 2;
		}

	}
}

bool allocdbinSin(Data *pData, long long dbNum)
{
	long long arrNum = -1, bitNum = -1;
	bool isFull = 0;

	isFull = checkbitArr(pData->dataArr, &arrNum, &bitNum);

	if(isFull)		{ return 1 ; }

	storedbNum(pData->dataArr,arrNum,bitNum,dbNum);

	return 0;
}
*/
/*
long long readbitArr(Data *pData, int k)
{
	long long mask10 = 0;
	long long result = 0;
	long long mid;
	long long tmp[16];

	for(int i=1;i<=10;i++)
		mask10 += movelluBit(i);

	for(int i=0;i<16;i++)
		tmp[i] = pData->dataArr[i];

	for(int i=1;i<k;i++)
		linkelementBit(tmp,mask1);

	mid=(((tmp[0])^mask10)&mask10) ;

	for(int i=0;i<10;i++)
	{
		if((mask1 & mid)== mask1)
			result += movelluBit(64-(10-i)+1);
		mid<<=1;
	}

	return result;
}
*/
int main(void)
{
	Data dataBlock[1024];
	long long arrNum = -1;
	long long bitNum = -1;
	long long result;
	bool isExist = 0;
	int abc;

	initdataBlock(dataBlock);
/*
	for(int i=0;i<102;i++)
	{
		printf("[%d]",i);
		checkbitArr(dataBlock[0].dataArr, &arrNum, &bitNum);
		storedbNum(dataBlock[0].dataArr, arrNum, bitNum,i) ;
		printf("arrNum:%d, bitNum:%d\n",arrNum,bitNum);
	}
	for(int i=0;i<16;i++)
	{
		printf("[%d]:",i);
		printBit(dataBlock[0].dataArr[i]);
	}
*/

	for(int i =0; i<102;i++)
		allocdbinSin(dataBlock, i + 1);
	/*
	for(int i=0;i<16;i++)
	{
		printf("[%d]:",i);
		printBit(dataBlock[0].dataArr[i]);
	}
	*/
	for(int i=1;i<=10;i++)
	{
	result = readbitArr(dataBlock, i); 
	printf("result:%llu\n",result);
	}

	return 0;
}


