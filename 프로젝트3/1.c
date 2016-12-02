#include <stdio.h>

int main(void)
{
	FILE *ofp = fopen("myfs.txt","r");

	fseek(ofp,0,SEEK_SET);
	printf("%d\n",ftell(ofp));
	fseek(ofp,0,SEEK_END);
	printf("%d\n",ftell(ofp));

	return 0;
}
