#include "filesystem.h"

int main(void)
{
	SNode SHBf = {{0},NULL};
	SNode SHAf = {{0},NULL};
	SNode *pSNode = NULL;


	char c;
	int i=0;

	while((c=getchar()) != '\n')
	{
		if(i==0 && c=='/')
		{
			pSNode=createSNode();
			savedatatoSNode(pSNode, c, i );
			pushSNodetoStack(&SHBf, pSNode);
			continue;
		}
		if(i==0)
			pSNode=createSNode();
		if(c=='/' || c==' ')	
		{
			pushSNodetoStack(&SHBf, pSNode);
			i=0;
			continue;
		}

		savedatatoSNode(pSNode, c, i );
		i++;

	}
	pushSNodetoStack(&SHBf, pSNode);

	/*
	   for(SNode *pSNodeI =SHBf.pNext;pSNodeI!= NULL;pSNodeI=pSNodeI->pNext)
	   {
	   printf("pSNodeI:%p, pSNodeI->pNext:%p\n",pSNodeI, pSNodeI->pNext);
	   for(int i=0;i<4;i++)
	   printf("pSNodeI->data[%d]:%c\n",i,pSNodeI->data[i]);
	   putchar('\n');
	   }
	   */
	for(SNode *pSNodeI =SHBf.pNext;pSNodeI!= NULL;pSNodeI=SHBf.pNext)
	{
		pSNode = createSNode();
		for(int i=0;i<20;i++)
			pSNode->data[i] = pSNodeI->data[i];
		deleteSNode(&SHBf,pSNodeI);
		pushSNodetoStack(&SHAf, pSNode);
	}


	for(SNode *pSNodeI =SHAf.pNext;pSNodeI!= NULL;pSNodeI=pSNodeI->pNext)
	{
		printf("pSNodeI:%p, pSNodeI->pNext:%p\n",pSNodeI, pSNodeI->pNext);
		for(int i=0;i<20;i++)
			printf("%c",pSNodeI->data[i]);
		putchar('\n');
	}

	return 0;
}
