


		//main.c//
		else if(strcmp("mycp",SHAf.pNext->data)==0)
		{
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			for(pIndex=pSNode;pIndex!=NULL;pIndex=pIndex->pNext)
			{
				//	printf("pIndex->data:%s\n",pIndex->data);
				if(strcmp(pIndex->data, ">") == 0)
					continue;
				if((pIndex->pNext==NULL) &&(strlen(tmpname) == 0))
				{
					strcpy(tmpname,pIndex->data);
					continue;
				}
				if((strlen(catArg1) == 0))
				{
					strcpy(catArg1,pIndex->data);
					continue;
				}
				if(strlen(catArg2) == 0)
		{
					strcpy(catArg2,pIndex->data);
					continue;
				}
				flag = 2;
			}
			printf("tmpname:%s\n",tmpname);
			printf("catArg1:%s\n",catArg1);
			printf("catArg2:%s\n",catArg2);

			if(strcmp(pSNode->data,"/") == 0)
			{
				for(;pSNode != NULL;)
				{
					absolutePath(inode, dataBlock, &Root, &tPwd, &tUwd,pSNode);
					deleteSNode(&SHAf, pSNode);
					pSNode = SHAf.pNext;
				}
			}
			else 
			{
				for(;pSNode!=NULL;)
				{
					relativePath(inode,&Root,&tPwd,&tUwd,dataBlock,pSNode);
					deleteSNode(&SHAf, pSNode);
					pSNode = SHAf.pNext;
				}
			}
			mycat(&spblock,inode,dataBlock,tPwd,tmpname,catArg1,catArg2);
			putchar('\n');
			tmpname[0]='\0';
			catArg1[0]='\0';
			catArg2[0]='\0';

		}
