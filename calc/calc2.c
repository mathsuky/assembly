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
		"L_err:\n"
		"\t.ascii \"E\\n\\0\"\n"  // エラー表示用のフォーマットを追加
		".text\n"
		".globl _main\n"
		".extern _exit\n"
		"_main:\n"
		"\tpushq %%rbp\n"
		"\tmovq %%rsp, %%rbp\n");
	// num: 数値, acc: 累積値, mem: 電卓のメモリ機能に格納された値, countS: 符号反転キーのカウント としてコメントを書く。
	printf("\txorl %%eax, %%eax\n");  // accを初期化
	printf("\txorl %%ecx, %%ecx\n");  // numを初期化
	printf("\tpushq $0\n");		   // countSを初期化， countSはスタックで管理する
	printf("\tpushq $0\n");		   // memを初期化， memはスタックで管理す

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 元ある数値に10を掛けて，新しい数値を加えることで数値の入力を実現
			printf("\timull $10, %%ecx, %%ecx\n");
			printf("\taddl $%d, %%ecx\n", (*p - '0'));
			printf("\tjo overflow\n");
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}

			// 符号反転キーが奇数回押された場合は符号反転
			printf("\t# 符号反転の処理\n");
			printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ecx\n");
			printf("1:\n");

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '-':
					printf("\tsubl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '*':
					printf("\timull %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '/':
					printf("\tcmpl $0, %%ecx\n");		// 除数が0でないかを確認
					printf("\tje division_by_zero\n");	// ゼロ割り算の処理
					printf("\tcltd\n");					// 符号拡張を行う
					printf("\tidivl %%ecx\n");
					break;
			}

			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			printf("\txorl %%ecx, %%ecx\n");	   // numを初期化
			printf("\tmovq $0, -8(%%rbp)\n");  // countSを初期化
		}
		else if (*p == 'C') {
			printf("\t# メモリクリア\n");
			printf("\taddq $8, %%rsp\n");  // スタックポインタを8バイト進めて，積まれていたmemを破棄
			printf("\tpushq $0\n");		   // スタックに新たなmemとして0をプッシュ
		}
		else if (*p == 'R') {
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
			printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ecx\n");
			printf("1:\n");

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '-':
					printf("\tsubl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '*':
					printf("\timull %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '/':
					printf("\tcmpl $0, %%ecx\n");		// 除数が0でないかを確認
					printf("\tje division_by_zero\n");	// ゼロ割り算の処理
					printf("\tcltd\n");					// 符号拡張を行う
					printf("\tidivl %%ecx\n");
					break;
			}

			// メモリに加算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\taddl %%eax, %%edx\n");
			printf("\tjo overflow\n");	// オーバーフローをチェック
			// メモリをスタックに戻す
			printf("\tpushq %%rdx\n");

			// 各種変数を初期化
			lastOp = '+';
			printf("\txorl %%eax, %%eax\n");
			printf("\txorl %%ecx, %%ecx\n");
			printf("\tmovq $0, -8(%%rbp)\n");
		}
		else if (*p == 'M') {
			printf("\t# メモリ減算\n");

			// 符号反転キーが奇数回押された場合は符号反転
			printf("\t# 符号反転の処理\n");
			printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			printf("\tnegl %%ecx\n");
			printf("1:\n");

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\taddl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '-':
					printf("\tsubl %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '*':
					printf("\timull %%ecx, %%eax\n");
					printf("\tjo overflow\n");	// オーバーフローをチェック
					break;
				case '/':
					printf("\tcmpl $0, %%ecx\n");		// 除数が0でないかを確認
					printf("\tje division_by_zero\n");	// ゼロ割り算の処理
					printf("\tcltd\n");					// 符号拡張を行う
					printf("\tidivl %%ecx\n");
					break;
			}

			// メモリから減算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\tsubl %%eax, %%edx\n");
			printf("\tjo overflow\n");	// オーバーフローをチェック
			printf("\tpushq %%rdx\n");	// メモリをスタックに戻す

			// 各種変数を初期化
			lastOp = '+';
			printf("\txorl %%eax, %%eax\n");
			printf("\txorl %%ecx, %%ecx\n");
			printf("\tmovq $0, -8(%%rbp)\n");
		}
		else if (*p == 'S') {
			printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			printf("\taddq $1, %%rdx\n");		  // countSをインクリメント
			printf("\tmovq %%rdx, -8(%%rbp)\n");  // countSをスタックに戻す
		}
		else {
			printf("\t# 電卓に存在しない文字%cが入力されました。この入力は無視されます。\n", *p);
		}
		p++;
	}
	// スタックに残っている値(memとcountS)を捨てる
	printf("\taddq $16, %%rsp\n");

	// 16バイト境界制約の確認
	printf("\tmovq %%rsp, %%rcx\n");
	printf("\tandq $0xF, %%rcx\n");	 // スタックポインタの下位4ビットを取り出す
	printf("\tcmpq $0x0, %%rcx\n");	 // 下位4ビットが0かどうかを確認
	printf("\tje end\n");

	// 16の倍数でなければ最下位ビットを0にする
	printf("\tandq $0xFFFFFFFFFFFFFFF0, %%rsp\n");

	// 計算結果を出力
	printf("end:\n");
	printf("\tleaq L_fmt(%%rip), %%rdi\n");
	printf("\tmovl %%eax, %%esi\n");
	printf("\txorl %%eax, %%eax\n");
	printf("\tcall _printf\n");

	// プログラムを終了
	printf("\tmovl $0, %%edi\n");  // exitステータス0を設定
	printf("\tcall _exit\n");

	printf("\tleave\n");
	printf("\tret\n");

	// オーバーフロー処理
	printf("overflow:\n");
	printf("\tleaq L_err(%%rip), %%rdi\n");	 // エラーメッセージのアドレスをセット
	printf("\tcall _printf\n");
	printf("\tmovl $1, %%edi\n");  // exitステータス1を設定
	printf("\tcall _exit\n");

	// ゼロ割り算処理
	printf("division_by_zero:\n");
	printf("\tleaq L_err(%%rip), %%rdi\n");	 // エラーメッセージのアドレスをセット
	printf("\tcall _printf\n");
	printf("\tmovl $1, %%edi\n");  // exitステータス1を設定
	printf("\tcall _exit\n");
}
