/******************************************************
 make 명령으로 컴파일
 gcc -o my_shell main.c filesystem_func.c commandfunc.c
 ******************************************************/

#include "filesystem.h"
#include <string.h>

#ifdef NOFILE
	#undef NOFILE
	#define NOFILE 512
#endif

#define	DEBUG	1

int prepareInode(SuperBlock *, Inode *, int, int);
int findInode(char *fileName, Data *pDB);
int findemptyDir_line(Data *);
void storeDatainBlock(Inode *pInode, Data *pData, char c);

void cmd_judge(char [][10], SuperBlock *, Inode *, Data *, TNode *);
//void f_mytouch(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mycp(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mycpfrom(char *, char *, SuperBlock *, Inode *, Data *, TNode *);
//void f_mycpto(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_command(char *);

