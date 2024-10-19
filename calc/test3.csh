#!/bin/csh -f
set noglob
set num = 0
set bad = 0

# calc0.cを厳しいオプションでコンパイル
gcc -Wall -Wextra -Werror -pedantic -o a.out calc0.c

# テストケースを処理
foreach line (`cat testcase1.txt`)
    @ num++
    set x = `echo $line | awk 'BEGIN{FS=","}{print $1, $2}'`
    set result=`./a.out $x[1]`
    if ($result != $x[2]) then
        echo $x[1], $x[2], bad result\!\! $result
        @ bad++
    endif
end

echo num = $num, bad = $bad
unset noglob