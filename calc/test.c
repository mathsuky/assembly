#include <stdio.h>
int main()
{
	int i = 0;
	for (i = 0; i < 10; i++) {
		int mod = i % 3;
		switch (mod) {
			case 0:
				printf("mod 0\n");
				break;
			case 1:
				printf("mod 1\n");
				break;
			case 2:
				printf("mod 2\n");
				break;
		}
	}
}