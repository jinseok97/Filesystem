#include <stdio.h>

int main(void)
{
	FILE *fp = fopen("myfs","rb");
	char a[20] = { 0};

	fseek(fp,0,SEEK_SET);
	fread(a,sizeof(char),4,fp);

	printf("%s\n",a);

	return 0;
}
