all: 
	gcc -Wall -g one_thread.c -o one_thread -lm
	gcc -Wall -g several_threads.c -o several_threads -lm
	gcc -Wall -g interval_generator.c -o interval_generator
        
