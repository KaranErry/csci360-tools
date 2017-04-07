run: sched_algs
	./sched_algs

test: sched_algs
	./sched_algs < self-assessment.txt

sched_algs: sched_algs.c
	gcc -Wreturn-type sched_algs.c -o sched_algs

clean:
	rm sched_algs