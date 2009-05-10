test : test.o
	gcc -g test.o -L/usr/X11R6/lib -lglfw -lGL -lGLU -lX11 -lpthread -lXxf86vm -lm -lXrandr -o test

test.o : test.c
	gcc -g -c test.c -o test.o

clean :
	rm test
	rm *.o
