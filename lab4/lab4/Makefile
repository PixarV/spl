all: application

application: list.o
	gcc main.c list.o -o app

list.o: list.c
	gcc list.c -c

test: bin/arrays_app10 bin/one_list_app10 bin/double_list_app10 bin/arrays_get10 bin/one_list_get10 bin/double_list_get10

bin/arrays_app10: arrays_app.c
	gcc arrays_app.c -DCOUNT=10 -o bin/arrays_app10
	gcc arrays_app.c -DCOUNT=100 -o bin/arrays_app100
	gcc arrays_app.c -DCOUNT=1000 -o bin/arrays_app1000

bin/one_list_app10: one_list_app.c
	gcc one_list_app.c -DCOUNT=10 list.o -o bin/one_list_app10
	gcc one_list_app.c -DCOUNT=100 list.o -o bin/one_list_app100
	gcc one_list_app.c -DCOUNT=1000 list.o -o bin/one_list_app1000

bin/double_list_app10: double_list_app.c
	gcc double_list_app.c -DCOUNT=10 list.o -o bin/double_list_app10
	gcc double_list_app.c -DCOUNT=100 list.o -o bin/double_list_app100
	gcc double_list_app.c -DCOUNT=1000 list.o -o bin/double_list_app1000

bin/arrays_get10: arrays_get.c
	gcc arrays_get.c -DCOUNT=10 -o bin/arrays_get10
	gcc arrays_get.c -DCOUNT=100 -o bin/arrays_get100
	gcc arrays_get.c -DCOUNT=1000 -o bin/arrays_get1000

bin/one_list_get10: 
	gcc one_list_get.c -DCOUNT=10 list.o -o bin/one_list_get10
	gcc one_list_get.c -DCOUNT=100 list.o -o bin/one_list_get00
	gcc one_list_get.c -DCOUNT=1000 list.o -o bin/one_list_get1000

bin/double_list_get10: 
	gcc double_list_get.c -DCOUNT=10 -o bin/double_list_get10
	gcc double_list_get.c -DCOUNT=100 -o bin/double_list_get100
	gcc double_list_get.c -DCOUNT=1000 -o bin/double_list_get1000
clean:
	rm list.o app
