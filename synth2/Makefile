SOURCES := drop.cpp Player.cpp Buffer.cpp myrand.cpp Hihats.cpp
# Objs are all the sources, with .cpp replaced by .o
OBJS := $(SOURCES:.cpp=.o)
CXXFLAGS += -std=c++1y

all: drop

# Compile the binary 't' by calling the compiler with cflags, lflags, and any libs (if defined) and the list of objects.
drop: $(OBJS)
	$(CXX) $(CXXFLAGS) -o drop.exe $(OBJS) $(LFLAGS) $(LIBS)

# Get a .o from a .cpp by calling compiler with cflags and includes (if defined)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<
