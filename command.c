
		//main.c byebye 밑에 추가하기//
		//main에 char tmpname[10]; 선언 필요
		else if(SHAf.pNext -> data[0] != 'm' || SHAf.pNext -> data[1] != 'y')
		{
			strncpy(tmpname, SHAf.pNext -> data, 4);
			deleteSNode(&SHAf, SHAf.pNext);
			pSNode = SHAf.pNext;
			if(pSNode == NULL)		
				flag = 1;
			else 
			{
				if(strcmp(pSNode->data,"/") == 0)
				{
					for(;pSNode->pNext != NULL;)
					{
						absolutePath(inode, dataBlock, &Root, &tPwd, &tUwd,pSNode);
						if(pSNode->pNext != NULL)
						{
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
				else 
				{
					for(;pSNode->pNext!=NULL;)
					{
						relativePath(inode,&Root,&tPwd,&tUwd,dataBlock,pSNode);
						if(pSNode->pNext != NULL)
						{
							deleteSNode(&SHAf, pSNode);
							pSNode = SHAf.pNext;
						}
					}
				}
			}
			if(pSNode -> data != NULL)
			{
				strcat(tmpname, " ");
				strcat(tmpname, pSNode -> data);
			}

			printf("1 : %s\n", tmpname);
			f_command(tmpname);

		}
