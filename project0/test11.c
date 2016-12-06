#include <stdio.h>
#include "filesystem.h"

int main(void)
{
	Data test;
	test.directory.name[0][0] ='1';
	test.directory.name[0][1] ='2';
	test.directory.name[0][2] ='3';

	Data test2=test;

	printf("test.name[0]:%s\n",test2.directory.name[0]);
	return 0;
}
