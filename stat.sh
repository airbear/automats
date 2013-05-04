#!/bin/bash

for i in $(seq 1 $1); do
	echo $i $(grep -cw $i tmp.txt)
done
