
all : key permute

key : key-schedule.c
	gcc key-schedule.c -o key

permute: permutation.c
	gcc permutation.c -o permute
