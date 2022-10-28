all: 
	gcc -Wall -g one_thread.c -o one_thread -lm
	gcc -Wall -g several_threads.c -o several_threads -lm
        
