
test_sympriority:
	gcc -fPIC -shared a1.cc -o a1.so
	gcc -fPIC -shared a2.cc -o a2.so
	gcc -fPIC -shared b1.cc a1.so -o b1.so
	gcc -fPIC -shared b2.cc a2.so -o b2.so

test_main:
	gcc main.cc b1.so b2.so -o main -Xlinker -rpath ./