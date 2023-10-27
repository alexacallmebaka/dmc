TESTFILES := $(wildcard **/*.dm) $(wildcard *.dm)
TESTS := $(TESTFILES:.dm=.test)

.PHONY: all

all: $(TESTS)

%.test:
	@echo "Testing $*.dm"
	@touch $*.err
	@../dmc $*.dm -c 2> $*.err ;\
	PROG_EXIT_CODE=$$?;\
	echo "diff error...";\
	diff $*.err $*.err.expected;\
	ERR_EXIT_CODE=$$?;\
	exit $$ERR_EXIT_CODE

clean:
	rm -f *.out *.err
