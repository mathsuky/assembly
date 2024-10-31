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
	printf("\tmovl $0, %%eax\n");  // accを初期化
	printf("\tmovl $0, %%ecx\n");  // numを初期化
	printf("\tpushq $0\n");		   // countSを初期化， countSはスタックで管理する
	printf("\tpushq $0\n");		   // memを初期化， memはスタックで管理する

	while (*p) {
		if (*p >= '0' && *p <= '9') {
			// 元ある数値に10を掛けて，新しい数値を加えることで数値の入力を実現
			printf("\tmovl %%ecx, %%edx\n");
			printf("\tsall $3, %%ecx\n");
			printf("\tsall $1, %%edx\n");
			printf("\taddl %%edx, %%ecx\n");

			printf("\taddl $%d, %%ecx\n", (*p - '0'));
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=') {
			// 演算子が連続している場合を考え、最後の演算子までポインタを進める
			while (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '*' || *(p + 1) == '/' || *(p + 1) == '=') {
				p++;
			}
			// // imull,idvを排除した関係で，掛け算の符号処理は別の方法で行う
			// if (lastOp != '*') {
			// 	// 符号反転キーが奇数回押された場合は符号反転
			// 	printf("\t# 符号反転の処理\n");
			// 	printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			// 	printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			// 	printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			// 	printf("\tnegl %%ecx\n");			  // numの符号を反転
			// 	printf("1:\n");
			// }

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\taddl %%ecx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\tsubl %%ecx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// 掛け算をビットシフトとキャリーフラグを用いて実行
					// accの符号をチェック
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("1:\n");
					printf("\tmovl $0, %%edx\n");  // %%edxを初期化（累積値用）
					printf("2:\n");
					printf("\trcrl $1, %%ecx\n");	  // %%ecxの最下位ビットをCFへ移動
					printf("\tjnc 3f\n");			  // キャリーフラグがクリアなら加算をスキップ
					printf("\taddl %%eax, %%edx\n");  // キャリーフラグがセットされている場合、%%edxに%%eaxを加算

					printf("3:\n");
					printf("\tshll $1, %%eax\n");	   // %%eaxを左シフト（次の桁へ移動）
					printf("\ttestl %%ecx, %%ecx\n");  // %%ecxが0かチェック
					printf("\tjnz 2b\n");			   // %%ecxが0でなければ再度2ラベルへ
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%edx\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%edx, %%eax\n");  // 最終結果を%%eaxに格納
					break;
				case '/':
					// 割られる数 $eax, 割られる数を1ビットずつ写すためのレジスタ　$edi, 割る数 edx, 割り算の結果を格納するレジスタ $esi
					// 除算 (減算とビットシフト)
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("\tmovl $0, %%edx\n");
					printf("1:\n");
					printf("\tmovl $0, %%edi\n");
					printf("\tmovl %%ecx, %%edx\n");
					printf("\tmovl $32, %%ecx\n");
					printf("\tmovl $0, %%esi\n");

					printf("2:\n");
					printf("\tshll $1, %%eax\n");
					printf("\trcll $1, %%edi\n");
					printf("\tshll $1, %%esi\n");
					printf("\tcmpl %%edx, %%edi\n");
					printf("\tjl 3f\n");  // 被除数が小さい場合はシフト
					printf("\taddl $1, %%esi\n");
					printf("\tsubl %%edx, %%edi\n");  // 被除数から除数を減算

					printf("3:\n");
					printf("\tdecl %%ecx\n");
					printf("\ttestl %%ecx, %%ecx\n");
					printf("\tjnz 2b\n");
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%esi\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%esi, %%eax\n");
					break;
			}

			// 演算子を更新
			lastOp = *p;
			// 数値を初期化
			printf("\tmovl $0, %%ecx\n");	   // numを初期化
			printf("\tmovl $0, -8(%%rbp)\n");  // countSを初期化
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
			// printf("\t# 符号反転の処理\n");
			// printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			// printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			// printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			// printf("\tnegl %%ecx\n");			  // numの符号を反転
			// printf("1:\n");

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\taddl %%ecx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\tsubl %%ecx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// 掛け算をビットシフトとキャリーフラグを用いて実行
					// accの符号をチェック
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("1:\n");
					printf("\tmovl $0, %%edx\n");  // %%edxを初期化（累積値用）
					printf("2:\n");
					printf("\trcrl $1, %%ecx\n");	  // %%ecxの最下位ビットをCFへ移動
					printf("\tjnc 3f\n");			  // キャリーフラグがクリアなら加算をスキップ
					printf("\taddl %%eax, %%edx\n");  // キャリーフラグがセットされている場合、%%edxに%%eaxを加算

					printf("3:\n");
					printf("\tshll $1, %%eax\n");	   // %%eaxを左シフト（次の桁へ移動）
					printf("\ttestl %%ecx, %%ecx\n");  // %%ecxが0かチェック
					printf("\tjnz 2b\n");			   // %%ecxが0でなければ再度2ラベルへ
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%edx\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%edx, %%eax\n");  // 最終結果を%%eaxに格納
					break;
				case '/':
					// 割られる数 $eax, 割られる数を1ビットずつ写すためのレジスタ　$edi, 割る数 edx, 割り算の結果を格納するレジスタ $esi
					// 除算 (減算とビットシフト)
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("\tmovl $0, %%edx\n");
					printf("1:\n");
					printf("\tmovl $0, %%edi\n");
					printf("\tmovl %%ecx, %%edx\n");
					printf("\tmovl $32, %%ecx\n");
					printf("\tmovl $0, %%esi\n");

					printf("2:\n");
					printf("\tshll $1, %%eax\n");
					printf("\trcll $1, %%edi\n");
					printf("\tshll $1, %%esi\n");
					printf("\tcmpl %%edx, %%edi\n");
					printf("\tjl 3f\n");  // 被除数が小さい場合はシフト
					printf("\taddl $1, %%esi\n");
					printf("\tsubl %%edx, %%edi\n");  // 被除数から除数を減算

					printf("3:\n");
					printf("\tdecl %%ecx\n");
					printf("\ttestl %%ecx, %%ecx\n");
					printf("\tjnz 2b\n");
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%esi\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%esi, %%eax\n");
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
			printf("\tmovl $0, %%ecx\n");
			printf("\tmovl $0, -8(%%rbp)\n");
			;
		}
		else if (*p == 'M') {
			printf("\t# メモリ減算\n");
			// // 符号反転キーが奇数回押された場合は符号反転
			// printf("\t# 符号反転の処理\n");
			// printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
			// printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
			// printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
			// printf("\tnegl %%ecx\n");			  // numの符号を反転
			// printf("1:\n");

			// 演算子に基づいて計算
			printf("\t# 演算キー処理\n");
			switch (lastOp) {
				case '+':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\taddl %%ecx, %%eax\n");  // accにnumを加算
					break;
				case '-':
					printf("\t# 符号反転の処理\n");
					printf("\tmovq -8(%%rbp), %%rdx\n");  // countSをrdxにロード
					printf("\ttestb $1, %%dl\n");		  // countSが2で割り切れるかチェック
					printf("\tjz 1f\n");				  // countSが2で割り切れるなら次の命令をスキップ
					printf("\tnegl %%ecx\n");			  // numの符号を反転
					printf("1:\n");
					printf("\tsubl %%ecx, %%eax\n");  // accからnumを減算
					break;
				case '*':
					// 掛け算をビットシフトとキャリーフラグを用いて実行
					// accの符号をチェック
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("1:\n");
					printf("\tmovl $0, %%edx\n");  // %%edxを初期化（累積値用）
					printf("2:\n");
					printf("\trcrl $1, %%ecx\n");	  // %%ecxの最下位ビットをCFへ移動
					printf("\tjnc 3f\n");			  // キャリーフラグがクリアなら加算をスキップ
					printf("\taddl %%eax, %%edx\n");  // キャリーフラグがセットされている場合、%%edxに%%eaxを加算

					printf("3:\n");
					printf("\tshll $1, %%eax\n");	   // %%eaxを左シフト（次の桁へ移動）
					printf("\ttestl %%ecx, %%ecx\n");  // %%ecxが0かチェック
					printf("\tjnz 2b\n");			   // %%ecxが0でなければ再度2ラベルへ
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%edx\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%edx, %%eax\n");  // 最終結果を%%eaxに格納
					break;
				case '/':
					// 割られる数 $eax, 割られる数を1ビットずつ写すためのレジスタ　$edi, 割る数 edx, 割り算の結果を格納するレジスタ $esi
					// 除算 (減算とビットシフト)
					printf("\t# 符号反転の処理\n");
					printf("\ttestl %%eax, %%eax\n");
					printf("\tjns 1f\n");
					printf("\tnegl %%eax\n");  // accを正にする
					// 代わりにcountSをインクリメントする
					printf("\tmovq -8(%%rbp), %%rdx\n");
					printf("\taddq $1, %%rdx\n");
					printf("\tmovq %%rdx, -8(%%rbp)\n");
					printf("\tmovl $0, %%edx\n");
					printf("1:\n");
					printf("\tmovl $0, %%edi\n");
					printf("\tmovl %%ecx, %%edx\n");
					printf("\tmovl $32, %%ecx\n");
					printf("\tmovl $0, %%esi\n");

					printf("2:\n");
					printf("\tshll $1, %%eax\n");
					printf("\trcll $1, %%edi\n");
					printf("\tshll $1, %%esi\n");
					printf("\tcmpl %%edx, %%edi\n");
					printf("\tjl 3f\n");  // 被除数が小さい場合はシフト
					printf("\taddl $1, %%esi\n");
					printf("\tsubl %%edx, %%edi\n");  // 被除数から除数を減算

					printf("3:\n");
					printf("\tdecl %%ecx\n");
					printf("\ttestl %%ecx, %%ecx\n");
					printf("\tjnz 2b\n");
					// もうecxは使わないので，ecxにcountSの値を格納
					printf("\tmovq -8(%%rbp), %%rcx\n");
					printf("\ttestb $1, %%cl\n");
					printf("\tjz 4f\n");
					printf("\tnegl %%esi\n");  // accの正負とcountSの正負をまとめて処理する
					printf("4:\n");
					printf("\tmovl %%esi, %%eax\n");
					break;
			}
			// メモリから減算
			printf("\tpopq %%rdx\n");  // スタックからメモリを取り出す
			printf("\tsubl %%eax, %%edx\n");
			printf("\tpushq %%rdx\n");	// メモリをスタックに戻す
			// 各種変数を初期化
			lastOp = '+';
			printf("\tmovl $0, %%eax\n");
			printf("\tmovl $0, %%ecx\n");
			printf("\tmovl $0, -8(%%rbp)\n");
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
	printf("\t# 16バイト境界制約の確認\n");
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

	return 0;
}
