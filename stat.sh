#!/bin/bash

for i in $(seq 1 $1); do
	count=$(grep -cw $i tmp.txt)
	ver=$(echo "scale=3; $2/100" | bc)
	chance=$(echo "scale=5; $count/$ver" | bc)
	echo $i $chance
done
