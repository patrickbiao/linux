main:main.o func.o
	gcc -o main main.o func.o
main.o:main.c
	gcc -c main.c
func.o:func.c
	gcc -c func.c
.PHONY:clear
clear:
	rm -f main.o func.o  main
