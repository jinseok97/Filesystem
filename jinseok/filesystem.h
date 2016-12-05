#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifndef __file_system_
#define __file_system_

#define UI 8			//usable Inode
#define UD 16			//usable DataBlock
#define DS 128			//DataBlock Size
#define BAN 16			//Block Array Number

typedef struct tagSuperBlock
{
	/*unsigned*/ long long usableInode[UI];
	/*unsigned*/ long long usabledataBlock[UD];

}SuperBlock;

typedef union tagData Data;

typedef struct tagNode Node;

typedef struct tagInode Inode;

typedef struct tagDNode DNode ;

typedef struct tagDirectory
{
	char name[16][4];
	int idNum[16];
}Directory;

union tagData
{
	char file[DS];
	Directory directory;
	long long dataArr[BAN];
};

struct tagInode
{
	int fileType;
	time_t time;
	int fileSize;
	short direct;			//strData 사용	 	
	short sindirect;		//datatArr[BAN] 사용 
	short dlindirect;	//directArr[BAN] 사용 
};

struct tagDNode
{
	Data *pData;
	DNode *pNext;
};

typedef struct tagTNode TNode; 

struct tagTNode 
{
	TNode *pPrev;
	long long idNum;
	TNode *pNext;
};

typedef struct tagSNode SNode;

struct tagSNode
{
	char data[20];
	SNode *pNext;
};



unsigned long long movelluBit(int);

int moveintBit(int);

void printBit(unsigned long long );

void initsuperBlock(SuperBlock *);

bool findusableInode(/*unsigned*/ long long *, int *, int *) ;

void markInode(/*unsigned*/ long long *, int, int);

void unmarkInode(/*unsigned*/ long long *, int, int );

bool findusabledataBlock(/*unsigned*/ long long *, int * , int *);

void markdataBlock(/*unsigned*/ long long *, int, int);

void unmarkdataBlock(/*unsigned*/ long long *, int, int);

void initdataBlock(Data *);

bool checkbitArr(long long *, long long *, long long *);

void linkelementBit(long long *);

void storedbNum(long long *, long long, long long , long long );

int allocDbtoDb(Data *, long long);

long long readbitArr(Data *, int);

int allocbitDbtoDr(Data *, long long);

int allocbitDrtoSin(Data *, long long);

bool isBreak(int);

void initDataBlock(Data *);

int preparedataBlock(Data *,long long *,long long *);

int allocdbinIDdirect(SuperBlock *, Inode *, Data *);

int allocdbinIDsindirect(SuperBlock *, Inode *, Data *);

int allocdbinIDdlindirect(SuperBlock *, Inode *, Data *);

long long readIDDire(Inode *);

long long readDireinSin(Data *, long long ,int);

long long readIDSinD(Inode *, Data *, int);

long long readSinDinDlin(Data *, long long,int);

long long readIDDlin(Inode *, Data *, int);

long long readDbNuminID(Inode * ,Data *, int );

DNode *createDBNode(Data *);

void insertDBNode(DNode **, DNode *);

void deleteDBNode(DNode *, DNode **);

DNode *findDBPrevNode(DNode *, DNode *);

TNode *createTNode(long long);

void insertleftTNode(TNode *, TNode *);

void deleteTNodeNext(TNode *pPrev, TNode *pRemo);

void deleteTNodePrev(TNode *pPrev, TNode *pRemo);

void saveNuminTNode(TNode *pTNode, long long idNum);

long long getNumfromTNode(TNode *pTNode);

void deleteTNode(TNode *pPrev, TNode *pRemo);

SNode *createSNode(void);

void pushSNodetoStack(SNode *pHead, SNode *pSNode);

void savedatatoSNode(SNode *pSNode, char c, int index);

void deleteSNode(SNode *pHead, SNode *pRemo);
#endif
