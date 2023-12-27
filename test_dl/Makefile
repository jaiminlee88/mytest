
test_myfunc:
	g++ --std=c++11 -fPIC -shared -O0 myfunc.cc -o libmyfunc.so

test_dl: clear test_myfunc
	g++ --std=c++11 -O0 test_dl.cc -o test_dl.out -ldl
	# gcc -o test_dl.out test_dl.c -ldl

test_dlgene:
	gcc test_dlgene.cc -o test_dlgene.out -ldl

clear:
	rm -f *.so *.out