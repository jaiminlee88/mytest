################################################################################
# Makefile for hts EE
################################################################################
# EXTS = htsee
EDGEROOT=../edge/
EEKIT = ../libeekit/
DISTROOT= /big/archive/pkg/tradeapi
DEPLIB = ./dep/lib/
# DBG = 1

ifdef DBG
OPTFLAGS += -g -O0
else ifdef ODBG
OPTFLAGS += -g -O2
else
OPTFLAGS += -O2
endif

ifeq ($(PY),3.6)
	PY := 3.6m
else
	PY := 3.8
endif
PYINC  := $(shell python$(PY)-config --includes)
ifeq ($(PYINC),)
  $(error /usr/include/python$(PY) not found)
endif

EXTVERHH = extver.hh
MODVERSTR = $(shell ${EDGEROOT}/.verstr .)
MODVERHASH = $(shell ${EDGEROOT}/.verhash .)

USE_VERHH = yes


# htsapi.so
# STRAT_OBJS = $(subst .cc,.o,$(wildcard *.cc))
_CFLAGS = $(OPTFLAGS) -Wall --std=c++17 -shared -fPIC

_IFLAGS += $(PYINC) -I../common_types -I../riskcheck -I../tradeapi -I../encrypt-login/
_IFLAGS += -I ${EEKIT}


EXTLIB = -Wl,-whole-archive -L${EEKIT}/ -leekit -L../riskcheck/ -lriskcheck -Wl,-no-whole-archive
# _LFLAGS = -L -lsecitpdklib -lfixapi -lsecitpdk -L../encrypt-login/ -l_hts_login ${EXTLIB}

# _IFLAGS += -I./dep/include/
_IFLAGS += -I/usr/include/htsmt
# _LFLAGS += -L${DEPLIB} -lfixapi -lmaritpdk -L. -l_htsmt_login ${EXTLIB} ${EXTLIB}
_LFLAGS += -L${DEPLIB} -lfixapi -lmaritpdk -L../encrypt-login -l_htsmt_login ${EXTLIB}

# _LFLAGS = -L -lsecitpdklib -lfixapi -lsecitpdk -L../encrypt-login/ -l_hts_login -Wl,-whole-archive -L../riskcheck/ -lriskcheck  -L${KITDIR}/lib -lkit -llzma -llz4 -lbid -lgmp -Wl,-no-whole-archive
# _LFLAGS = -L -lsecitpdklib -lfixapi -lsecitpdk -L../encrypt-login/ -l_hts_login -L../riskcheck -lriskcheck
# _LFLAGS = -Wl,-whole-archive  -L -lsecitpdklib -lfixapi -lsecitpdk -L../encrypt-login/ -l_hts_login -L../riskcheck -lriskcheck -Wl,-no-whole-archive

# OBJS=$(subst .cc,.o,$(wildcard *.cc))

# %.o: %.cc
# 	$(CXX) $(CFLAGS) $(IFLAGS) $^ -c
.PHONY: cleaner wheel $(EXTVERHH)

test:
	$(CXX) $(_CFLAGS) $(_LFLAGS) $(_IFLAGS) htsmt_trdapi.cc -c
	$(CXX) $(_CFLAGS) $(_LFLAGS) $(_IFLAGS) htsmt_bind.cc -c
	$(CXX) -o htsmt_trdapi`python$(PY)-config --extension-suffix` $(_CFLAGS) htsmt_trdapi.o htsmt_bind.o $(_IFLAGS) $(_LFLAGS) -Wl,-rpath=./:${DEPLIB}
	cp htsmt_trdapi`python$(PY)-config --extension-suffix` libhtsmt_trdapi.${MODVERSTR}.so
	ln -sf libhtsmt_trdapi.${MODVERSTR}.so libhtsmt_trdapi.so
	rm *.o

so:
	$(CXX) $(_CFLAGS) $(_LFLAGS) $(_IFLAGS) htsmt_trdapi.cc -c
	$(CXX) $(_CFLAGS) $(_LFLAGS) $(_IFLAGS) htsmt_bind.cc -c
	$(CXX) -o htsmt_trdapi`python$(PY)-config --extension-suffix` $(_CFLAGS) htsmt_trdapi.o htsmt_bind.o $(_IFLAGS) $(_LFLAGS) -Wl,-rpath=./:${DEPLIB}
	cp htsmt_trdapi`python$(PY)-config --extension-suffix` libhtsmt_trdapi.${MODVERSTR}.so
	ln -sf libhtsmt_trdapi.${MODVERSTR}.so libhtsmt_trdapi.so
	rm *.o

