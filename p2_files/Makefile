LEXER_TOOL := flex
CXX ?= g++ # Set the C++ compiler to g++ iff it hasn't already been set
CPP_SRCS := $(wildcard *.cpp) 
OBJ_SRCS := parser.o lexer.o $(CPP_SRCS:.cpp=.o)
DEPS := $(OBJ_SRCS:.o=.d)
FLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wuninitialized -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wundef -Werror -Wno-unused -Wno-unused-parameter


TESTPROGS := $(wildcard tests/*.dm)
TESTS := $(TESTPROGS:.dm=)

GOOD_TESTS := $(wildcard tests/good/*.dm)
BAD_TESTS := $(wildcard tests/bad/*.dm)

.PHONY: all clean test cleantest

all: 
	make dmc

clean:
	rm -rf *.output *.o *.cc *.hh $(DEPS) dmc

-include $(DEPS)

dmc: $(OBJ_SRCS)
	$(CXX) $(FLAGS) -g -std=c++14 -o $@ $(OBJ_SRCS)

%.o: %.cpp 
	$(CXX) $(FLAGS) -g -std=c++14 -MMD -MP -c -o $@ $<

parser.o: parser.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-switch-default -g -std=c++14 -MMD -MP -c -o $@ $<

#parser.cc: drewno_mars.yy
parser.cc: adair.yy
	bison -Werror -Wno-deprecated --defines=frontend.hh -v $<

lexer.yy.cc: drewno_mars.l
	$(LEXER_TOOL) --outfile=lexer.yy.cc $<

lexer.o: lexer.yy.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-old-style-cast -Wno-switch-default -g -std=c++14 -c lexer.yy.cc -o lexer.o

test: all
	./dmc test1_good.dm -p || exit 1;
	./dmc test2_bad.dm -p && exit 0;
	@echo "All tests performed as expected!"
