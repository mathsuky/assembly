#include <stdio.h>
#include <stdlib.h>
// メモ;imulの代わりにlea命令を使えるかも？
int calculate(int acc, char op, int num, int countS);

int main(int argc, char **argv)
{
	// 引数が1つでない場合はエラー
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <calculation expression>\n", argv[0]);
		return 1;
	}

	// 入力された要素を保持する変数
	// p: 入力文字列のポインタ, lastOp: 最後に入力された演算子
	char *p = argv[1], lastOp = '+';
	// num: 数値, acc: 累積値, mem: メモリ, countS: 符号反転キーのカウント
	int num = 0, acc = 0, mem = 0, countS = 0;

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			num = num * 10 + (*p - '0');
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え，最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}
			// 元の演算子に基づいて計算
			acc = calculate(acc, lastOp, num, countS);
			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			countS = 0;
			num = 0;
		}
		else if (*p == 'C') {
			// メモリをクリア
			mem = 0;
		}
		else if (*p == 'R') {
			// メモリから読み込み
			acc = mem;
		}
		else if (*p == 'P') {
			// メモリに加算
			mem += calculate(acc, lastOp, num, countS);
			// メモリに記録したので各種変数を初期値にリセット
			lastOp = '+';
			countS = 0;
			num = 0;
			acc = 0;
		}
		else if (*p == 'M') {
			// メモリから減算
			mem -= calculate(acc, lastOp, num, countS);
			// メモリに記録したので各種変数を初期値にリセット
			countS = 0;
			lastOp = '+';
			num = 0;
			acc = 0;
		}
		else if (*p == 'S') {
			countS++;
		}
		else {
			printf("電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}

	printf("%d\n", acc);

	return 0;
}

int calculate(int acc, char op, int num, int countS)
{
	// 符号反転キーが奇数回押された場合は符号反転
	if (countS % 2 == 1) {
		num *= -1;
	}
	// 演算子に基づいて計算
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
