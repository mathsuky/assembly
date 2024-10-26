#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	// 引数が2つでない場合はエラー
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <calculation expression>\n", argv[0]);
		return 1;
	}

	// 入力された要素を保持する変数
	// p: 入力文字列のポインタ, lastOp: 最後に入力された演算子
	char *p = argv[1], lastOp = '+';

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
	// num: 数値, acc: 累積値, mem: 電卓のメモリ機能に格納された値, countS: 符号反転キーのカウント としてコメントを書く。
	printf("\tmovl $0, %%eax\n");  // accを初期化, accは$eaxレジスタに格納
	printf("\tmovl $0, %%ebx\n");  // numを初期化, numは$ebxレジスタに格納
	printf("\tmovl $0, %%ecx\n");  // countSを初期化, countSは$ecxレジスタに格納
	printf("\tpushq $0\n");		   // memを初期化， memはスタックで管理する
	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 数字の場合は数値を構築
			// 元ある数値に10を掛けて，新しい数値を加えることで数値の入力を実現
			printf("\timull $10, %%ebx, %%ebx\n");		// ebxに10を掛ける
			printf("\taddl $%d, %%ebx\n", (*p - '0'));	// ebxに対応する数値を加える
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}
			// 符号反転キーが奇数回押された場合は符号反転
			printf("\t# 符号反転の処理\n");
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリア
					printf("\tcltd\n");				  // idiv命令の前にcltd命令
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}

			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			printf("\tmovl $0, %%ebx\n");  // numを初期化
			printf("\tmovl $0, %%ecx\n");  // countSを初期化;
		}
		else if (*p == 'C') {
			// メモリをクリア
			printf("\t# メモリクリア\n");
			printf("\taddq $8, %%rsp\n");  // スタックポインタを8バイト進めて，積まれていたmemを破棄
			printf("\tpushq $0\n");		   // スタックに新たなmemとして0をプッシュ
		}
		else if (*p == 'R') {
			// メモリから読み込み
			printf("\t# メモリ読み込み\n");
			printf("\tpopq %%rdx\n");		  // スタックからメモリを取り出す
			printf("\tmovl %%edx, %%eax\n");  // メモリをaccにロード
			// スタックにメモリを戻す
			printf("\tpushq %%rdx\n");
		}
		else if (*p == 'P') {
			printf("\t# メモリ加算\n");
			// 符号反転キーが奇数回押された場合は符号反転
			printf("\t# 符号反転の処理\n");
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリア
					printf("\tcltd\n");				  // idiv命令の前にcltd命令
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}
			// メモリに加算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\taddl %%eax, %%edx\n");
			// メモリをスタックに戻す
			printf("\tpushq %%rdx\n");

			// 各種変数を初期化
			lastOp = '+';
			printf("\tmovl $0, %%eax\n");
			printf("\tmovl $0, %%ebx\n");
			printf("\tmovl $0, %%ecx\n");
		}
		else if (*p == 'M') {
			printf("\t# メモリ減算\n");
			// 符号反転キーが奇数回押された場合は符号反転
			printf("\t# 符号反転の処理\n");
			printf("\ttestb $1, %%cl\n");  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");		   // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ebx\n");	   // numの符号を反転
			printf("1:\n");
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ebx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\tsubl %%ebx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					printf("\timull %%ebx, %%eax\n");  // accとnumを乗算
					break;
				case '/':
					printf("\txorl %%edx, %%edx\n");  // 除算の前にedxをクリアしておく
					printf("\tcltd\n");				  // 符号拡張を行う
					printf("\tidivl %%ebx\n");		  // accをnumで除算
					break;
			}
			// メモリから減算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\tsubl %%eax, %%edx\n");
			printf("\tpushq %%rdx\n");	// メモリをスタックに戻す
			// 各種変数を初期化
			lastOp = '+';
			printf("\tmovl $0, %%eax\n");
			printf("\tmovl $0, %%ebx\n");
			printf("\tmovl $0, %%ecx\n");
		}
		else if (*p == 'S') {
			printf("\taddl $1, %%ecx\n");  // countSをインクリメント
		}
		else {
			printf("\t# 電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}

	// 計算結果を出力
	printf("\tleaq L_fmt(%%rip), %%rdi\n");
	printf("\tmovl %%eax, %%esi\n");
	printf("\txorl %%eax, %%eax\n");
	printf("\tcall _printf\n");

	// プログラムを終了
	printf("\tmovl $0, %%edi\n");  // exitステータス0を設定
	printf("\tcall _exit\n");

	printf("\tleave\n");
	printf("\tret\n");

	return 0;
}
