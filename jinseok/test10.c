#include <stdio.h>

int main(void)
{
	FILE *fp = fopen("myfs","ab");
	char a[20]="abc";
	char b[20]="def";
	char c[20]="123";


	//fseek(fp,0,SEEK_SET);
	//fwrite(a,sizeof(char),4,fp);
	//fseek(fp,4,SEEK_SET);
	//fwrite(b,sizeof(char),4,fp);
	fwrite(c,sizeof(char),4,fp);

	fclose(fp);

	return 0;
}
