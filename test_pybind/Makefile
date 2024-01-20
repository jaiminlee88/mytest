
ifeq ($(PY),)
	PY := 3.8
endif

PYINC  := $(shell python$(PY)-config --includes)
ifeq ($(PYINC),)
  $(error /usr/include/python$(PY) not found)
endif

CXX = g++
CXXSTD = c++17
CFLAGS = -O0 -Wall -shared --std=${CXXSTD} -fPIC
IFLAGS = $(PYINC)

STRAT_OBJS = $(subst .cc,.o,$(wildcard *.cc))

%.o: %.cc
	$(CXX) $(CFLAGS) $(IFLAGS) -c $^ -o $@

demo_pybind: $(STRAT_OBJS)
	$(CXX) $(CFLAGS) $(IFLAGS) $^ -o demo_pybind`python$(PY)-config --extension-suffix`
	rm *.o -fr

clean:
	rm -rf *.o *.so extver.hh