makef: nim.c
	gcc -o nim nim.c
	
clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
