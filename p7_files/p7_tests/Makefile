TESTFILES := $(wildcard *.dm)
TESTS := $(TESTFILES:.dm=.test)
LIBLINUX := -dynamic-linker /lib64/ld-linux-x86-64.so.2

.PHONY: all

all: $(TESTS)

%.test:
	@echo "TEST $*"
	@../dmc $*.dm -o $*.s ;\
	COMP_EXIT_CODE=$$?;\
	./tester.sh $*.s $*.in > $*.out;\
	diff -B --ignore-all-space $*.out $*.out.expected;\
	RUN_DIFF_EXIT=$$?;\
	exit $$RUN_DIFF_EXIT

clean:
	rm -f *.3ac *.out *.err *.o *.s *.prog
	ssh "${KUID}@cycle2.eecs.ku.edu" 'rm -rf *.prog *.o *.s *.in'
