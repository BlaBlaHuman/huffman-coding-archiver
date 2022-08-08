XX = g++
CXXFLAGS = -O3 -Wall -Werror -std=c++2a -Iinclude
LDFLAGS =

EXE = huffman
SRCDIR = src
OBJDIR = obj

EXE_TESTS = huffman_tests
SRCDIR_TESTS = custom_tests
OBJDIR_TESTS = obj_tests

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))
OBJECTS_TESTS = $(patsubst $(SRCDIR_TESTS)/%.cpp,$(OBJDIR_TESTS)/%.o,$(wildcard $(SRCDIR_TESTS)/*.cpp))

all: $(EXE) $(EXE_TESTS)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)

$(EXE_TESTS): $(OBJECTS_TESTS) $(OBJDIR)/decoder.o $(OBJDIR)/encoder.o $(OBJDIR)/file_utility.o $(OBJDIR)/huffman_tree.o
	$(CXX) $(OBJDIR)/decoder.o $(OBJDIR)/encoder.o $(OBJDIR)/file_utility.o $(OBJDIR)/huffman_tree.o $(OBJECTS_TESTS) -o $(EXE_TESTS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

$(OBJDIR_TESTS)/%.o: $(SRCDIR_TESTS)/%.cpp | $(OBJDIR_TESTS)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(OBJDIR)/*.d)
include $(wildcard $(OBJDIR_TESTS)/*.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR_TESTS):
	mkdir -p $(OBJDIR_TESTS)

clean:
	rm -rf $(OBJDIR_TESTS) $(EXE_TESTS)
	rm -rf $(OBJDIR) $(EXE)
.PHONY: clean all
