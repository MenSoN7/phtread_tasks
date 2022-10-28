all: 
	gcc -Wall -g one_thread.c -o one_thread -lm
	gcc -Wall -g several_threads.c -o several_threads -lm
        gcc -Wall -g one_thread_v2.c -o one_thread_v2 -lm
	gcc -Wall -g several_threads_v2.c -o several_threads_v2 -lm
