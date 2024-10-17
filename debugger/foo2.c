#include <stdio.h>
int main()
{
	int *p = (int *)0xFFFFFFFFFFFFFFFF;
	printf("%d\n", *p);
}