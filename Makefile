CXX = g++
CXXFLAGS = -O2 -ggdb -Wall -Wextra -Werror -pedantic -std=c++14
LDFLAGS =
LDLIBS =

SRCDIR = src
INCDIR = inc

INCLUDES = -I$(INCDIR)

BUILDDIR = build
TESTDIR = testcases

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
HDRS := $(shell find $(INCDIR) -name '*.hpp')
FILES = $(SRCS) $(HDRS)

OBJS = $(addprefix $(BUILDDIR)/, $(notdir $(subst .cpp,.o, $(SRCS))))
OUT = $(BUILDDIR)/papg

TESTINPUT := $(shell find $(TESTDIR) -maxdepth 1 -name '*.gm')
TESTOUTPUT := $(addprefix $(BUILDDIR)/, $(notdir $(subst .gm,.result, $(TESTINPUT))))
PERFOUTPUT := $(addprefix $(BUILDDIR)/, $(notdir $(subst .gm,.perf, $(TESTINPUT))))

# ==================
# build targets

all: $(OUT)

$(OUT): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(OUT) $(OBJS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	rm -rf $(BUILDDIR)/*

# ==================
# test targets

test: $(TESTOUTPUT)
	@tail -n +1 $(TESTOUTPUT)

$(BUILDDIR)/%.result.tmp: $(TESTDIR)/%.gm $(TESTDIR)/%.expect all
	$(OUT) $< > $@

$(BUILDDIR)/%.result: $(BUILDDIR)/%.result.tmp
	@diff -I '#.*' $(addprefix $(TESTDIR)/, $(notdir $(subst .result.tmp,.expect, $<))) $< > $@ && echo "OK" >> $@ || echo "FAIL" >> $@

testview: $(TESTOUTPUT)
	@tail -n +1 $(addsuffix .tmp, $(TESTOUTPUT))

perf: $(PERFOUTPUT)
	@tail -n +1 $(PERFOUTPUT)

$(BUILDDIR)/%.perf: $(BUILDDIR)/%.result.tmp
	@grep '#' $^ > $@

.PRECIOUS: $(BUILDDIR)/%.result.tmp