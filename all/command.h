#include "filesystem.h"
#include <string.h>

int prepareInode(SuperBlock *, Inode *, int, int);
int findInode(char *fileName, Data *pDB);
int findemptyDir_line(Data *);

void cmd_judge(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mytouch(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mycp(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mycpfrom(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_mycpto(char [][10], SuperBlock *, Inode *, Data *, TNode *);
void f_command(char [][10]);

