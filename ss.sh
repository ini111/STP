#!/bin/bash
# author: wuxinyu
g++ Steiner.cpp -O3

STARTTIME=$(date +%s)


./a.out ./instances/hc6u.stp ./results/hc6u.stp

ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
