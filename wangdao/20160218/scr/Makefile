ELF:=../bin/server
OBJ:=factory.c main.c work_que.c pool_n.c
$(ELF):$(OBJ)
	gcc $^ -o $@ -I ../include -lpthread 
.PHONY:clean
clean:
		rm -rf ../bin/server
