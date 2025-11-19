CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lcs50

# build any target that has a matching .c
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *~ 
