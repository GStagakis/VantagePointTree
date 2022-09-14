#!/bin/bash

echo > output.txt

for i in 512 1024 2048 4096 8192 16384 32768 65536 131072 262144
do
    ./main $i 256 0 >> output.txt
done

echo > cilk_output.txt

for i in 512 1024 2048 4096 8192 16384 32768 65536 131072 262144
do
    ./main $i 256 1 >> cilk_output.txt
done

echo > thresh_output.txt

for i in 512 1024 2048 4096 8192 16384 32768 65536 131072 262144
do
    ./main $i 256 2 >> thresh_output.txt
done