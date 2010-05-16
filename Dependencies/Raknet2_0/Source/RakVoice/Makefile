include $(BASE_DIR)/makefile.defs

all: voiceshared voicestatic

voiceshared:
	$(CC) $(DEBUG) -I $(INCLUDE) -I ../ -w -c *.cpp
	$(CC) $(DEBUG) -shared -Wl-soname,librakvoice.so.2 -o $(BASE_DIR)/Lib/linux/librakvoice.so.$(VERSION) *.o $(LIBS)

voicestatic:
	$(CC) $(DEBUG) -I $(INCLUDE) -I ../ -w -c *.cpp
	$(AR) rcs $(BASE_DIR)/Lib/linux/rakvoice.$(VERSION).a *.o

clean:
	rm -f core
	rm -f *.o
	rm -f $(BASE_DIR)/Lib/linux/librakvoice.so.$(VERSION)
	rm -f $(BASE_DIR)/Lib/linux/rakvoice.$(VERSION).a
