# Debug level in build time
# 	-1	deactivate all log messages
# 	0	allow only error messages
# 	1	allow also warnings
# 	2	allow info messages
# 	3	allow debug messages
DEBUG_LEVEL = 5 

# executable file for project
START    = server_app
RUN      = ./$(START)


LIBS = RakNet OIS
LDFLAGS = $(shell pkg-config --libs $(LIBS))
LOADLIBES =
LDLIBS = $(LDFLAGS) -lm -lrt
CFLAGS = -g
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(CFLAGS)
OBJS = $(shell cat ./Makefile.objs )
DIRS = $(shell find . -mindepth 1 -maxdepth 1 -type d -not -iname .svn )

CXXFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)

build: compile $(RUN)


compile: $(OBJS)
	@for dir in $(DIRS) ; do \
		cd $$dir ; \
		$(MAKE) $(TARGET) ; \
		cd .. ; \
	done
	
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
