#!/bin/csh -f

# calc1.cをコンパイルしてデバッグ情報を含む実行ファイルを生成
gcc -g -o calc1 calc1.c

# calc1を実行してアセンブリコードを生成
./calc1 '1=' > foo.s

# 生成されたアセンブリコードをコンパイルして実行ファイルを生成
gcc -g -o b.out foo.s

# 実行ファイルを実行
./b.out