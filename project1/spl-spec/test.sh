#!/bin/sh
clear
make clean
make
iter=1
while [ "$iter" -lt 10 ]
do
name="test_/test_1_r0"$iter
bin/splc ${name}".spl"
echo ${name}
# diff ${name}".out" ${name}".out"
iter=$(( $iter + 1 ))
done
while [ "$iter" -lt 13 ]
do
name="test_/test_1_r"$iter
bin/splc ${name}".spl"
echo ${name}
# diff ${name}".out" ${name}".out"
iter=$(( $iter + 1 ))
done

# iter=1
# while [ "$iter" -lt 6 ]
# do
# name="test/test_11912013_"$iter
# bin/splc ${name}".spl"
# echo ${name}
# iter=$(( $iter + 1 ))
# done