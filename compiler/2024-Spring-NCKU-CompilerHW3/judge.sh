#!/bin/bash
input_dir="/home/share/hw3/input"
result_dir="/home/$USER/2024-Spring-NCKU-CompilerHW3/result"
answer_dir="/home/share/hw3/answer"
compiler="/home/$USER/2024-Spring-NCKU-CompilerHW3/build/out/compiler"

RED="\033[0;31m"
YELLOW="\033[0;33m"
GREEN="\033[0;32m"
RESET="\033[0m"

show_diff=false
run_only=false
specific_test=""
specific_subtest=""
specific_testcase=""
specific_test_find=false
score=0

usage() {
    echo "Usage: $0 [-d|--diff] [--case=subtask/testcase] [-h|--help]"
    echo "Options:"
    echo "  -d, --diff        Show difference between actual and expected output"
    echo "  --case=subtask/testcase"
    echo "                    Run a specific testcase"
    echo "  -h, --help        Display this usage information"
    echo "  -l, --local       Use local input and answer to judge"
    echo "  -r, --run         Run only, no judge"
}

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        -d|--diff)
            show_diff=true
            shift
        ;;
        --case=*)
            specific_test="${key#*=}"
            specific_subtest=$(dirname $specific_test)
            specific_testcase=$(basename $specific_test)
            if [ $specific_subtest = "." ]; then
                specific_subtest=$specific_testcase
                specific_testcase=""
            fi
            shift
        ;;
        -r|--run)
            run_only=true
            shift
        ;;
        -l|--local)
            input_dir="./input"
            result_dir="./result"
            answer_dir="./answer"
            compiler="./build/out/compiler"
            shift
        ;;
        -h|--help)
            usage
            exit 0
        ;;
        *)
            echo "Unknown option: $key"
            exit 1
        ;;
    esac
done

make build

get_score() {
    local subtask="$1"
    case "$subtask" in
        subtask01-helloworld) return 8 ;;
        subtask02-comment) return 8 ;;
        subtask03-precedenc) return 8 ;;
        subtask04-assigment) return 8 ;;
        subtask05-casting) return 8 ;;
        subtask06-if) return 8 ;;
        subtask07-while) return 8 ;;
        subtask08-for) return 8 ;;
        subtask09-function) return 8 ;;
        subtask10-array) return 8 ;;
        subtask11-autotype) return 8 ;;
        subtask12-loop2) return 10 ;;
        subtask13-2Darray) return 8 ;;
        subtask14-forall) return 14 ;;
        subtask15-bonus) return 5 ;;
    esac
}

run_test() {
    local subtask="$1"
    local testcase="$2"
    local cache_file="$result_dir/../build/Main.j"
    local asm_output_file="$result_dir/$subtask/$testcase.j"
    local output_file="$result_dir/$subtask/$testcase.out"
    local answer_file="$answer_dir/$subtask/$testcase.out"

    # Compile the testcase
    make compile_asm IN="$input_dir/$subtask/$testcase.cpp" > /dev/null
    # cp $cache_file $asm_output_file
    make run_nomsg > $output_file

    if [ $? -eq 0 ]; then
        if [ ! -f "$answer_file" ]; then
            # Check if answer exist
            echo -e "🫠$YELLOW $subtask/$testcase Answer not ready yet$RESET"
        elif [ ! -f "$output_file" ]; then
            # Check if expected output file exists
            echo -e "❌$RED $subtask/$testcase No expected output file found$RESET"
        elif [ "$run_only" = false ]; then
            # Compare the output with expected output
            diff --color -u --strip-trailing-cr "$answer_file" "$output_file"
            if [ $? -eq 0 ]; then
                echo -e "✅$GREEN $testcase output matches expected$RESET"
                return 0
            else
                echo -e "❌$RED '$subtask/$testcase' output does not match expected$RESET"
                if [ "$show_diff" = true ]; then
                    echo "$diff_output"
                fi
            fi
        fi

        if [ "$run_only" = true ]; then
            cat "$output_file"
            return 0
        fi
        return 1
    else
        echo -e "❌$RED $subtask/$testcase exit with code $?$RESET"
        return 2
    fi
}

# Iterate through each test group
for sub_dir in "$input_dir"/*; do
    # Extract subtask name
    subtask=$(basename "$sub_dir")
    mkdir -p "$result_dir/$subtask"

    # Check if specific subtest
    if [ -n "$specific_test" ] && [ "$subtask" != "$specific_subtest" ]; then
        continue
    fi

    echo "########## $subtask ##########"
    pass_subtask=true
    # Iterate through each testcase
    for testcase_file in "$sub_dir"/*.cpp; do
        # Extract testcase filename without extension
        testcase=$(basename -s .cpp "$testcase_file")
        # Check if specific testcase
        if [ -n "$specific_testcase" ] && [ "$testcase" != "$specific_testcase" ]; then
            continue
        fi
        specific_test_find=true
        run_test "$subtask" "$testcase"
        # Check if test pass
        if [ $? -ne 0 ]; then
            pass_subtask=false
        fi
    done
    if [ $pass_subtask = true ]; then
        get_score "$subtask"
        score=$((score+$?))
    fi
done

if [ -n "$specific_test" ] && [ $specific_test_find = false ]; then
    echo "Testcase: '$specific_subtest/$specific_testcase' not found"
    echo "Available: "
    for sub_dir in "$input_dir"/*; do
        subtask=$(basename "$sub_dir")
        for testcase_file in "$sub_dir"/*.cpp; do
            testcase=$(basename -s .cpp "$testcase_file")
            echo "$subtask/$testcase"
        done
    done
else
    echo "Score: $score"
fi