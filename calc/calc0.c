#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	// 引数が1つでない場合はエラー
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <number>\n", argv[0]);
		return 1;
	}

	// 入力された要素を保持する変数
	char *p = argv[1], lastOp = '+';
	int num = 0, acc = 0;

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			num = num * 10 + (*p - '0');
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が変わったら，元の演算子に基づいて計算
			switch (lastOp) {
				case '+':
					acc += num;
					break;
				case '-':
					acc -= num;
					break;
				case '*':
					acc *= num;
					break;
				case '/':
					acc /= num;
					break;
			}
			// 演算子を更新
			lastOp = *p;
			num = 0;
		}
		else {
			fprintf(stderr, "Invalid character: %c\n", *p);
			return 1;
		}
		p++;
	}
	printf("%d\n", acc);
}