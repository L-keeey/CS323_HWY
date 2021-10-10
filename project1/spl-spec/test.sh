name="test/test_1_r01"
clear
make clean
make
bin/splc ${name}".spl"
diff ${name}".out1" ${name}".out" 