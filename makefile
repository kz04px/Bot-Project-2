CC     = g++
CFLAGS = -std=c++17 -Wall -Wextra -Wshadow
RFLAGS = -O3 -flto -march=native -DNDEBUG
DFLAGS = -g

LINKER = g++ -o
LFLAGS = -g -lglfw -lGLEW -lGL -pthread

TARGET = main
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INC    = -I include

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

release:
	$(MAKE) CFLAGS="$(CFLAGS) $(RFLAGS)"

debug:
	$(MAKE) CFLAGS="$(CFLAGS) $(DFLAGS)" TARGET="$(TARGET)-debug"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR) $(BINDIR)/$(TARGET) $(BINDIR)/$(TARGET)-debug

.PHONY: clean
