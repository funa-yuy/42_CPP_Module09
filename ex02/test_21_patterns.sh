#!/bin/bash

# 色の定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# カウンター
TOTAL=0
PASSED=0
FAILED=0

# ビルド（既にバイナリがある場合はスキップ）
if [ ! -f "./PmergeMe" ]; then
    echo -e "${BLUE}Building PmergeMe...${NC}"
    make re > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}Build successful!${NC}\n"
else
    echo -e "${GREEN}Using existing PmergeMe binary${NC}\n"
fi

# ソート確認関数
is_sorted() {
    local arr=($@)
    for ((i=0; i<${#arr[@]}-1; i++)); do
        if [ ${arr[i]} -gt ${arr[i+1]} ]; then
            return 1
        fi
    done
    return 0
}

# テスト実行関数
run_test() {
    local name="$1"
    shift
    local input="$@"

    TOTAL=$((TOTAL + 1))
    echo -e "${BLUE}Test $TOTAL: $name${NC}"
    echo -e "Input: $input"

    # プログラム実行
    output=$(./PmergeMe $input 2>&1)
    exit_code=$?

    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}✗ Failed (exit code: $exit_code)${NC}\n"
        FAILED=$((FAILED + 1))
        return
    fi

    # After行を抽出
    after_line=$(echo "$output" | grep "^After" | sed 's/After ://')
    if [ -z "$after_line" ]; then
        echo -e "${RED}✗ Failed (no output)${NC}\n"
        FAILED=$((FAILED + 1))
        return
    fi

    # ソート確認
    sorted_array=($after_line)
    if is_sorted "${sorted_array[@]}"; then
        # 比較回数を抽出
        compare_count=$(echo "$output" | grep "std::vector" | grep -o "比較回数: [0-9]*" | grep -o "[0-9]*")
        echo -e "${GREEN}✓ Passed (比較回数: $compare_count)${NC}"
        echo -e "Output: $after_line\n"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Failed (not sorted)${NC}"
        echo -e "Output: $after_line\n"
        FAILED=$((FAILED + 1))
    fi
}

echo -e "${YELLOW}=== Testing 21 Elements Patterns ===${NC}\n"

# 1. 昇順（最良ケース）
run_test "Ascending Order" 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21

# 2. 降順（最悪ケース）
run_test "Descending Order" 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1

# 3. ランダム1（与えられた例）
run_test "Random Pattern 1" 21 1 13 2 17 3 12 4 20 5 15 6 19 7 14 8 18 9 16 10 11

# 4. ランダム2（逆パターン）
run_test "Random Pattern 2" 11 10 16 9 18 8 14 7 19 6 15 5 20 4 12 3 17 2 13 1 21

# 5. 前半昇順・後半降順
run_test "Asc-Desc Split" 1 2 3 4 5 6 7 8 9 10 11 21 20 19 18 17 16 15 14 13 12

# 6. 前半降順・後半昇順
run_test "Desc-Asc Split" 11 10 9 8 7 6 5 4 3 2 1 12 13 14 15 16 17 18 19 20 21

# 7. 奇数位置と偶数位置で交互
run_test "Odd-Even Swap" 2 1 4 3 6 5 8 7 10 9 12 11 14 13 16 15 18 17 20 19 21

# 8. 中央値を中心に配置
run_test "Median Center" 11 1 21 2 20 3 19 4 18 5 17 6 16 7 15 8 14 9 13 10 12

# 9. 3要素ずつのブロック逆順
run_test "Block Reversed" 3 2 1 6 5 4 9 8 7 12 11 10 15 14 13 18 17 16 21 20 19

# 10. ほぼソート済み（2つだけ逆）
run_test "Almost Sorted (2 swaps)" 1 2 3 4 5 6 7 8 9 10 12 11 13 14 15 16 17 18 19 21 20

# 11. ほぼソート済み（先頭と末尾が逆）
run_test "Almost Sorted (ends)" 21 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1

# 12. 5要素ブロックのシャッフル
run_test "5-Block Shuffle" 5 4 3 2 1 10 9 8 7 6 15 14 13 12 11 20 19 18 17 16 21

# 13. 山型（中央が最大）
run_test "Mountain Pattern" 1 3 5 7 9 11 13 15 17 19 21 20 18 16 14 12 10 8 6 4 2

# 14. 谷型（中央が最小）
run_test "Valley Pattern" 21 19 17 15 13 11 9 7 5 3 1 2 4 6 8 10 12 14 16 18 20

# 15. ジグザグ1
run_test "Zigzag 1" 1 21 2 20 3 19 4 18 5 17 6 16 7 15 8 14 9 13 10 12 11

# 16. ジグザグ2
run_test "Zigzag 2" 21 1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11

# 17. 回転1（右に5シフト）
run_test "Rotate Right 5" 17 18 19 20 21 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16

# 18. 回転2（左に5シフト）
run_test "Rotate Left 5" 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 1 2 3 4 5

# 19. 前半と後半を交換
run_test "Half Swap" 12 13 14 15 16 17 18 19 20 21 1 2 3 4 5 6 7 8 9 10 11

# 20. ランダムパターン3
run_test "Random Pattern 3" 7 14 3 18 11 2 19 6 15 1 21 10 5 17 9 4 20 8 13 12 16

# 21. ランダムパターン4
run_test "Random Pattern 4" 13 7 19 2 16 11 4 21 9 15 3 18 6 12 1 20 8 14 5 17 10

# 22. ランダムパターン5
run_test "Random Pattern 5" 10 17 4 14 21 7 2 18 13 5 19 1 16 9 6 20 12 3 15 8 11

# 23. 小さい数と大きい数を交互に
run_test "Small-Large Alternating" 1 21 2 20 3 19 4 18 5 17 6 16 7 15 8 14 9 13 10 12 11

# 24. 3の倍数を最初、残りを後ろ
run_test "Multiples of 3 First" 3 6 9 12 15 18 21 1 2 4 5 7 8 10 11 13 14 16 17 19 20

# 25. 素数を最初、合成数を後ろ
run_test "Primes First" 2 3 5 7 11 13 17 19 1 4 6 8 9 10 12 14 15 16 18 20 21

echo -e "\n${YELLOW}=== Test Summary ===${NC}"
echo -e "Total tests: ${TOTAL}"
echo -e "${GREEN}Passed: ${PASSED}${NC}"
echo -e "${RED}Failed: ${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed!${NC}"
    exit 1
fi

