objs = key-schedule.o permutation.o des.o f.o

des: $(objs)
	gcc $(objs) -o des

ddes: $(obgs)
	gcc -D DEBUG=1 $(objs) -o ddes

f.o: f.c permutation.c key-schedule.c des.h
	gcc -c f.c permutation.c key-schedule.c

clean:
	rm *.o
