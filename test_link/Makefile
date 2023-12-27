REORDER=yes
CXX=c++11

all:
	g++ --std=${CXX} -c a.cc -o a.o
	g++ --std=${CXX} -c b.cc -o b.o

link:
	ld a.o b.o -e main -o ab

test_link: clean  all  link

test_lib:
	# g++ --std=c++11 -fvisibility=hidden -fPIC -shared -o Lib.so Lib.cc #屏蔽了Lib中的符号
	g++ --std=c++11 -fPIC -shared -o Lib.so Lib.cc
	g++ --std=c++11 -o Program1 Program1.cc -L./ Lib.so
	g++ --std=c++11 -o Program2 Program2.cc -L./ Lib.so
	g++ --std=c++11 -o loadso test_loadso.cc

clean:
	rm -f *.o ab



0000044c <bar>:
44c: 55                    push   %ebp
44d: 89 e5                 mov    %esp,%ebp
44f: e8 40 00 00 00        call   494 <__i686.get_pc_thunk.cx>
454: 81 c1 8c 11 00 00     add    $0x118c,%ecx
45a: c7 81 28 00 00 00 01  movl   $0x1,0x28(%ecx)         // a = 1
461: 00 00 00 
464: 8b 81 f8 ff ff ff     mov 0xfffffff8(%ecx),%eax
46a: c7 00 02 00 00 00     movl   $0x2,(%eax)             // b = 2
470: 5d                    pop    %ebp
471: c3                    ret    

00000494 <__i686.get_pc_thunk.cx>:
494: 8b 0c 24              mov    (%esp),%ecx
497: c3                    ret 