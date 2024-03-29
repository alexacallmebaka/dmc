LEXER_TOOL := flex
CXX ?= g++ # Set the C++ compiler to g++ iff it hasn't already been set
CPP_SRCS := $(wildcard *.cpp) 
OBJ_SRCS := parser.o lexer.o $(CPP_SRCS:.cpp=.o)
DEPS := $(OBJ_SRCS:.o=.d)
FLAGS=--coverage -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wuninitialized -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wundef -Werror -Wno-unused -Wno-unused-parameter
#add these FLAGS for profiling 
#CXX = clang++
#FLAGS+=-fprofile-instr-generate -fcoverage-mapping


.PHONY: all clean test cleantest cov


all: dmc

.PRECIOUS: %.prog

clean:
	rm -rf *.gcov *.gcno *.gcda *.output *.o *.cc *.hh $(DEPS) dmc parser.dot parser.png

-include $(DEPS)

dmc: $(OBJ_SRCS)
	$(CXX) $(FLAGS) -g -std=c++14 -o $@ $(OBJ_SRCS)
	chmod a+x dmc

%.o: %.cpp 
	$(CXX) $(FLAGS) -g -std=c++14 -MMD -MP -c -o $@ $<

parser.o: parser.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-switch-default -g -std=c++14 -MMD -MP -c -o $@ $<

parser.cc: drewno_mars.yy
	bison -Werror --graph=parser.dot --defines=frontend.hh -v $<
	# Use the below version if you have an old version of bison
	#bison -Wnone --graph=parser.dot --defines=frontend.hh -v $< 

lexer.yy.cc: drewno_mars.l
	$(LEXER_TOOL) --outfile=lexer.yy.cc $<

lexer.o: lexer.yy.cc
	$(CXX) $(FLAGS) -Wno-sign-compare -Wno-sign-conversion -Wno-old-style-cast -Wno-switch-default -g -std=c++14 -c lexer.yy.cc -o lexer.o

cov: test
	gcov *.gcda | grep -A 1 -E ".hpp|.cpp"

test: p3 p4

p3: all
	$(MAKE) -C tests/sdt_tests

p4: all
	$(MAKE) -C tests/name_analysis_tests

cleantest:
	$(MAKE) -C tests/sdt_tests clean
	$(MAKE) -C tests/name_analysis_tests clean
