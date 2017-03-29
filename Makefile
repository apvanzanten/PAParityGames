CXX = g++
CXXFLAGS = -O2 -ggdb -Wall -Wextra -Werror -pedantic -std=c++14
LDFLAGS =
LDLIBS =

SRCDIR = src
INCDIR = inc

INCLUDES = -I$(INCDIR)

BUILDDIR = build
TESTDIR = testcases
DININGDIR = $(TESTDIR)/dining
CCPDIR = $(TESTDIR)/ccp
ELEVATORDIR = $(TESTDIR)/elevator

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
HDRS := $(shell find $(INCDIR) -name '*.hpp')
FILES = $(SRCS) $(HDRS)

OBJS = $(addprefix $(BUILDDIR)/, $(notdir $(subst .cpp,.o, $(SRCS))))
OUT = $(BUILDDIR)/papg

TESTINPUT := $(shell find $(TESTDIR) -maxdepth 1 -name '*.gm')
TESTOUTPUT := $(addprefix $(BUILDDIR)/, $(notdir $(subst .gm,.result, $(TESTINPUT))))
PERFOUTPUT := $(addprefix $(BUILDDIR)/, $(notdir $(subst .gm,.perf, $(TESTINPUT))))

PERFDININGINPUT = $(shell find $(DININGDIR) -maxdepth 1 -name 'dining_5*.gm')
PERFDININGOUTPUT = $(addprefix $(BUILDDIR)/dining., $(notdir $(subst .gm,.perf, $(PERFDININGINPUT))))

PERFCCPINPUT = $(shell find $(CCPDIR) -maxdepth 1 -name 'german_linear_2*.gm') $(shell find $(CCPDIR) -maxdepth 1 -name 'german_linear_3*.gm') $(shell find $(CCPDIR) -maxdepth 1 -name 'german_linear_4*.gm')
PERFCCPOUTPUT = $(addprefix $(BUILDDIR)/ccp., $(notdir $(subst .gm,.perf, $(PERFCCPINPUT))))

PERFELEVATORINPUT = $(addprefix $(ELEVATORDIR)/, elevator1_2.gm elevator1_3.gm elevator1_4.gm elevator1_5.gm elevator2_2.gm elevator2_3.gm elevator2_4.gm elevator2_5.gm)
PERFELEVATOROUTPUT = $(addprefix $(BUILDDIR)/elevator., $(notdir $(subst .gm,.perf, $(PERFELEVATORINPUT))))


EDITOR = subl

# ==================
# build targets

all: $(OUT)

run: all
	@echo "Running..." && \
	echo "==========" && \
	$(OUT)

$(OUT): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(OUT) $(OBJS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	rm -rf $(BUILDDIR)/*

# ==================
# utility targets

edit: 
	$(EDITOR) $(FILES)

editDir:
	$(EDITOR) $(SRCDIR)

list:
	@ls -l $(INCDIR) $(SRCDIR)

test: $(TESTOUTPUT)
	@tail -n +1 $(TESTOUTPUT)

$(BUILDDIR)/%.result.tmp: $(TESTDIR)/%.gm $(TESTDIR)/%.expect all
	$(OUT) $< > $@

$(BUILDDIR)/%.result: $(BUILDDIR)/%.result.tmp
	@diff -I '#.*' $(addprefix $(TESTDIR)/, $(notdir $(subst .result.tmp,.expect, $<))) $< > $@ && echo "OK" >> $@ || echo "FAIL" >> $@

perf: $(PERFOUTPUT)
	@tail -n +1 $(PERFOUTPUT)

$(BUILDDIR)/%.perf: $(BUILDDIR)/%.result.tmp
	@grep '#' $^ > $@

$(BUILDDIR)/dining.%.result.tmp: $(DININGDIR)/%.gm all
	$(OUT) $< > $@

perfdining: $(PERFDININGOUTPUT)
	@tail -n +1 $(PERFDININGOUTPUT)

$(BUILDDIR)/ccp.%.result.tmp: $(CCPDIR)/%.gm all
	$(OUT) $< > $@

perfccp: $(PERFCCPOUTPUT)
	@tail -n +1 $(PERFCCPOUTPUT)

$(BUILDDIR)/elevator.%.result.tmp: $(ELEVATORDIR)/%.gm all
	$(OUT) $< > $@

perfelevator: $(PERFELEVATOROUTPUT)
	@tail -n +1 $(PERFELEVATOROUTPUT)


.PRECIOUS: $(BUILDDIR)/%.result.tmp $(BUILDDIR)/dining.%.result.tmp $(BUILDDIR)/ccp.%.result.tmp $(BUILDDIR)/elevator.%.result.tmp  