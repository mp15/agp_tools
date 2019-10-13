all : combinescaff
.PHONY : all clean

main.o: main.c graph.h agp_file.h

combinescaff: main.o
	${CC} -o $@ $^

clean:
	rm -f combinescaff *.o
