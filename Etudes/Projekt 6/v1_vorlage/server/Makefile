PLIBPATH	:= ../packetlib
TOLIBPATH	:= ../timeoutlib
7SEGLIBPATH	:= ./7seglib

CC := arm-elf-gcc

WARN 	:= -Wall
LDFLAGS	:= -Wl,-elf2flt
CFLAGS 	:= -O2 -Wall


vslabd: vslabd.o packetlib.o timeoutlib.o 7seg.o
	@echo -n "Building/linking vslabd... "
	@$(CC) $(CFLAGS) $(LDFLAGS) vslabd.o packetlib.o timeoutlib.o 7seg.o -o vslabd
	@echo "Done."

vslabd.o: vslabd.c vslabd.h
	@echo -n "Compiling vslabd... "
	@$(CC) $(CFLAGS) -c vslabd.c -o vslabd.o
	@echo "Done."
7seg.o: $(7SEGLIBPATH)/7seg.c $(7SEGLIBPATH)/7seg.h
	@echo -n "Compiling 7seg lib... "
	@$(CC) $(CFLAGS) -c $(7SEGLIBPATH)/7seg.c -o 7seg.o
	@echo "Done."
packetlib.o: $(PLIBPATH)/packetlib.c $(PLIBPATH)/packetlib.h
	@echo -n "Compiling packet handler... "
	@$(CC) $(CFLAGS) -c $(PLIBPATH)/packetlib.c -o packetlib.o
	@echo "Done."
timeoutlib.o: $(TOLIBPATH)/timeoutlib.c $(TOLIBPATH)/timeoutlib.h
	@echo -n "Compiling timeout handler... "
	@$(CC) $(CFLAGS) -c $(TOLIBPATH)/timeoutlib.c -o timeoutlib.o
	@echo "Done."



.PHONY:	clean

clean:
	@echo -n "Cleaning up daemon directory... "
	@rm -f *.o *~ vslabd *.gdb *.elf2flt *.elf
	@echo "Done."
