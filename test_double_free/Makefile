

all:
	g++ -fPIC -c libkit.cc
	ar crs libkit.a libkit.o
	g++ -shared libkit.o -o libkit.so
	#g++ -fPIC -shared xtpapi.cc -Wl,-whole-archive -L. -lkit -Wl,-no-whole-archive -o libxtpapi.so
	g++ -fPIC -shared xtpapi.cc ./libkit.so -o libxtpapi.so
	# g++ edge.cc -rdynamic -Wl,-whole-archive ./libkit.a -Wl,-no-whole-archive -ldl -o edge
	# g++ edge.cc -Wl,-whole-archive ./libkit.a(包含了一个静态变量) -Wl,-no-whole-archive -ldl -o edge(dlopen中包含了一个静态变量)
	g++ edge.cc -L./ -lkit -rdynamic -Wl,-no-whole-archive -ldl -o edge

clean:
	rm edge *.so *.a *.o
