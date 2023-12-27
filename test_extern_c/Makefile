
so:
	g++ -shared -fpic -o liblogin.so login.cc

all: so
	g++ main.cc -o main -L./ -llogin