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
		"\t.ascii \"E\\n\\0\"\n"  // エラー表示用のフォーマット
		".text\n"
		".globl _main\n"
		".extern _exit\n"
		"_main:\n"
		"\tpushq %%rbp\n"
		"\tmovq %%rsp, %%rbp\n");

	printf(
		"calc_with_sign_and_op:\n"
		"\t# 符号反転の処理\n"
		"\ttestb $1, %%cl\n"  // countSが奇数か確認
		"\tjz skip_negate\n"  // 偶数の場合は符号反転をスキップ
		"\tnegl %%ebx\n"	  // numの符号を反転
		"skip_negate:\n"
		"\t# 演算キー処理\n"
		"\tmovb lastOp, %%dl\n"	 // lastOpをdlにロード
		"\tcmpb $'+', lastOp\n"	 // lastOpが'+'の場合
		"\tje do_add\n"
		"\tcmpb $'-', lastOp\n"	 // lastOpが'-'の場合
		"\tje do_sub\n"
		"\tcmpb $'*', lastOp\n"	 // lastOpが'*'の場合
		"\tje do_mul\n"
		"\tcmpb $'/', lastOp\n"	 // lastOpが'/'の場合
		"\tje do_div\n"
		"\tret\n"  // 該当しない場合は終了
		"do_add:\n"
		"\taddl %%ebx, %%eax\n"	 // accにnumを加算
		"\tjo overflow\n"		 // オーバーフローをチェック
		"\tret\n"
		"do_sub:\n"
		"\tsubl %%ebx, %%eax\n"	 // accからnumを減算
		"\tjo overflow\n"
		"\tret\n"
		"do_mul:\n"
		"\timull %%ebx, %%eax\n"  // accとnumを乗算
		"\tjo overflow\n"
		"\tret\n"
		"do_div:\n"
		"\tcmpl $0, %%ebx\n"	   // 除数が0でないか確認
		"\tje division_by_zero\n"  // ゼロ割り算エラー
		"\txorl %%edx, %%edx\n"	   // edxをクリア
		"\tcltd\n"				   // idiv前にcltd命令
		"\tidivl %%ebx\n"		   // accをnumで除算
		"\tret\n");
	// num: 数値, acc: 累積値, mem: 電卓のメモリ機能に格納された値, countS: 符号反転キーのカウント としてコメントを書く。
	printf("\tmovl $0, %%eax\n");  // accを初期化
	printf("\tmovl $0, %%ebx\n");  // numを初期化
	printf("\tmovl $0, %%ecx\n");  // countSを初期化
	printf("\tpushq $0\n");		   // memを初期化， memはスタックで管理する

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 元ある数値に10を掛けて，新しい数値を加えることで数値の入力を実現
			printf("\timull $10, %%ebx, %%ebx\n");		// ebxに10を掛ける
			printf("\taddl $%d, %%ebx\n", (*p - '0'));	// ebxに対応する数値を加える
			printf("\tjo overflow\n");
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}

			printf("\tcall calc_with_sign_and_op\n");
			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			printf("\tmovl $0, %%ebx\n");  // numを初期化
			printf("\tmovl $0, %%ecx\n");  // countSを初期化;
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

			printf("\tcall calc_with_sign_and_op\n");

			// メモリに加算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\taddl %%eax, %%edx\n");
			printf("\tjo overflow\n");	// オーバーフローをチェック
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

			printf("\tcall calc_with_sign_and_op\n");

			// メモリから減算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\tsubl %%eax, %%edx\n");
			printf("\tjo overflow\n");	// オーバーフローをチェック
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

	// 16バイト境界制約の確認
	printf("\tmovq %%rsp, %%rbx\n");
	printf("\tandq $0xF, %%rbx\n");	 // スタックポインタの下位4ビットを取り出す
	printf("\tcmpq $0x0, %%rbx\n");	 // 下位4ビットが0かどうかを確認
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
