.data
msg_mod0: .ascii "mod 0\n\0"
msg_mod1: .ascii "mod 1\n\0"
msg_mod2: .ascii "mod 2\n\0"
hello_str:
    .ascii "Hello, World!\n\0"
L_fmt:
    .ascii "Result: %ld\n\0"     # 出力フォーマット

.text
.globl _main
.extern _printf
.extern _exit

_main:
    # 初期化
    movl $0, %edi           # i = 0

for_loop_start:
    cmpl $10, %edi          # i < 10
    jge end_for_loop        # i >= 10 の場合、ループを終了

    # mod計算（i % 3）
    movl %edi, %eax         # eaxにiを設定
    movl $3, %ebx           # 3で割る
    xorl %edx, %edx         # edxを0にクリア
    divl %ebx               # eax / ebx、余りはedxに格納される
    movl %edx, %esi         # mod = i % 3 の結果をesiに

    # switch文を模倣するジャンプテーブル
    cmpl $2, %esi           # modの範囲チェック
    ja default_case         # 範囲外ならデフォルトケースへ

    jmp *jump_table(, %esi, 4)  # modの値に基づいてジャンプ

case0:
    movl $msg_mod0, %edi    # "mod 0" をediに設定
    call _puts
    jmp end_switch

case1:
    movl $msg_mod1, %edi    # "mod 1" をediに設定
    call _puts
    jmp end_switch

case2:
    movl $msg_mod2, %edi    # "mod 2" をediに設定
    call _puts
    jmp end_switch

default_case:
    # デフォルトの処理 (今回はなし)
    jmp end_switch

end_switch:
    # i++
    incl %edi
    jmp for_loop_start      # ループの最初に戻る

end_for_loop:
    movl $0, %eax
    ret

    # ジャンプテーブル
jump_table:
    .long case0             # mod = 0 の場合のジャンプ先
    .long case1             # mod = 1 の場合のジャンプ先
    .long case2             # mod = 2 の場合のジャンプ先