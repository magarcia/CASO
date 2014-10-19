#!/usr/bin/env bash
TIMES=10
times=${1:-$TIMES}

echo "# Test running $times times"; echo

function calcAvg {
    avgSec=$(echo "`cat tmp.out | awk '{print $6}' | tail -n $times | paste -sd+ | bc -l`/$times" | bc -l | awk '{printf "%.5f\n", $0}')
    avgBand=$(echo "`cat tmp.out | awk '{print $8}' | tail -n $times | paste -sd+ | bc -l`/$times" | bc -l | awk '{printf "%.5f\n", $0}')
    echo "Average seconds $avgSec s, average bandwidth $avgBand MB/s"
    rm tmp.out
    touch tmp.out
}


touch tmp.out # Initialize temporal file output

#######################################################

echo "## filewriter.c"

for i in `seq 1 $times`
do
    out=$(./filewriter)
    echo $out
    echo $out >> tmp.out
done

calcAvg; echo

#######################################################

echo "## dd if=/dev/zero of=output count=3276800 bs=64"

for i in `seq 1 $times`
do
    out=$( (dd if=/dev/zero of=output count=3276800 bs=64) 2>&1 | tail -n 1)
    echo $out
    echo $out >> tmp.out
done

calcAvg; echo

#######################################################

echo "## dd if=/dev/zero of=output count=409600 bs=512"

for i in `seq 1 $times`
do
    out=$( (dd if=/dev/zero of=output count=409600 bs=512) 2>&1 | tail -n 1)
    echo $out
    echo $out >> tmp.out
done

calcAvg; echo

#######################################################

echo "## dd if=/dev/zero of=output count=25600 bs=8192"

for i in `seq 1 $times`
do
    out=$( (dd if=/dev/zero of=output count=25600 bs=8192) 2>&1 | tail -n 1)
    echo $out
    echo $out >> tmp.out
done

calcAvg; echo

