#!/bin/bash
# Run program each iteration n times
n=3
k=1
while [ $k -lt 100 ]
do
    r=0
    echo -n "\coordinate (m) at (0,0); \foreach \p in {" >> 1.out
    while [ $r -lt $n ]
    do
        ./nqueens "$k" 2
        let r=$r+1
    done
    echo "}{\coordinate (m) at (\$(m)+(\p)$); } \draw[fill=red] let \p1=(m) in (\x1 / 3,\y1 / 3) circle (0.05cm);" >> 1.out
    echo $k
    let k=$k+1
done
