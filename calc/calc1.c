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
	// int num = 0, acc = 0, mem = 0, countS = 0;

	// AT&T形式のアセンブリ出力の準備
	printf(
		".data\n"
		"L_fmt:\n"
		"\t.ascii \"%%d\\n\\0\"\n"
		".text\n"
		".globl _main\n"
		".extern _exit\n"
		"_main:\n"
		"\tpushq %%rbp\n"
		"\tmovq %%rsp, %%rbp\n");
	printf("\tmovl $0, %%eax\n");  // accを初期化
	printf("\tmovl $0, %%ebx\n");  // numを初期化
	printf("\tmovl $0, %%ecx\n");  // 符号反転キーのカウントを初期化
	printf("\tpushq $0\n");		   // 電卓のメモリはスタックで管理する。メモリを初期化
	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 数字の場合は数値を構築
			// num = num * 10 + (*p - '0');
			// アセンブリでnumの更新
			printf("\timull $10, %%ebx, %%ebx\n");		// ebxに10を掛ける
			printf("\taddl $%d, %%ebx\n", (*p - '0'));	// ebxに対応する数値を加える
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}
			// TODO　講義資料5-25pを見よ
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			// 演算子に基づいて計算
			switch (lastOp) {
				case '+':
					// acc += num;
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					// acc -= num;
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// acc *= num;
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					// acc /= num;
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリア todo: 本当に必要？cltd命令で十分では？確認する
					printf("\tcltd\n");				  // idiv命令の前にcltd命令
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}

			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			// num = 0;
			printf("\tmovl $0, %%ebx\n");  // numを初期化
			printf("\tmovl $0, %%ecx\n");  // countS = 0;
		}
		else if (*p == 'C') {
			// メモリをクリア
			// mem = 0;
			// スタックからメモリを取り出す
			// printf("\taddq $8, %%rsp\n");  // スタックポインタを8バイト進める（64ビットモードの場合） todo:こっちの方がいいんじゃね？知らんけど
			printf("\tpopq %%rdx\n");
			// メモリを初期化
			printf("\tpushq $0\n");
		}
		else if (*p == 'R') {
			// メモリから読み込み
			// acc = mem;
			printf("\tpopq %%rdx\n");		  // スタックからメモリを取り出す
			printf("\tmovl %%edx, %%eax\n");  // メモリをaccにロード
			// スタックにメモリを戻す
			printf("\tpushq %%rdx\n");
		}
		else if (*p == 'P') {
			// 符号反転キーが奇数回押された場合は符号反転
			// if (countS % 2 == 1) {
			// 	num *= -1;
			// 	printf("  negl %%ebx\n");  // numの符号を反転
			// }
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			// 一時的にecxは不必要となり，演算終了後に初期化するので，スタックの値を一時的に保持するレジスタとして使う。
			switch (lastOp) {
				case '+':
					// acc += num;
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					// acc -= num;
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// acc *= num;
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					// acc /= num;
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリア todo: 本当に必要？cltd命令で十分では？確認する
					printf("\tcltd\n");				  // idiv命令の前にcltd命令
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}
			// メモリに加算
			// mem += acc + num;
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\taddl %%eax, %%edx\n");
			printf("\taddl %%ebx, %%edx\n");
			// メモリをスタックに戻す
			printf("\tpushq %%rdx\n");

			// 各種変数を初期化
			// num = 0;
			// acc = 0;
			printf("\tmovl $0, %%eax\n");
			printf("\tmovl $0, %%ebx\n");
			lastOp = '+';
			printf("\tmovl $0, %%ecx\n");
		}
		else if (*p == 'M') {
			// 符号反転キーが奇数回押された場合は符号反転
			// if (countS % 2 == 1) {
			// 	num *= -1;
			// 	printf("  negl %%ebx\n");  // numの符号を反転
			// }
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			switch (lastOp) {
				case '+':
					// acc += num;
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					// acc -= num;
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// acc *= num;
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					// acc /= num;
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリア todo: 本当に必要？cltd命令で十分では？確認する
					printf("\tcltd\n");				  // idiv命令の前にcltd命令
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}
			// メモリから減算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\tsubl %%eax, %%edx\n");
			printf("\tsubl %%ebx, %%edx\n");
			// 各種変数を初期化
			// num = 0;
			// acc = 0;
			lastOp = '+';
			// countS = 0;
			printf("\tmovl $0, %%eax\n");
			printf("\tmovl $0, %%ebx\n");
			lastOp = '+';
			printf("\tmovl $0, %%ecx\n");
		}
		else if (*p == 'S') {
			printf("\taddl $1, %%ecx\n");  // countSをインクリメント
		}
		else {
			printf("電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}

	// 計算結果を出力
	printf("\tleaq L_fmt(%%rip), %%rdi\n");	 // フォーマット文字列のアドレスをrdiにロード
	printf("\tmovl %%eax, %%esi\n");		 // 計算結果をesiにロード
	printf("\txorl %%eax, %%eax\n");		 // eaxをクリア
	printf("\tcall _printf\n");

	// プログラムを終了
	printf("\tmovl $0, %%edi\n");  // exitステータス0を設定
	printf("\tcall _exit\n");

	printf("\tleave\n");
	printf("\tret\n");

	return 0;
}
