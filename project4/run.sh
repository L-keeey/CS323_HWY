rm -f ./test/*.s

make clean
make
./bin/splc ./test/test_4_r01.ir
#./bin/splc ./sample/test_4_fact.ir
./bin/splc ./test/test_4_r02.ir

./bin/splc ./test/test_4_r03.ir