$(EXTVERHH):
	@echo \#define EDGEX_VERSION \"$(MODVERSTR)\" > $@
	@echo \#define EDGEX_VERHASH \"$(MODVERHASH)\" >> $@
	@echo \#define EDGEX_COMPILE \"$(_CFLAGS)\" >> $@

arg0 := $(word 1, $(MAKECMDGOALS))
arg1 := $(word 2, $(MAKECMDGOALS))
arg2 := $(word 3, $(MAKECMDGOALS))

ifdef f
	srcfile := $(shell echo $(f)|awk -F '. ' '{ print $$1 }')
else
	ifneq ($(arg1),)
		srcfile := $(shell echo $(arg1)|awk -F '.' '{ print $$1 }')
		f := $(arg1)
	endif
endif

gen:
	@echo param: f=${f} srcfile=${srcfile}
	g++ --std=c++11 -g -O0 -lpthread -latomic ${f} -o ${srcfile}.out

gen14:
	@echo param: f=${f} srcfile=${srcfile}
	g++ --std=c++14 -g -O0 -lpthread -latomic ${f} -o ${srcfile}.out

gen17:
	@echo param: f=${f} srcfile=${srcfile}
	g++ --std=c++17 -g -O0 -lpthread -latomic ${f} -o ${srcfile}.out

test_pimpl:
	g++ --std=c++11 -g -O0 -lpthread -latomic  -I./ test_pimpl_gadget.hh test_pimpl_widget.hh   test_pimpl_widget.cc  test_pimpl.cc  -o test_pimpl.out

test_reference:
	g++ --std=c++11 -g -O0 test_reference.cc -o test_reference.out

test_obj:
	g++ --std=c++11 -g -O0 test_obj.cc -o test_obj.out

test_sym:
	g++ --std=c++11 -g -O0 test_sym.cc -o test_sym.out

test_linkonce:
	g++ --std=c++11 -O0 test_linkonce.cc -o test_linkonce.o

test_hello:
	gcc -c -fno-builtin -m32 test_hello.c -o test_hello.o
	ld -static -m elf_i386  -T test_hello.lds -o test_hello.out test_hello.o
	# ld -static -e nomain -m elf_i386 test_hello.o -o test_hello.out

test_segment:
	g++ --std=c++11 -O0 test_segment.cc -o test_segment.out
	# g++ --std=c++11 -static -O0 test_segment.cc -o test_segment.out
	# gcc -static -O0 test_segment.cc -o test_segment.out

test_maxheap:
	g++ --std=c++11 -O0 test_maxheap.cc -o test_maxheap.out

test_json:
	g++ --std=c++11 -O0 test_json.cc -o test_json.out

test_enum:
	g++	--std=c++11 -O0 test_enum.cc -o test_enum.out

test_convnum:
	g++ --std=c++11 -O0 test_convnum.cc -o test_convnum.out

test_mmap:
	g++ --std=c++11 -O0 test_mmap.cc -o test_mmap.out

test_ctors:
	g++ --std=c++11 -O0 test_ctors.cc -o test_ctors.out

test_read:
	g++ --std=c++11 -O0 test_read.cc -o test_read.out

test_virtual:
	g++ --std=c++11 -O0 test_virtual.cc -o test_virtual.out

test_auto:
	g++ --std=c++11 -O0 test_auto.cc -o test_auto.out

test_rstr:
	g++ --std=c++11 -O0 test_rstr.cc -o test_rstr.out

wheel: clean $(EXTVERHH)
	rm wheel/dist -fr
	make so
	cp htsmt_trdapi.cpython-38-x86_64-linux-gnu.so ./dep/lib/* wheel/htsmt_trdapi/
	cp ./wheel/itpdk.ini ./wheel/htsmt_trdapi/itpdk.ini
	cp ./wheel/htsmt_rest_api.py ./wheel/htsmt_trdapi/htsmt_rest_api.py
	cd ./wheel && python$(PY) setup.py bdist_wheel

test:
	$(CXX) $(_CFLAGS) test.cc -o test

dist: wheel
	@echo `ssh dev@dev 'mkdir -p ${DISTROOT}'`
	@echo `scp wheel/dist/* dev@dev:${DISTROOT}`
	@echo `basename ./wheel/dist/* | xargs -i echo source: ${DISTROOT}/{}`
	@echo "done"

clean:
	@rm *.o *.so *.a *.out -f
	@rm -rf wheel/htsapi.egg-info/ wheel/dist wheel/build ${EXTVERHH}
	@rm -rf wheel/htsapi/__about__.py
### Makefile ends here

