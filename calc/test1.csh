#!/bin/csh -f
set noglob
set num = 0
set bad = 0

# calc0.cを厳しいオプションでコンパイル
gcc -Wall -Wextra -Werror -pedantic -o a.out calc1.c

# テストケースを処理
foreach line (`cat testcase1.txt`)
    @ num++
    set x = `echo $line | awk 'BEGIN{FS=","}{print $1, $2}'`
    ./a.out $x[1] > foo.s
    gcc -Wall -Wextra -Werror -pedantic -Wl,-no_pie -o b.out foo.s
    # gcc -o b.out foo.s
    set result=`./b.out`
    if ($result != $x[2]) then
        echo $x[1], $x[2], bad result\!\! $result
        @ bad++
    endif
end

echo num = $num, bad = $bad
unset noglob