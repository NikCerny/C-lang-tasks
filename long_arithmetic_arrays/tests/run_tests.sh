#!/bin/bash
set -e

PROGRAM=./a.out
MODE="$1"   # "sum", "prod", or empty = both

run_tests() {
    local op=$1
    local dir="tests/$op"

    echo "=== Running $op tests ==="
    for i in {1..13}; do
        in_file="$dir/${i}_in.txt"
        ref_file="$dir/${i}_ref.txt"

        echo "Running test $i ($op)..."

        # Run program
        $PROGRAM "$in_file" "$op"

        # Compare
        if diff -q -w -B output.txt "$ref_file" > /dev/null; then
            echo "✅ Test $i ($op) passed"
        else
            echo "❌ Test $i ($op) failed"
            echo "Differences:"
            diff -w -B output.txt "$ref_file"
        fi

        echo "------------------------"
    done
}

if [[ "$MODE" == "sum" ]]; then
    run_tests sum
elif [[ "$MODE" == "prod" ]]; then
    run_tests prod
else
    run_tests sum
    run_tests prod
fi
