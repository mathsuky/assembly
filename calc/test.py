import random

# 32ビット整数の範囲
INT_MIN = -2147483648
INT_MAX = 2147483647

# 約数を取得する関数


def get_divisors(n):
    if n == 0:
        return [1]
    n = abs(n)
    divisors = []
    for i in range(1, n + 1):
        if n % i == 0:
            divisors.append(i)
    return divisors

# 計算を実行する関数


def calculate_expression(expression):
    try:
        # 符号反転 (Sキー) の処理
        while 'S' in expression:
            idx = expression.index('S')
            # 数字の終わりの位置
            num_end = idx
            # 数字の開始位置を探す
            num_start = idx - 1
            while num_start >= 0 and expression[num_start].isdigit():
                num_start -= 1
            num_start += 1  # 実際の数字の開始位置に調整
            num = int(expression[num_start:num_end])
            # 符号反転して置き換え
            expression = expression[:num_start] + \
                str(-num) + expression[idx + 1:]

        # トークンに分解
        tokens = []
        i = 0
        while i < len(expression):
            if expression[i].isdigit() or (expression[i] == '-' and i + 1 < len(expression) and expression[i + 1].isdigit()):
                num_start = i
                i += 1
                while i < len(expression) and expression[i].isdigit():
                    i += 1
                tokens.append(expression[num_start:i])
            elif expression[i] in '+-*/':
                tokens.append(expression[i])
                i += 1
            else:
                # 無効な文字があればエラー
                return ''
        # 逐次計算
        acc = int(tokens[0])
        i = 1
        while i < len(tokens):
            op = tokens[i]
            i += 1
            num = int(tokens[i])
            i += 1
            if op == '+':
                acc += num
            elif op == '-':
                acc -= num
            elif op == '*':
                acc *= num
            elif op == '/':
                if num == 0:
                    return 'E'
                acc //= num
            else:
                return ''
            if acc < INT_MIN or acc > INT_MAX:
                return 'E'
        return str(acc)
    except Exception:
        return ''

# 式を生成する関数


def generate_expression():
    operators = ['+', '-', '*', '/']
    elements = []

    # 最初の数値を生成
    number = str(random.randint(1, 100))
    acc = int(number)
    elements.append(number)

    num_count = random.randint(3, 5)
    for _ in range(num_count - 1):
        op = random.choice(operators)
        elements.append(op)
        if op == '/':
            divisors = get_divisors(acc)
            if len(divisors) <= 1:
                # 除算できないので他の演算子に変更
                op = random.choice(['+', '-', '*'])
                elements[-1] = op
            else:
                num = random.choice(divisors[1:])  # 1以外の約数
                if random.random() < 0.3:
                    next_num = str(num) + 'S'
                    num = -num
                else:
                    next_num = str(num)
                elements.append(next_num)
                if num == 0:
                    elements.pop()
                    elements.pop()
                    continue
                acc //= num
                continue
        else:
            # 他の演算子の場合
            if random.random() < 0.3:
                next_num = str(random.randint(1, 100)) + 'S'
                num = -int(next_num[:-1])
            else:
                next_num = str(random.randint(1, 100))
                num = int(next_num)
            elements.append(next_num)
            # accを更新
            if op == '+':
                acc += num
            elif op == '-':
                acc -= num
            elif op == '*':
                acc *= num
    return ''.join(elements)


# ファイルに出力
with open("calculator_results.txt", "w") as file:
    for _ in range(50):
        expression = generate_expression()
        result = calculate_expression(expression)
        if result != '':
            file.write(f"{expression}=,{result}\n")

print("calculator_results.txt ファイルが生成されました。")
