CXX=/export/home/acs/stud/c/cristian.brande/gcc-4.9/bin/g++ 
CFLAGS=-std=c++11 -fcilkplus -g

build: main

main:
	$(CXX) $(CFLAGS) *.cpp -o main -L/export/home/acs/stud/c/cristian.brande/gcc-4.9/lib64 -pthread

run:
	LD_LIBRARY_PATH=/export/home/acs/stud/c/cristian.brande/gcc-4.9/lib64/ CILK_NWORKERS=6 ./main

clean:
	rm main
