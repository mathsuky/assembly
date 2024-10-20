#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "引数の個数が正しくありません\n");
		return 1;
	}

	char *p = argv[1];

	// AT&T記法でのアセンブリコードを出力
	printf(".section .text\n");
	printf(".globl main\n");
	printf("main:\n");

	// まず最初の数値をraxに読み込む
	printf("  movq $%ld, %%rax\n", strtol(p, &p, 10));

	while (*p) {
		if (*p == '+') {
			p++;
			printf("  addq $%ld, %%rax\n", strtol(p, &p, 10));
			continue;
		}

		if (*p == '-') {
			p++;
			printf("  subq $%ld, %%rax\n", strtol(p, &p, 10));
			continue;
		}

		// 予期しない文字に対するエラーメッセージ
		fprintf(stderr, "予期しない文字です: '%c'\n", *p);
		return 1;
	}

	// 計算結果がraxに入った状態で終了
	printf("  ret\n");
	return 0;
}
