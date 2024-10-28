#!/bin/csh -f

# calc1.cをコンパイルしてデバッグ情報を含む実行ファイルを生成
gcc -g -o calc3 calc3.c

# calc1を実行してアセンブリコードを生成
./calc3 '3*4*5=' > foo.s

# 生成されたアセンブリコードをコンパイルして実行ファイルを生成
gcc -g -o b.out foo.s

# 実行ファイルを実行
./b.out

# 2147483648
# 4000000001