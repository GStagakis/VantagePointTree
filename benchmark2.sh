#!/bin/bash

echo > output.txt

for i in 2 4 8 16 32 64 128 256
do
    ./main 65536 $i 0 >> output.txt
done

echo > cilk_output.txt

for i in 2 4 8 16 32 64 128 256
do
    ./main 65536 $i 1 >> cilk_output.txt
done

echo > thresh_output.txt

for i in 2 4 8 16 32 64 128 256
do
    ./main 65536 $i 2 >> thresh_output.txt
done