#include <stdio.h>
#include <stdlib.h>

int calculate(int acc, char op, int num, char sign);

int main(int argc, char **argv)
{
	// 引数が1つでない場合はエラー
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <number>\n", argv[0]);
		return 1;
	}

	// 入力された要素を保持する変数
	char *p = argv[1], lastOp = '+', sign = '+';
	int num = 0, acc = 0, mem = 0;

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			num = num * 10 + (*p - '0');
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え，最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}
			// 演算子が変わったら，元の演算子に基づいて計算
			acc = calculate(acc, lastOp, num, sign);
			// 演算子を更新
			lastOp = *p;
			num = 0;
			sign = '+';	 // signをリセット
		}
		else if (*p == 'C') {
			mem = 0;  // メモリをクリア
		}
		else if (*p == 'R') {
			acc = mem;	// メモリから読み込み
		}
		else if (*p == 'P') {
			// printf("lastOp = %c, num = %d, acc = %d, mem = %d\n", lastOp, num, acc, mem);
			mem += calculate(acc, lastOp, num, sign);  // メモリに加算
			lastOp = '+';
			num = 0;
			acc = 0;
			// printf("lastOp = %c, num = %d, acc = %d, mem = %d\n", lastOp, num, acc, mem);
		}
		else if (*p == 'M') {
			// printf("lastOp = %c, num = %d, acc = %d, mem = %d\n", lastOp, num, acc, mem);
			mem -= calculate(acc, lastOp, num, sign);  // メモリから減算
			lastOp = '+';
			num = 0;
			acc = 0; // todo: ここでaccをリセットするのは正しいか？
			// printf("lastOp = %c, num = %d, acc = %d, mem = %d\n", lastOp, num, acc, mem);
		}
		else if (*p == 'S') {
			sign = (sign == '+') ? '-' : '+';  // 符号を反転
		}
		else {
			printf("電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}

	printf("%d\n", acc);

	return 0;
}

int calculate(int acc, char op, int num, char sign)
{
	if (sign == '-') {
		num = -num;
	}

	switch (op) {
		case '+':
			return acc + num;
		case '-':
			return acc - num;
		case '*':
			return acc * num;
		case '/':
			return acc / num;
		default:
			return acc;
	}
}
