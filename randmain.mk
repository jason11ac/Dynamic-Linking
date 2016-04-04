#Makefile

#Didn't work with -rpath=$PWD
randmain: randmain.o randcpuid.o 
	gcc -Wall -ldl -Wl,-rpath=. randmain.o randcpuid.o -o $@


randlibhw.so: randlibhw.c
	gcc -Wall -shared -mrdrnd -fPIC randlibhw.c -o $@


randlibsw.so: randlibsw.c
	gcc -Wall -shared -mrdrnd -fPIC randlibsw.c -o $@


randcpuid.o: randcpuid.c
	gcc -c randcpuid.c -o $@
