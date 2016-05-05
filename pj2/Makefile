cflags = -std=c99 -pthread -Wall -Wextra -Wpedantic
all: sched_test

.PHONY:run clean

sched_test: sched_test.c
	gcc sched_test.c $(cflags) -o sched_test

run:
	./sched_test

clean:
	rm -rf ./sched_test
