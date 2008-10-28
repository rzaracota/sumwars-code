# Debug level in build time
# 	-1	deactivate all log messages
# 	0	allow only error messages
# 	1	allow also warnings
# 	2	allow info messages
# 	3	allow debug messages
DEBUG_LEVEL = 5 

# executable file for project
START    = sumwars
RUN      = ./$(START)


LIBS = OGRE OIS CEGUI CEGUI-OGRE RakNet
LDFLAGS = $(shell pkg-config --libs $(LIBS))
LOADLIBES =
LDLIBS = $(LDFLAGS) -lm -lrt
CFLAGS = -Wall -O3
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(CFLAGS)
CXXFLAGS += -I./core -I./gui -I./tinyxml
OBJS = $(shell cat ./Makefile.objs )
DIRS = $(shell find . -mindepth 1 -maxdepth 1 -type d -not -iname .svn )

CXXFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)  $(shell cat ./defines)

build: compile $(RUN)


compile: $(OBJS)
	
link: compile
	$(CXX) $(CXXFLAGS) -o $(RUN) $(OBJS) $(LDLIBS)

$(RUN): link


run: $(RUN)
	  $(RUN) $(HOST) $(PROTOCOL)


%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean :	
	rm -f $(OBJS) $(RUN)
