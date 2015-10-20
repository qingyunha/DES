objs = key-schedule.o permutation.o des.o

des: $(objs)
	gcc $(objs) -o des

key-schedule.o: key-schedule.c
	gcc -c key-schedule.c 

permutation.o: permutation.c
	gcc -c permutation.c 

des.o: des.c
	gcc -c des.c

clean:
	rm *.o
