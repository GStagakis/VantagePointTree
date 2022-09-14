#!/bin/bash

echo > output.txt

for i in {5000..20000..1000}
do
    ./main 65536 256 2 $i >> output.txt
done