#!/bin/bash
# Runs bench 1000 times and calculates average CPU time and block we malloced.

for f in time.txt, block.txt, log.txt
do
  [ -e $f ] && rm $f
done

make

for i in {1..1000}
do
  ./bench >> log.txt
done

make clean

egrep 'CPU' log.txt | sed 's/CPU time: //' >> time.txt
egrep 'Average' log.txt | sed 's/Average bytes per free block: //' >> block.txt

echo "TIME AVE : `cat time.txt | awk '{sum+=$0} END {print sum/NR}'`"
echo "BLOCK AVE : `cat block.txt |  awk '{sum+=$0} END {print sum/NR}'`"

rm time.txt block.txt log.txt