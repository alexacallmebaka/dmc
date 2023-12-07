#!/bin/bash
#assemble and link x64 assembly from a drewno mars file.
#intended to be run on cycle servers, used by tester.sh


#from makefile:
#	@as -o $*.o $*.s
#	@ld $(LIBLINUX) \
#		/usr/lib/x86_64-linux-gnu/crt1.o \
#		/usr/lib/x86_64-linux-gnu/crti.o \
#		-lc \
#		$*.o \
#		../stddm.o \
#		/usr/lib/x86_64-linux-gnu/crtn.o \
#		-o  $*.prog
#	@./$*.prog < $*.in > $*.out; \


as -o "${1}.o" "${1}"

ld -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/crt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  -lc \
  "${1}.o" \
  stddm.o \
  /usr/lib/x86_64-linux-gnu/crtn.o \
  -o  "${1}.prog"
chmod +x "${1}.prog"
