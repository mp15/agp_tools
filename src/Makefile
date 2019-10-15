all : combinescaff
.PHONY : all clean

agp_file.o: agp_file.h
graph.o: graph.h
main.o: main.c graph.h agp_file.h

combinescaff: main.o agp_file.o graph.o
	${CC} -o $@ $^

clean:
	rm -f combinescaff *.o
