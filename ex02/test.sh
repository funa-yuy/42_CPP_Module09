#!/bin/bash

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TOTAL=0
PASSED=0
FAILED=0

# Print test header
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  PmergeMe Tester${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

# Check if sorted (正しいソート検証)
is_sorted() {
    local output="$1"

    # 配列に変換
    local arr=($output)
    local len=${#arr[@]}

    # 要素が0または1個の場合はソート済み
    if [ $len -le 1 ]; then
        echo "YES"
        return
    fi

    # 各要素を比較
    for ((i=0; i<len-1; i++)); do
        local curr=${arr[i]}
        local next=${arr[i+1]}

        # 数値として比較
        if [ $curr -gt $next ]; then
            echo "NO"
            return
        fi
    done

    echo "YES"
}

# 要素数が一致するかチェック
check_element_count() {
    local input="$1"
    local output="$2"

    local input_arr=($input)
    local output_arr=($output)

    if [ ${#input_arr[@]} -eq ${#output_arr[@]} ]; then
        echo "YES"
    else
        echo "NO"
    fi
}

# Run test
run_test() {
    local test_name="$1"
    shift
    local args="$@"

    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo "Input: $args"

    # プログラム実行
    local full_output=$(./PmergeMe $args 2>&1)
    local exit_code=$?

    # After : の行を抽出
    local after_line=$(echo "$full_output" | grep "^After :")

    if [ -z "$after_line" ]; then
        echo -e "${RED}✗ FAIL${NC}: No 'After :' line found in output"
        echo "Full output:"
        echo "$full_output"
        FAILED=$((FAILED + 1))
        TOTAL=$((TOTAL + 1))
        return
    fi

    # "After :" を除去して数値のみ取得
    local output=$(echo "$after_line" | sed 's/^After ://' | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//')

    echo "Output: $output"

    # 要素数チェック
    local count_match=$(check_element_count "$args" "$output")
    if [ "$count_match" != "YES" ]; then
        echo -e "${RED}✗ FAIL${NC}: Element count mismatch"
        echo "  Input count: $(echo $args | wc -w)"
        echo "  Output count: $(echo $output | wc -w)"
        FAILED=$((FAILED + 1))
        TOTAL=$((TOTAL + 1))
        return
    fi

    # ソート検証
    local sorted=$(is_sorted "$output")

    if [ "$sorted" = "YES" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC}: Output is not sorted"
        FAILED=$((FAILED + 1))
    fi

    TOTAL=$((TOTAL + 1))
}

# Performance test
run_perf_test() {
    local test_name="$1"
    local count=$2
    local range=$3

    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo "Generating $count random numbers from 1-$range..."

    local random_nums=$(shuf -i 1-$range -n $count | tr "\n" " ")

    # 実行時間計測
    local start_time=$(date +%s%N 2>/dev/null || date +%s)
    local full_output=$(./PmergeMe $random_nums 2>&1)
    local end_time=$(date +%s%N 2>/dev/null || date +%s)

    # ナノ秒が使える場合はミリ秒に変換
    if [[ $start_time =~ ^[0-9]{19}$ ]]; then
        local elapsed_ms=$(( (end_time - start_time) / 1000000 ))
        echo "Execution time: ${elapsed_ms}ms"
    fi

    # After : の行を抽出
    local after_line=$(echo "$full_output" | grep "^After :")

    if [ -z "$after_line" ]; then
        echo -e "${RED}✗ FAIL${NC}: No 'After :' line found"
        FAILED=$((FAILED + 1))
        TOTAL=$((TOTAL + 1))
        return
    fi

    local output=$(echo "$after_line" | sed 's/^After ://' | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//')

    # 要素数チェック
    local count_match=$(check_element_count "$random_nums" "$output")
    if [ "$count_match" != "YES" ]; then
        echo -e "${RED}✗ FAIL${NC}: Element count mismatch"
        FAILED=$((FAILED + 1))
        TOTAL=$((TOTAL + 1))
        return
    fi

    # ソート検証
    local sorted=$(is_sorted "$output")

    if [ "$sorted" = "YES" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC}: Output is not sorted correctly"
        FAILED=$((FAILED + 1))
    fi

    TOTAL=$((TOTAL + 1))
}

# Main
print_header

# Build program
echo "Building PmergeMe..."
make re > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed! Trying to continue with existing binary...${NC}"
    if [ ! -f "./PmergeMe" ]; then
        echo -e "${RED}No executable found. Exiting.${NC}"
        exit 1
    fi
else
    echo -e "${GREEN}Build successful${NC}\n"
fi

# Basic tests
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Basic Tests${NC}"
echo -e "${BLUE}========================================${NC}"

run_test "Simple 3 numbers" 3 2 1
run_test "Simple 5 numbers" 5 4 3 2 1
run_test "Already sorted" 1 2 3 4 5
run_test "Reverse sorted" 10 9 8 7 6 5 4 3 2 1

# Edge cases
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Edge Cases${NC}"
echo -e "${BLUE}========================================${NC}"

run_test "Two numbers" 2 1
run_test "Single number" 42
run_test "Duplicates" 3 1 2 1 3
run_test "Large gap" 1 1000 2 999 3

# Specific test cases
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Problem Cases${NC}"
echo -e "${BLUE}========================================${NC}"

run_test "Example from subject" 3 5 9 7 4
run_test "Problem case 1" 34 5 6 1 4 2 100 39 0 3
run_test "Problem case 2" 7 3 5 4 9 0

# Even/odd tests
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Even/Odd Count Tests${NC}"
echo -e "${BLUE}========================================${NC}"

run_test "Even count (10)" 10 9 8 7 6 5 4 3 2 1
run_test "Odd count (11)" 11 10 9 8 7 6 5 4 3 2 1

# Large numbers
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Large Value Tests${NC}"
echo -e "${BLUE}========================================${NC}"

run_test "Large values" 2147483647 1 2147483646 2

# Performance tests
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Performance Tests${NC}"
echo -e "${BLUE}========================================${NC}"

run_perf_test "100 random numbers" 100 10000
run_perf_test "1000 random numbers" 1000 100000
run_perf_test "3000 random numbers" 3000 100000

# Print summary
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Test Summary${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total : $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
