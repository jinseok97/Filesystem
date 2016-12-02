#include "filesystem.h"
#include <string.h>
#define w_d "/ " //디렉토리 구현 후 수정해야

int prepareInode(SuperBlock *, Inode *, int, int);
int findInode(char *fileName, Data *pDB);

void cmd_judge(char [][10], SuperBlock *, Inode *, Data *);
void f_mytouch(char [][10], SuperBlock *, Inode *, Data *);
void f_mycpfrom(char [][10], SuperBlock *, Inode *, Data *);
void f_mycpto(char [][10], SuperBlock *, Inode *, Data *);
void f_command(char [][10]);

