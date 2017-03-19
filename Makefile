CXX = g++
CXXFLAGS = -O2 -ggdb -Wall -Wextra -Werror -pedantic -std=c++14
LDFLAGS =
LDLIBS =

SRCDIR = src
INCDIR = inc

INCLUDES = -I$(INCDIR)

BUILDDIR = build

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
HDRS := $(shell find $(INCDIR) -name '*.hpp')
FILES = $(SRCS) $(HDRS)

OBJS = $(addprefix $(BUILDDIR)/, $(notdir $(subst .cpp,.o, $(SRCS))))
OUT = $(BUILDDIR)/papgs

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
