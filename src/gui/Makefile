# Debug level in build time
# 	-1	deactivate all log messages
# 	0	allow only error messages
# 	1	allow also warnings
# 	2	allow info messages
# 	3	allow debug messages
DEBUG_LEVEL = 5 

# executable file for project
RUN = client_app

LDFLAGS =
LOADLIBES =
LIBS = OGRE OIS CEGUI CEGUI-OGRE RakNet
LDLIBS = `pkg-config  $(LIBS) --libs`
LDLIBS += -lm -lrt  -lGL -lglut
#LDLIBS += -L../../server/trunk

CFLAGS = -m32 -g
CXXFLAGS = $(CFLAGS) 
CXXFLAGS += `pkg-config  $(LIBS) --cflags`
CXXFLAGS += -I../core
CXXFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)


OBJS = $(shell cat ./Makefile.objs ) $(shell cat ../core/Makefile.objs )
DIRS = $(shell find . -mindepth 1 -maxdepth 1 -type d -not -iname .svn )

build: compile $(RUN)

compile: $(OBJS)
#	@for dir in $(DIRS) ; do \
#		cd $$dir ; \
#		$(MAKE) $(TARGET) ; \
#		cd .. ; \
#	done

link: compile
	$(CXX) $(CXXFLAGS) -o $(RUN) $(OBJS) $(LDLIBS)


$(RUN): link
	
	
run: $(RUN)
	./$(RUN)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:	
	rm -f $(OBJS) $(RUN)
	
