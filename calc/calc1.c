#include <stdio.h>
#include <stdlib.h>

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

	// AT&T形式のアセンブリ出力の準備
	printf(".text\n");
	printf(".globl _main\n");
	printf("_main:\n");
	printf("  pushq %%rbp\n");
	printf("  movq %%rsp, %%rbp\n");
	printf("  movl $0, %%eax\n");  // accを初期化
	printf("  movl $0, %%ebx\n");  // numを初期化

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 数字の場合は数値を構築
			num = num * 10 + (*p - '0');
			// アセンブリでnumの更新
			printf("  imull $10, %%ebx, %%ebx\n");		// ebxに10を掛ける
			printf("  addl $%d, %%ebx\n", (*p - '0'));	// ebxに対応する数値を加える
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}

			// 符号反転キーが奇数回押された場合は符号を反転
			if (countS % 2 == 1) {
				num *= -1;
				printf("  negl %%ebx\n");  // numの符号を反転
			}

			// 演算子に基づいて計算
			switch (lastOp) {
				case '+':
					acc += num;
					printf("  addl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					acc -= num;
					printf("  subl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					acc *= num;
					printf("  imull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					acc /= num;
					printf("  xorl %%edx, %%edx\n");  // 除算の前にedxをクリア
					printf("  idivl %%ebx\n");		  // accをnumで除算
					break;
			}

			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			num = 0;
			printf("  movl $0, %%ebx\n");  // numを初期化
			countS = 0;
		}
		else if (*p == 'C') {
			// メモリをクリア
			mem = 0;
			printf("  movl $0, %%ecx\n");  // メモリをクリア
		}
		else if (*p == 'R') {
			// メモリから読み込み
			acc = mem;
			printf("  movl %%ecx, %%eax\n");  // accにメモリからの値を読み込み
		}
		else if (*p == 'P') {
			// 符号反転キーが奇数回押された場合は符号反転
			if (countS % 2 == 1) {
				num *= -1;
				printf("  negl %%ebx\n");  // numの符号を反転
			}
			// メモリに加算
			mem += acc + num;
			printf("  addl %%eax, %%ecx\n");  // メモリに加算
			printf("  addl %%ebx, %%ecx\n");

			// 各種変数を初期化
			num = 0;
			acc = 0;
			lastOp = '+';
			printf("  movl $0, %%ebx\n");
			printf("  movl $0, %%eax\n");
			countS = 0;
		}
		else if (*p == 'M') {
			// 符号反転キーが奇数回押された場合は符号反転
			if (countS % 2 == 1) {
				num *= -1;
				printf("  negl %%ebx\n");  // numの符号を反転
			}
			// メモリから減算
			mem -= acc + num;
			printf("  subl %%eax, %%ecx\n");  // メモリから減算
			printf("  subl %%ebx, %%ecx\n");

			// 各種変数を初期化
			num = 0;
			acc = 0;
			lastOp = '+';
			printf("  movl $0, %%ebx\n");
			printf("  movl $0, %%eax\n");
			countS = 0;
		}
		else if (*p == 'S') {
			countS++;
		}
		else {
			printf("電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}

	// 計算結果を出力
	printf("  movl %%eax, %%edi\n");
	printf("  call _printf\n");
	printf("  movl $0, %%eax\n");  // 正常終了
	printf("  leave\n");
	printf("  ret\n");

	return 0;
}
