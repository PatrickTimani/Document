PLIBPATH	:= ../packetlib
TOLIBPATH	:= ../timeoutlib
VSLCLIBPATH	:= ./vslabclib

CC := gcc

CFLAGS 		:= -O2 -Wall


vslabc: vslabc.o vslabclib.o packetlib.o timeoutlib.o
	@echo -n "Building/linking client application... "
	@$(CC) $(CFLAGS) vslabc.o vslabclib.o packetlib.o timeoutlib.o -o vslabc
	@echo "Done."

vslabc.o: vslabc.c vslabc.h
	@echo -n "Compiling client... "
	@$(CC) $(CFLAGS) -c vslabc.c -o vslabc.o
	@echo "Done."
vslabclib.o: $(VSLCLIBPATH)/vslabclib.c $(VSLCLIBPATH)/vslabclib.h $(TOLIBPATH)/timeoutlib.h $(PLIBPATH)/packetlib.h
	@echo -n "Compiling vslab client lib... "
	@$(CC) $(CFLAGS) -c $(VSLCLIBPATH)/vslabclib.c -o vslabclib.o
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
	@echo -n "Cleaning up client directory... "
	@rm -f *.o *~ vslabc *.gdb	
	@echo "Done."
