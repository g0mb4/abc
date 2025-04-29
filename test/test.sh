#!/bin/bash

#set -x

export BPATH="../b/"
export LIBBPATH="../libb/"

tests=(
    t-hw
    t-auto
)

passed=0
total=0

compile () {
    ${BPATH}b $1;
    as -o b.out.o b.out.s
    ld ${LIBBPATH}/brt1.o b.out.o ${LIBBPATH}/libb1.o
    rm b.out.s
}

test () {
    echo -n "[TEST] $1 "
    compile $1.b
    ./a.out > $1.tout

    cmp --silent $1.tout $1.stdout
    if [ $? -eq 0 ]; then
        let "passed = passed + 1"
        echo "[PASS]"
    else
        echo "[FAIL]"
    fi

    let "total = total + 1"
}

gen () {
    echo "[GEN] $1"
    compile $1.b
    ./a.out > $1.stdout
}

if [ "$#" -eq "0" ]; then
    for t in "${tests[@]}"; do
        test "$t"
    done

    echo "$passed/$total passed"

    if [ $passed -eq $total ]; then
        exit 0
    else
        exit 1
    fi
fi

if [ "$1" = "gen" ]; then
    for t in "${tests[@]}"; do
        gen "$t"
    done
fi
