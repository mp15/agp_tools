all : combinescaff
.PHONY : all clean

agp_file.o: agp_file.h
main.o: main.c graph.h agp_file.h

combinescaff: main.o agp_file.o
	${CC} -o $@ $^

clean:
	rm -f combinescaff *.o
