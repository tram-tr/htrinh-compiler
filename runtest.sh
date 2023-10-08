#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <test_type>"
    exit 1
fi

TEST_DIR=tests
TEST_TYPE="$1"
BMINOR=./bminor

if [ "$TEST_TYPE" == "encode" ] || [ "$TEST_TYPE" == "encode-hidden" ]; then
    for testfile in $TEST_DIR/$TEST_TYPE/good*.bminor
    do
        if $BMINOR --encode "$testfile" > "$testfile.out"
        then
            echo "$testfile success (as expected)"
        else
            echo "$testfile failure (INCORRECT)"
        fi
    done

    for testfile in $TEST_DIR/$TEST_TYPE/bad*.bminor
    do
        if $BMINOR --encode "$testfile" > "$testfile.out"
        then
            echo "$testfile success (INCORRECT)"
        else
            echo "$testfile failure (as expected)"
        fi
    done
elif [ "$TEST_TYPE" == "scanner" ] || [ "$TEST_TYPE" == "scanner-sample" ]; then
    for testfile in $TEST_DIR/$TEST_TYPE/good*.bminor
    do
        if $BMINOR --scan "$testfile" > "$testfile.out"
        then
            echo "$testfile success (as expected)"
        else
            echo "$testfile failure (INCORRECT)"
        fi
    done

    for testfile in $TEST_DIR/$TEST_TYPE/bad*.bminor
    do
        if $BMINOR --scan "$testfile" > "$testfile.out"
        then
            echo "$testfile success (INCORRECT)"
        else
            echo "$testfile failure (as expected)"
        fi
    done
elif [ "$TEST_TYPE" == "parser" ] || [ "$TEST_TYPE" == "parser-sample" ]; then
    for testfile in $TEST_DIR/$TEST_TYPE/good*.bminor
    do
        if $BMINOR --parse "$testfile" > "$testfile.out"
        then
            echo "$testfile success (as expected)"
        else
            echo "$testfile failure (INCORRECT)"
        fi
    done

    for testfile in $TEST_DIR/$TEST_TYPE/bad*.bminor
    do
        if $BMINOR --parse "$testfile" > "$testfile.out"
        then
            echo "$testfile success (INCORRECT)"
        else
            echo "$testfile failure (as expected)"
        fi
    done
else
    echo "Unknown test type: $TEST_TYPE"
fi
